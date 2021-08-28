#!/bin/bash

#send_status_email()
#{
#  rm sendmail.txt
#  echo "Dear "$BUILD_USER"," >> sendmail.txt
#  echo "" >> sendmail.txt
#  echo "Your build is in progress!" >> sendmail.txt
#  echo "" >> sendmail.txt
#  echo "Thanks" >> sendmail.txt
#  cat sendmail.txt | heirloom-mailx -s "[Jenkins] $JOB_NAME Task #$BUILD_ID Fixed" $BUILD_USER_EMAIL
#  rm sendmail.txt
#}

#exit_with_email()
#{
#  rm sendmail.txt
#  echo "Dear "$BUILD_USER"," >> sendmail.txt
#  echo "" >> sendmail.txt
#  if [[ $1 == "1" ]]; then
#    echo "Your build had failed with below error:" >> sendmail.txt
#    echo $2  >> sendmail.txt
#  else
#    echo "Your build had successful!" >> sendmail.txt
#  fi
#  echo "" >> sendmail.txt
#  echo "Please open below link to check result:" >> sendmail.txt
#  echo $BUILD_URL >> sendmail.txt
#  echo "" >> sendmail.txt
#  echo "Thanks" >> sendmail.txt
#
#  if [[ $1 == "1" ]]; then
#    cat sendmail.txt | heirloom-mailx -s "[Jenkins] $JOB_NAME Task #$BUILD_ID failed" $BUILD_USER_EMAIL
#    echo "error is $2"
#  else
#    cat sendmail.txt | heirloom-mailx -s "[Jenkins] $JOB_NAME Task #$BUILD_ID successful" $BUILD_USER_EMAIL
#  fi
#  rm sendmail.txt
#
#  exit $1
#}

username="zhaoyong0602"
	
gerritID="update_out"
project="QD97|SM8350|R/LAHAINA/MASTER|p6.xml"
specialBranch=""
module="allu"
subModule=""
setTAG="newest"
uploadFTP=true
gerritReview=true
#--------------------------
echo ""
echo ""
echo "00. Start build"
OLD_IFS="$IFS"
IFS="+"
split_gerritID=($gerritID)
IFS="$OLD_IFS"
gerritID=${split_gerritID[0]}
magicCode=${split_gerritID[1]}

echo "input_param:"
echo "magicCode=$magicCode"
echo "gerritID=$gerritID"
echo "project=$project"
echo "module=$module"
echo "subModule=$subModule"
echo "setTAG=$setTAG"
echo "uploadFTP=$uploadFTP"
echo "gerritReview=$gerritReview"

if [[ $magicCode != "" ]]; then
	echo "magicCode=$magicCode"
	exit 1
fi

if [[ $project == "-- Please Select Project --" ]]; then
	echo"ERROR: select project is null!!!"
	exit 1	
fi

if [[ $project == "-- Android_R --" ]]; then
	echo "ERROR: select project is null!!!"
	exit 1
fi

if [[ $project == "-- Android_Q --" ]]; then
	echo "ERROR: select project is null!!!"
	exit 1
fi

PFsList="Anakis|Banagher|Dio|Dozle|Johnny|Josuke|Kars|Pucci|Judau|Jerid|Sarah|Lila|Recoa|Chara"

gotaBuild=`echo $module | grep gota`
alluBuild=`echo $module | grep allu`
amssBuild=`echo $module | grep amss`
skipList=`echo $project | grep -E "$PFsList"`

echo "input_param:"
echo gotaBuild is $gotaBuild
echo alluBuild is $alluBuild
echo amssBuild is $amssBuild
echo skipList is $skipList

if [[ $gotaBuild != "" ]] || [[ $alluBuild != "" ]] || [[ $amssBuild != "" ]]; then
	if [[ $skipList != "" ]]; then
		echo "ERROR: project not support $module build!!!"
		exit 1
	fi
fi

build_home="/home/zhaoyong/test"


########
echo ""
echo ""
echo "01. Start download"

#rm -rf workspace
mkdir -p $build_home/workspace/BUILD && cd $build_home/workspace/BUILD 

OLD_IFS="$IFS"
IFS="|"
split_project=($project)
IFS="$OLD_IFS"
download_branch=${split_project[2]}
if [[ $specialBranch != "" ]]; then
	echo "switch to special branch: $specialBranch"
	download_branch=$specialBranch
fi
download_manifest=${split_project[3]}
if [[ $download_manifest != "" ]]; then
	download_manifest="-m $download_manifest"
fi
echo "download link is $download_branch $download_manifest"
tmp_model_version=${split_project[2]}
model_version=${tmp_model_version:0:1}
echo "get actually model_version=$model_version"

if [[ $download_branch == "" ]]; then
	echo "ERROR: Please input right project!!!"
	exit 1
fi

if [[ $setTAG == "newest" ]]; then
	depthParam="--depth=5"
else
	depthParam=""
fi

echo repog init $mirrorParam $depthParam -u ssh://$username@10.74.185.240:29420/sharp/PCQ/offshore/manifest -b $download_branch $download_manifest --repo-branch=stable_sharp
repog init $mirrorParam $depthParam -u ssh://$username@10.74.185.240:29420/sharp/PCQ/offshore/manifest -b $download_branch $download_manifest --repo-branch=stable_sharp

if [[ $setTAG != "newest" ]]; then
	if [[ $setTAG == "_"* ]]; then
		echo "ERROR: Not support detect!!!"
		exit 1
	fi
	if [[ $setTAG == "" ]]; then
		echo "ERROR: TAG was null, please confirm!!!"
		exit 1
	fi

	cd .repo/manifests
	git checkout $setTAG
	manifestCommit=`git rev-parse HEAD`
	git checkout default
	git reset --hard $manifestCommit
	cd ../..
fi


let timer=0
ret="1st run"
while [[ $timer -lt 5 ]] && [[ $ret != "" ]] && false;
do
	let timer=$timer+1
	echo "No."$timer" into sync"
	rm -rf init.log
	cpuNumber=`cat /proc/cpuinfo | grep 'processor' | wc -l`
	echo "time repo sync -c -q -j2 2>&1 | tee init.log"
	time repog sync -c -q -j2 2>&1 | tee init.log
	ret=`cat init.log | grep "error: Exited sync due to fetch errors"`
done

if [[ $ret != "" ]]; then
	cd $build_home && rm -rf workspace
	echo "ERROR: Jenkins bug, repo sync error!!!"
	#exit 1	
fi
rm -rf init.log

####switch tag
if [[ $setTAG != "newest" ]]; then
	if [[ $setTAG == "_"* ]]; then
		current_save_dir=`pwd`

		ls -al .
		dirList=`ls` && splitDirList=($dirList) && echo "project folder directory number is ${#splitDirList[@]}"
		if [[ ${#splitDirList[@]} == 1 ]]; then
			workDir=${splitDirList[0]}
			cd $workDir
		else
			cd $build_home && rm -rf workspace
			echo "ERROR: Jenkins bug, code location error!!!"
			exit 1
		fi

		OLD_IFS="$IFS"
		IFS="|"
		split_project=($project)
		IFS="$OLD_IFS"
		model=${split_project[0]}
		ls -al .
		echo "MODEL/$model/" && cd MODEL/$model/

		newSetTAG=`git tag | grep $setTAG`
		setTAG=$newSetTAG

		cd $current_save_dir
	fi
	repo forall -c "git checkout -b $setTAG $setTAG"
fi

cd $build_home

########
echo ""
echo ""
echo "03. Start miyabuild"

cd workspace/BUILD_ID/ && ls -al .
dirList=`ls` && splitDirList=($dirList) && echo ${#splitDirList[@]}

if [[ ${#splitDirList[@]} == 1 ]]; then
	workDir=${splitDirList[0]}
	cd $workDir
else
	cd $build_home && rm -rf workspace
	echo "ERROR: Jenkins bug, code location error!!!"
	exit 1
fi

chmod 666 LINUX/android/Makefile && cp LINUX/android/build/make/core/root.mk LINUX/android/Makefile && chmod 444 LINUX/android/Makefile
chmod 777 customize.sh && cp tools/customize.sh customize.sh && chmod 555 customize.sh
chmod 777 emu_start.sh && cp tools/emu_start.sh emu_start.sh && chmod 555 emu_start.sh
chmod 777 load_localenv.sh && cp tools/load_localenv.sh load_localenv.sh && chmod 555 load_localenv.sh
chmod 777 miyabi_setenv.sh && cp tools/miyabi_setenv.sh miyabi_setenv.sh && chmod 555 miyabi_setenv.sh
chmod 777 miyabuild.sh && cp tools/miyabuild.sh miyabuild.sh && chmod 555 miyabuild.sh
chmod 777 modelprofile.sh && cp tools/modelprofile.sh modelprofile.sh && chmod 555 modelprofile.sh
chmod 777 out_swap.sh && cp tools/out_swap.sh out_swap.sh && chmod 555 out_swap.sh
chmod 777 sign_pack.sh && cp tools/sign_pack.sh sign_pack.sh && chmod 555 sign_pack.sh
chmod 777 remove_auto_create_folder.sh && cp tools/remove_auto_create_folder.sh remove_auto_create_folder.sh && chmod 555 remove_auto_create_folder.sh
chmod 777 gota_package.sh && cp tools/gota_package.sh gota_package.sh && chmod 555 gota_package.sh
chmod 777 securebuild.sh && cp tools/securebuild.sh securebuild.sh && chmod 555 securebuild.sh
chmod 777 create_vbmeta.sh && cp tools/create_vbmeta.sh create_vbmeta.sh && chmod 555 create_vbmeta.sh

OLD_IFS="$IFS"
IFS="|"
split_project=($project)
IFS="+"
split_module=($module)
IFS="$OLD_IFS"

model=${split_project[0]}
echo "get actually mode=$model"
echo "get actually module=${split_module[@]}"


for one_module in ${split_module[@]};
do
	echo "./miyabuild.sh $model $one_module 2>&1 | tee build.log"
	rm -rf build.log
	if [[ $one_module == "dist"* ]]; then
		export SH_BUILD_POST_ANDROID_DIST=yes
	elif [[ $one_module == "manual" ]]; then
		one_module=""
	fi
	export SH_BUILD_BOOTIMAGES=TRUE
	cpuNumber=`cat /proc/cpuinfo | grep 'processor' | wc -l` && let cpuNumber=$cpuNumber+$cpuNumber && echo "yanxidecpu $cpuNumber" && export SH_BUILD_PROC_LMT=$cpuNumber
  
	./customize.sh restore && ./customize.sh $model
  
	if [[ $model == "Mineva" ]]; then
		echo "./miyabuild.sh $model vendork"	
		./miyabuild.sh $model vendork
	fi
  
	./miyabuild.sh $model $one_module $subModule 2>&1 | tee build.log
	ret1=`cat build.log | grep "error: build_"`
	ret2=`cat build.log | grep "error : Choice"`
	ret3=`cat build.log | grep "Build imcomplete"`
	mv build.log ../../../build_${one_module}.log
	if [[ $ret1 != "" ]]; then
		cd $build_home && rm -rf workspace
		WORKSPACE=$BACKUP_WORKSPACE
		BUILD_DISPLAY_NAME=$BACKUP_BUILD_DISPLAY_NAME
		BUILD_ID=$BACKUP_BUILD_ID
		BUILD_NUMBER=$BACKUP_BUILD_NUMBER
		BUILD_TAG=$BACKUP_BUILD_TAG
		BUILD_URL=$BACKUP_BUILD_URL
		echo 1 "ERROR: build error!!!"
		exit 1
	fi

	if [[ $ret2 != "" ]]; then
		cd $build_home && rm -rf workspace
		WORKSPACE=$BACKUP_WORKSPACE
		BUILD_DISPLAY_NAME=$BACKUP_BUILD_DISPLAY_NAME
		BUILD_ID=$BACKUP_BUILD_ID
		BUILD_NUMBER=$BACKUP_BUILD_NUMBER
		BUILD_TAG=$BACKUP_BUILD_TAG
		BUILD_URL=$BACKUP_BUILD_URL
		echo "ERROR: module select error!!!"
		exit 1
	fi

	if [[ $ret3 != "" ]]; then
		cd $build_home && rm -rf workspace
		WORKSPACE=$BACKUP_WORKSPACE
		BUILD_DISPLAY_NAME=$BACKUP_BUILD_DISPLAY_NAME
		BUILD_ID=$BACKUP_BUILD_ID
		BUILD_NUMBER=$BACKUP_BUILD_NUMBER
		BUILD_TAG=$BACKUP_BUILD_TAG
		BUILD_URL=$BACKUP_BUILD_URL
		echo "ERROR: Build imcomplete. Confirm error contents from log.!!!"
		exit 1
  fi
done

WORKSPACE=$BACKUP_WORKSPACE
BUILD_DISPLAY_NAME=$BACKUP_BUILD_DISPLAY_NAME
BUILD_ID=$BACKUP_BUILD_ID
BUILD_NUMBER=$BACKUP_BUILD_NUMBER
BUILD_TAG=$BACKUP_BUILD_TAG
BUILD_URL=$BACKUP_BUILD_URL



########
echo ""
echo ""
echo "04. Start package"

cd workspace/${BUILD_ID}/ && ls -al .
dirList=`ls` && splitDirList=($dirList) && echo "project folder directory number is ${#splitDirList[@]}"

if [[ ${#splitDirList[@]} == 1 ]]; then
  workDir=${splitDirList[0]}
  cd $workDir
else
  cd $build_home && rm -rf workspace
  exit_with_email 1 "ERROR: Jenkins bug, code location error!!!"
fi

OLD_IFS="$IFS"
IFS="|"
split_project=($project)
IFS="$OLD_IFS"
model=${split_project[0]}
tmp_model_version=${split_project[2]}
model_platform=${split_project[1]}

model_version=${tmp_model_version:0:1}

echo "get actually model=$model"
echo "get actually model_version=$model_version"
echo "get actually model_platform=$model_platform"

if [[ $model == "Aquos"* ]]; then
  model_version=""
fi

source MODEL/$model/build_env.list
build_id=${SH_BUILD_ID}Jenkins${BUILD_ID}
build_number=$SH_BUILD_NUMBER

if [[ $model == "Aquos"* ]]; then
  mv ROM_IMAGE_FILES* BIN_${model^^}${model_version}_${build_id}_2020
  if [[ $uploadFTP == true ]]; then
    mkdir ${model^^}_${SH_BUILD_ID}Jenkins${BUILD_ID}_2020
    scp -r ${model^^}_${SH_BUILD_ID}Jenkins${BUILD_ID}_2020 scm@10.74.185.226:/home/gota/Build/$model_platform/
    rm -rf ${model^^}_${SH_BUILD_ID}Jenkins${BUILD_ID}_2020
    scp -r BIN_${model^^}${model_version}_${build_id}_2020 scm@10.74.185.226:/home/gota/Build/$model_platform/${model^^}_${SH_BUILD_ID}Jenkins${BUILD_ID}_2020/
  fi
  mv BIN_${model^^}${model_version}_${build_id}_2020 ../../../
else
  mv ROM_IMAGE_FILES* BIN_${model^^}${model_version}_${build_id}_2020
  tar czf BIN_${model^^}${model_version}_${build_id}_2020.tar.gz BIN_${model^^}${model_version}_${build_id}_2020
  rm -rf BIN_${model^^}${model_version}_${build_id}_2020
  if [[ $uploadFTP == true ]]; then
    mkdir ${model}
    scp -r ${model} scm@10.74.185.226:/home/gota/Build/$model_platform/
    rm -rf ${model}
    mkdir ${model^^}_${SH_BUILD_ID}Jenkins${BUILD_ID}_2020
    scp -r ${model^^}_${SH_BUILD_ID}Jenkins${BUILD_ID}_2020 scm@10.74.185.226:/home/gota/Build/$model_platform/${model}/
    rm -rf ${model^^}_${SH_BUILD_ID}Jenkins${BUILD_ID}_2020
    scp -r BIN_${model^^}${model_version}_${build_id}_2020.tar.gz scm@10.74.185.226:/home/gota/Build/$model_platform/${model}/${model^^}_${SH_BUILD_ID}Jenkins${BUILD_ID}_2020/
  fi
  mv BIN_${model^^}${model_version}_${build_id}_2020.tar.gz ../../../
fi
gotaBuild=`echo $module | grep gota`
if [[ $gotaBuild != "" ]]; then
  rm -rf MODEL/$model/ROM_IMAGE_GOTA/IMAGES
  rm -rf MODEL/$model/ROM_IMAGE_GOTA/META
  rm -rf MODEL/$model/ROM_IMAGE_GOTA/RADIO
  if [[ $model == "Aquos"* ]]; then
    mv MODEL/$model/ROM_IMAGE_GOTA/ GOTA_${model^^}${model_version}_${build_id}
    if [[ $uploadFTP == true ]]; then
      scp -r GOTA_${model^^}${model_version}_${build_id} scm@10.74.185.226:/home/gota/Build/$model_platform/${model^^}_${SH_BUILD_ID}Jenkins${BUILD_ID}_2020/
    fi
    mv GOTA_${model^^}${model_version}_${build_id} ../../../
  else
    mv MODEL/$model/ROM_IMAGE_GOTA/ GOTA_${model^^}${model_version}_${build_id}
    tar czf GOTA_${model^^}${model_version}_${build_id}.tar.gz GOTA_${model^^}${model_version}_${build_id}
    rm -rf GOTA_${model^^}${model_version}_${build_id}
    if [[ $uploadFTP == true ]]; then
      scp -r GOTA_${model^^}${model_version}_${build_id}.tar.gz scm@10.74.185.226:/home/gota/Build/$model_platform/${model}/${model^^}_${SH_BUILD_ID}Jenkins${BUILD_ID}_2020/
    fi
    mv GOTA_${model^^}${model_version}_${build_id}.tar.gz ../../../
  fi
fi

cd $build_home

#!/bin/bash
########
echo ""
echo ""
echo "05. Finish the build"
if [[ $magicCode != "yxSaveCode" ]]; then
  rm -rf workspace
fi

if [[ $gerritReview == true ]]; then
  review_list=`cat cherrypick.log`
  for review_id in $review_list;
  do
    echo $review_id
    ssh -p 29418 lx17070011@10.24.71.91 gerrit review $review_id --code-review +1 --message "$model\ build\ successful\ with\ $build_id\ $module\ $subModule\ in\ Jenkins\ Task\ #${BUILD_ID}"
  done
fi

echo ""
echo "ROM build successful with below information:"
echo "model: $model"
echo "build_id: $build_id"
echo "build_number: $build_number"
echo ""

if [[ $model == "Aquos"* ]]; then
  modelFolder=""
  endName=""
else
  modelFolder=${model}"/"
  endName=".tar.gz"
fi

echo ""
echo "ROM save at:"
if [[ $uploadFTP == true ]]; then
  echo "ftp://10.74.185.226/Build/"$model_platform"/"${modelFolder}${model^^}"_"${SH_BUILD_ID}"Jenkins"${BUILD_ID}"_2020/BIN_"${model^^}${model_version}"_"${build_id}"_2020"${endName}
fi
echo "Windows: \\\\"${NODE_NAME}"\\newBuildJenkins\\"${BUILD_ID}"\\BIN_"${model^^}${model_version}"_"${build_id}"_2020"${endName}
echo "Ubuntu : smb://"${NODE_NAME}"/newBuildJenkins/"${BUILD_ID}"/BIN_"${model^^}${model_version}"_"${build_id}"_2020"${endName}
echo ""

gotaBuild=`echo $module | grep gota`
if [[ $gotaBuild != "" ]]; then
  echo "GOTA save at:"
  if [[ $uploadFTP == true ]]; then
    echo "ftp://10.74.185.226/Build/"$model_platform"/"${modelFolder}${model^^}"_"${SH_BUILD_ID}"Jenkins"${BUILD_ID}"_2020/GOTA_"${model^^}${model_version}"_"${build_id}${endName}
  fi
  echo "Windows: \\\\"${NODE_NAME}"\\newBuildJenkins\\"${BUILD_ID}"\\GOTA_"${model^^}${model_version}"_"${build_id}${endName}
  echo "Ubuntu : smb://"${NODE_NAME}"/newBuildJenkins/"${BUILD_ID}"/GOTA_"${model^^}${model_version}"_"${build_id}${endName}
  echo ""
fi

exit_with_email 0 "successful"
