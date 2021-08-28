#!/bin/bash

send_status_email()
{
  rm sendmail.txt
  echo "Dear "$BUILD_USER"," >> sendmail.txt
  echo "" >> sendmail.txt
  echo "Your build is in progress!" >> sendmail.txt
  echo "" >> sendmail.txt
  echo "Thanks" >> sendmail.txt
  cat sendmail.txt | heirloom-mailx -s "[Jenkins] $JOB_NAME Task #$BUILD_ID Fixed" $BUILD_USER_EMAIL
  rm sendmail.txt
}

exit_with_email()
{
  rm sendmail.txt
  echo "Dear "$BUILD_USER"," >> sendmail.txt
  echo "" >> sendmail.txt
  if [[ $1 == "1" ]]; then
    echo "Your build had failed with below error:" >> sendmail.txt
    echo $2  >> sendmail.txt
  else
    echo "Your build had successful!" >> sendmail.txt
  fi
  echo "" >> sendmail.txt
  echo "Please open below link to check result:" >> sendmail.txt
  echo $BUILD_URL >> sendmail.txt
  echo "" >> sendmail.txt
  echo "Thanks" >> sendmail.txt

  if [[ $1 == "1" ]]; then
    cat sendmail.txt | heirloom-mailx -s "[Jenkins] $JOB_NAME Task #$BUILD_ID failed" $BUILD_USER_EMAIL
    echo "error is $2"
  else
    cat sendmail.txt | heirloom-mailx -s "[Jenkins] $JOB_NAME Task #$BUILD_ID successful" $BUILD_USER_EMAIL
  fi
  rm sendmail.txt

  exit $1
}

########
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
echo "gerritID=$gerritID"
echo "project=$project"
echo "module=$module"
echo "subModule=$subModule"
echo "setTAG=$setTAG"
echo "uploadFTP=$uploadFTP"
echo "gerritReview=$gerritReview"
if [[ $magicCode != "" ]]; then
  echo "magicCode=$magicCode"
fi

if [[ $project == "-- Please Select Project --" ]]; then
  exit_with_email 1 "ERROR: select project is null!!!"
fi

if [[ $project == "-- Android_R --" ]]; then
  exit_with_email 1 "ERROR: select project is null!!!"
fi

if [[ $project == "-- Android_Q --" ]]; then
  exit_with_email 1 "ERROR: select project is null!!!"
fi

PFsList="Anakis|Banagher|Dio|Dozle|Johnny|Josuke|Kars|Pucci|Judau|Jerid|Sarah|Lila|Recoa|Chara"

gotaBuild=`echo $module | grep gota`
alluBuild=`echo $module | grep allu`
amssBuild=`echo $module | grep amss`
skipList=`echo $project | grep -E "$PFsList"`

if [[ $gotaBuild != "" ]] || [[ $alluBuild != "" ]] || [[ $amssBuild != "" ]]; then
  if [[ $skipList != "" ]]; then
    exit_with_email 1 "ERROR: project not support $module build!!!"
  fi
fi

gerritID=`python -c '''
import sys
gerritID = sys.argv
gerrit_list = gerritID[1:]
new_gerrit_list = []
for id in gerrit_list:
    if id not in new_gerrit_list:
        new_gerrit_list.append(id)
new_gerritID = " ".join(new_gerrit_list)
print new_gerritID
''' $gerritID`


build_home=`pwd`

echo "home is $build_home"

#!/bin/bash
########
echo ""
echo ""
echo "01. Start download"

rm -rf workspace
mkdir -p workspace/${BUILD_ID} && cd workspace/${BUILD_ID}

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
  exit_with_email 1 "ERROR: Please input right project!!!"
fi

if [[ $setTAG == "newest" ]]; then
  depthParam="--depth=5"
else
  depthParam=""
fi

#if [[ -d /home/gerrit/yanxi/mirror ]];then
#  mirrorParam="--reference=/home/gerrit/yanxi/mirror"
#else
  mirrorParam=""
#fi

echo "repo init $mirrorParam $depthParam -u ssh://liuyx0410@10.74.185.240:29420/sharp/PCQ/offshore/manifest -b $download_branch $download_manifest --repo-branch=stable_sharp"
repo init $mirrorParam $depthParam -u ssh://liuyx0410@10.74.185.240:29420/sharp/PCQ/offshore/manifest -b $download_branch $download_manifest --repo-branch=stable_sharp

if [[ $setTAG != "newest" ]]; then
  if [[ $setTAG == "_"* ]]; then
    exit_with_email 1 "ERROR: Not support detect!!!"
  fi
  if [[ $setTAG == "" ]]; then
    exit_with_email 1 "ERROR: TAG was null, please confirm!!!"
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
while [[ $timer -lt 5 ]] && [[ $ret != "" ]]; do
  let timer=$timer+1
  echo "No."$timer" into sync"
  rm -rf init.log
  cpuNumber=`cat /proc/cpuinfo | grep 'processor' | wc -l`
  echo "time repo sync -c -q -j2 2>&1 | tee init.log"
  time repo sync -c -q -j2 2>&1 | tee init.log
  ret=`cat init.log | grep "error: Exited sync due to fetch errors"`
done

if [[ $ret != "" ]]; then
  cd $build_home && rm -rf workspace
  exit_with_email 1 "ERROR: Jenkins bug, repo sync error!!!"
fi
rm -rf init.log

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
      exit_with_email 1 "ERROR: Jenkins bug, code location error!!!"
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

#!/bin/bash
########
echo ""
echo ""
echo "02. Start change code"

currentDir=`pwd`

cd workspace/${BUILD_ID}

if [[ $gerritID != "update_out" ]]; then
  rm -rf cherrypick.log
  for ID in $gerritID;
  do
    cp_change_id=$ID
    cp_change_endofid=${cp_change_id:$((-2))}
    ssh -p 29418 lx17070011@10.24.71.91 gerrit query --format=TEXT --current-patch-set $cp_change_id > tmpfile
    grep_project=`cat tmpfile | grep "  project: "`;split_project=($grep_project);cp_change_project=${split_project[1]}
    grep_branch=`cat tmpfile | grep "  branch: "`;split_branch=($grep_branch);cp_change_branch=${split_branch[1]}
    grep_patchset=`cat tmpfile | grep "    number: "`;split_patchset=($grep_patchset);cp_change_patchset=${split_patchset[1]}
    grep_commitid=`cat tmpfile | grep "    revision: "`;split_commitid=($grep_commitid);cp_change_commitid=${split_commitid[1]}
    if [[ $cp_change_project"X" == "X" || $cp_change_branch"X" == "X" || $cp_change_patchset"X" == "X" || $cp_change_commitid"X" == "X" ]]; then cat tmpfile; cd $build_home && rm -rf workspace; exit_with_email 1 "ERROR: not detect the gerrit id";fi
    rm tmpfile

    repo info $cp_change_project > tmpfile
    grep_path=`cat tmpfile | grep "Mount path: "`;split_path=($grep_path);cp_target_path=${split_path[2]}
    grep_branch=`cat tmpfile | grep "Current revision: "`;split_branch=($grep_branch);cp_target_branch=${split_branch[2]}
    if [[ $cp_target_path"X" == "X" || $cp_target_branch"X" == "X" ]]; then cat tmpfile; cd $build_home && rm -rf workspace; exit_with_email 1 "ERROR: not detect the repo info";fi
    rm tmpfile

    echo $cp_change_project,$cp_change_branch,$cp_change_patchset,$cp_change_commitid,${cp_change_commitid:0:7}
    echo $cp_target_path,$cp_target_branch
    echo "start --repo $cp_change_project --gerrit_id $cp_change_id --revision $cp_change_patchset --commit_id ${cp_change_commitid:0:7}"
    
    cd $cp_target_path
    git show -s $cp_change_commitid
    ret=$?
    cd $currentDir/workspace/${BUILD_ID}/
    if [[ $ret == "0" ]]; then
      echo "[JenkinsDebug] patch is exist"
    else
      if [[ $cp_change_branch == $cp_target_branch ]]; then
        ssh yanxi@10.74.185.200 "mkdir -p /cherryPick/${BUILD_ID}"
        ssh yanxi@10.74.185.200 "cp /cherryPick/input/* /cherryPick/${BUILD_ID}/"
        timeout 600 ssh yanxi@10.74.185.200 "python /cherryPick/${BUILD_ID}/get_gerrit_patch.py --repo $cp_change_project --gerrit_id $cp_change_id --revision $cp_change_patchset --commit_id ${cp_change_commitid:0:7}"
        scp yanxi@10.74.185.200:/cherryPick/${BUILD_ID}/${cp_change_commitid:0:7}.diff.zip .
        ssh yanxi@10.74.185.200 "rm -rf /cherryPick/${BUILD_ID}"
        unzip ${cp_change_commitid:0:7}.diff.zip -d ./
        cd $cp_target_path
        ret1=`git am --keep-cr $currentDir/workspace/${BUILD_ID}/${cp_change_commitid:0:7}.diff`
        ret2=$(echo $ret1 | grep "git am --abort")
        if [[ $ret2 != "" ]] || [[ ! -f $currentDir/workspace/${BUILD_ID}/${cp_change_commitid:0:7}.diff ]]; then
          git am --abort
          ret3=$(cat $currentDir/workspace/${BUILD_ID}/${cp_change_commitid:0:7}.diff | grep "Binary files differ")
          if [[ $ret3 != "" ]] || [[ ! -f $currentDir/workspace/${BUILD_ID}/${cp_change_commitid:0:7}.diff ]]; then
            cd $currentDir/workspace/${BUILD_ID}/
            retryCpfgNum=1
            while true
            do
              echo "repo cpfg -u lx17070011 -g http://10.24.71.91/gerrit -c $cp_change_id,$cp_change_patchset 2>&1 | tee cpfg_$cp_change_id.log"
              timeout 1800 repo cpfg -u lx17070011 -g http://10.24.71.91/gerrit -c $cp_change_id,$cp_change_patchset 2>&1 | tee cpfg_$cp_change_id.log
              sed -i '/\[detached\ HEAD/d' cpfg_$cp_change_id.log
              cpfg_ret_fatal="";cpfg_ret_fatal=`cat cpfg_$cp_change_id.log | grep -c fatal`
              cpfg_ret_error="";cpfg_ret_error=`cat cpfg_$cp_change_id.log | grep -c error`
              cpfg_ret_network="";cpfg_ret_network=`cat cpfg_$cp_change_id.log | grep -c "he remote end hung up unexpectedly"`
              rm cpfg_$cp_change_id.log
              if [[ $cpfg_ret_network != "0" ]] && [[ $retryCpfgNum != "5" ]]; then
                echo "network error, retry cpfg..."
                let retryCpfgNum=$retryCpfgNum+1
                sleep 30
                continue
              fi
              if [[ $cpfg_ret_fatal != "0" ]] || [[ $cpfg_ret_error != "0" ]]; then
                if [[ $cpfg_ret_network != "0" ]]; then
                  cd $cp_target_path
                  retryCherrypickNum=1
                  while true
                  do
                    echo "bash -c \"git fetch --depth=2 ssh://lx17070011@10.24.71.91:29418/$cp_change_project refs/changes/$cp_change_endofid/$cp_change_id/$cp_change_patchset && git cherry-pick FETCH_HEAD\" 2>&1 | tee $currentDir/workspace/${BUILD_ID}/cherrypick_$cp_change_id.log"
                    timeout 1800 bash -c "git fetch --depth=2 ssh://lx17070011@10.24.71.91:29418/$cp_change_project refs/changes/$cp_change_endofid/$cp_change_id/$cp_change_patchset && git cherry-pick FETCH_HEAD" 2>&1 | tee $currentDir/workspace/${BUILD_ID}/cherrypick_$cp_change_id.log
                    sed -i '/\[detached\ HEAD/d' $currentDir/workspace/${BUILD_ID}/cherrypick_$cp_change_id.log
                    cherrypick_ret_fatal="";cherrypick_ret_fatal=`cat $currentDir/workspace/${BUILD_ID}/cherrypick_$cp_change_id.log | grep -c fatal`
                    cherrypick_ret_error="";cherrypick_ret_error=`cat $currentDir/workspace/${BUILD_ID}/cherrypick_$cp_change_id.log | grep -c error`
                    cherrypick_ret_network="";cherrypick_ret_network=`cat $currentDir/workspace/${BUILD_ID}/cherrypick_$cp_change_id.log | grep -c "he remote end hung up unexpectedly"`
                    rm $currentDir/workspace/${BUILD_ID}/cherrypick_$cp_change_id.log
                    if [[ $cherrypick_ret_network != "0" ]] && [[ $retryCherrypickNum != "5" ]]; then
                      echo "network error, retry cherrypick..."
                      let retryCherrypickNum=$retryCherrypickNum+1
                      sleep 30
                      continue
                    fi
                    if [[ $cherrypick_ret_fatal != "0" ]] || [[ $cherrypick_ret_error != "0" ]]; then
                      if [[ $cherrypick_ret_network != "0" ]]; then
                        cd $build_home && rm -rf workspace; exit_with_email 1 "ERROR: Jenkins bug, cherry pick fatal or error!!! May due to network error, please connect scm to check."
                      else
                        echo "######################################################################################################"
                        echo "####  ERROR: $ret1"
                        echo "######################################################################################################"
                        cd $build_home && rm -rf workspace; exit_with_email 1 "ERROR: cherry pick failed, please check."
                      fi
                    fi
                    break
                  done
                  cd $currentDir/workspace/${BUILD_ID}/
                else
                  echo "######################################################################################################"
                  echo "####  ERROR: $ret1"
                  echo "######################################################################################################"
                  cd $build_home && rm -rf workspace; exit_with_email 1 "ERROR: cherry pick failed, please check."
                fi
              fi
              break
            done
            cd $cp_target_path
            echo "[JenkinsDebug] cherry pick successful:"
            git log -1 --pretty=format:'%h %ad | %s%d [%an]'
            echo ""
          else
            echo "######################################################################################################"
            echo "####  ERROR: $ret1"
            echo "######################################################################################################"
            cd $build_home && rm -rf workspace; exit_with_email 1 "ERROR: cherry pick failed, please check."
          fi
        else
          echo "[JenkinsDebug] cherry pick successful:"
          git log -1 --pretty=format:'%h %ad | %s%d [%an]'
          echo ""
        fi
      else
        cd $build_home && rm -rf workspace; exit_with_email 1 "ERROR: gerrit branch not same with local code"
      fi
      cd $currentDir/workspace/${BUILD_ID}/
    fi
    cd $currentDir/workspace/${BUILD_ID}/
    rm ${cp_change_commitid:0:7}.diff ${cp_change_commitid:0:7}.diff.zip
    echo $cp_change_commitid >> cherrypick.log
  done
  mv cherrypick.log $build_home
else
  echo "update_out"
fi

if [[ $magicCode == "yxPending" ]]; then
  touch delete_this_file_after_edit
  chmod 777 delete_this_file_after_edit
  echo "gerrit123" | sudo -S cp /etc/samba/smb.conf /etc/samba/bak_smb.conf
  echo "gerrit123" | sudo -S chmod 777 /etc/samba/smb.conf
  echo "[Jenkins${BUILD_ID}]" >> /etc/samba/smb.conf
  echo "  comment = shared" >> /etc/samba/smb.conf
  echo "  browseable = yes" >> /etc/samba/smb.conf
  echo "  path = ${currentDir}" >> /etc/samba/smb.conf
  echo "  create mask = 0777" >> /etc/samba/smb.conf
  echo "  directory mask = 0777" >> /etc/samba/smb.conf
  echo "  force user = gerrit" >> /etc/samba/smb.conf
  echo "  force group = gerrit" >> /etc/samba/smb.conf
  echo "  public = yes" >> /etc/samba/smb.conf
  echo "  available = yes" >> /etc/samba/smb.conf
  echo "  writable = yes" >> /etc/samba/smb.conf
  echo "  guest ok = yes" >> /etc/samba/smb.conf
  echo "gerrit123" | sudo -S service smbd restart
  echo "gerrit123" | sudo -S chmod 644 /etc/samba/smb.conf
  starttime=`date +'%Y-%m-%d %H:%M:%S'`
  start_seconds=$(date --date="$starttime" +%s)
  while [[ -f delete_this_file_after_edit ]]; do
    endtime=`date +'%Y-%m-%d %H:%M:%S'`
    end_seconds=$(date --date="$endtime" +%s)
    if [[ $((end_seconds-start_seconds)) -le 1800 ]]; then
      sleep 30
    else
      echo "gerrit123" | sudo -S mv /etc/samba/bak_smb.conf /etc/samba/smb.conf
      echo "gerrit123" | sudo -S service smbd restart
      rm delete_this_file_after_edit
      exit_with_email 1 "ERROR: edit failed with timeout"
    fi
  done
  echo "finish"
  echo "gerrit123" | sudo -S mv /etc/samba/bak_smb.conf /etc/samba/smb.conf
  echo "gerrit123" | sudo -S service smbd restart
fi

cd $build_home

#!/bin/bash
########
echo ""
echo ""
echo "03. Start miyabuild"

cd workspace/${BUILD_ID}/ && ls -al .
dirList=`ls` && splitDirList=($dirList) && echo ${#splitDirList[@]}

if [[ ${#splitDirList[@]} == 1 ]]; then
  workDir=${splitDirList[0]}
  cd $workDir
else
  cd $build_home && rm -rf workspace
  exit_with_email 1 "ERROR: Jenkins bug, code location error!!!"
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

BACKUP_WORKSPACE=$WORKSPACE
BACKUP_BUILD_DISPLAY_NAME=$BUILD_DISPLAY_NAME
BACKUP_BUILD_ID=$BUILD_ID
BACKUP_BUILD_NUMBER=$BUILD_NUMBER
BACKUP_BUILD_TAG=$BUILD_TAG
BACKUP_BUILD_URL=$BUILD_URL
export -n WORKSPACE
export -n BUILD_DISPLAY_NAME
export -n BUILD_ID
export -n BUILD_NUMBER
export -n BUILD_TAG
export -n BUILD_URL

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
  #
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
    exit_with_email 1 "ERROR: build error!!!"
  fi
  if [[ $ret2 != "" ]]; then
    cd $build_home && rm -rf workspace
    WORKSPACE=$BACKUP_WORKSPACE
    BUILD_DISPLAY_NAME=$BACKUP_BUILD_DISPLAY_NAME
    BUILD_ID=$BACKUP_BUILD_ID
    BUILD_NUMBER=$BACKUP_BUILD_NUMBER
    BUILD_TAG=$BACKUP_BUILD_TAG
    BUILD_URL=$BACKUP_BUILD_URL
    exit_with_email 1 "ERROR: module select error!!!"
  fi
  if [[ $ret3 != "" ]]; then
    cd $build_home && rm -rf workspace
    WORKSPACE=$BACKUP_WORKSPACE
    BUILD_DISPLAY_NAME=$BACKUP_BUILD_DISPLAY_NAME
    BUILD_ID=$BACKUP_BUILD_ID
    BUILD_NUMBER=$BACKUP_BUILD_NUMBER
    BUILD_TAG=$BACKUP_BUILD_TAG
    BUILD_URL=$BACKUP_BUILD_URL
    exit_with_email 1 "ERROR: Build imcomplete. Confirm error contents from log.!!!"
  fi
done

WORKSPACE=$BACKUP_WORKSPACE
BUILD_DISPLAY_NAME=$BACKUP_BUILD_DISPLAY_NAME
BUILD_ID=$BACKUP_BUILD_ID
BUILD_NUMBER=$BACKUP_BUILD_NUMBER
BUILD_TAG=$BACKUP_BUILD_TAG
BUILD_URL=$BACKUP_BUILD_URL

cd $build_home

#!/bin/bash
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
