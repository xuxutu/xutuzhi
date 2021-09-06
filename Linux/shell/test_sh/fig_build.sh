#!/bin/bash

###### 01. download code
echo "01. download code"

echo gerritID is $Gerrit_IDs

echo project is $project

build_home=`pwd`
echo work_home is $build_home


#获取信息
OLD_IFS="$IFS"
IFS="|"
split_project=($project) #数组化
echo split_project is $split_project
echo split_project[0] is ${split_project[0]} #数组引用
echo split_project[1] is ${split_project[1]}
echo split_project[2] is ${split_project[2]}
echo split_project[3] is ${split_project[3]}
echo split_project[4] is ${split_project[4]}
manifest="${split_project[3]}_SP_BUILD.xml"
download_branch=${split_project[2]}

repo=${split_project[4]}
IFS="$OLD_IFS"

code_home=""

#创建工作目录
if [ ! -d "${split_project[0]}" ]; then
    mkdir -p ${split_project[0]} && cd ${split_project[0]}
    workdir=`pwd`
    code_home=$workdir
    
    #初始化代码指令
    echo "${split_project[4]} init  --depth=1 -u ssh://zhaoyong0602@10.74.185.240:29420/FIH/QC/manifest -b $download_branch -m $manifest"
    ${split_project[4]} init  --depth=1 -u ssh://zhaoyong0602@10.74.185.240:29420/FIH/QC/manifest -b $download_branch -m $manifest

    let timer=0
    ret="1st run"
    while [[ $timer -lt 5 ]] && [[ $ret != "" ]]; do
        let timer=$timer+1
        echo "No."$timer" into sync"
        rm -rf init.log
        cpuNumber=`cat /proc/cpuinfo | grep 'processor' | wc -l`
        echo "time ${split_project[4]} sync -c -q -j${cpuNumber} 2>&1 | tee init.log"
        time ${split_project[4]} sync -c -q -j${cpuNumber} 2>&1 | tee init.log
        #touch init.log
        #echo "error: Exited sync due to fetch errors"	> init.log
        ret=`cat init.log | grep "error: Exited sync due to fetch errors"`
    done
	
	rm init.log

    if [[ $ret != "" ]]; then
	    cd $build_home && rm -rf workspace
	    exit 1
    fi
       
else	
	cd ${split_project[0]}
    workdir=`pwd`
    code_home=$workdir
	#初始化代码指令
    echo "${split_project[4]} init  --depth=1 -u ssh://zhaoyong0602@10.74.185.240:29420/FIH/QC/manifest -b $download_branch -m $manifest"
    #${split_project[4]} init --depth=1 -u ssh://zhaoyong0602@10.74.185.240:29420/FIH/QC/manifest -b $download_branch -m $manifest
    
    #切换或创建base分支
    echo "$repo forall -c \"$repo start BASE .\""
    #$repo forall -c "$repo start BASE ."
 
 	if [ $? -eq 1 ]; then
 		echo "error: branch switch failed!"
        exit 1	
 	fi
  	#更新代码
    let timer=0
    ret="1st run"
    while [[ $timer -lt 5 ]] && [[ $ret != "" ]]; do
        let timer=$timer+1
        echo "No."$timer" into sync"
        rm -rf init.log
        cpuNumber=`cat /proc/cpuinfo | grep 'processor' | wc -l`
        pwd
        echo "time ${split_project[4]} sync -c -q -j${cpuNumber} 2>&1 | tee init.log"
        #time ${split_project[4]} sync -c -q -j${cpuNumber} 2>&1 | tee init.log
        #touch init.log
        #echo "error: Exited sync due to fetch errors"	> init.log
        ret=`cat init.log | grep "error: Exited sync due to fetch errors"`
    done
	
	rm init.log

    if [[ $ret != "" ]]; then
	    #cd $build_home && rm -rf workspace
	    exit 1
    fi
 
    #删除SP分支
    echo "$repo forall -c \"git branch -D SP\""
    #$repo forall -c "git branch -D SP"
    
    #创建并切换至SP分支
    echo "$repo forall -c \"$repo start SP .\""
    #$repo forall -c "$repo start SP ."
fi

###### 02. cherry pick
echo "02. cherry pick"
declare -A patch_info
declare -A xml
declare -A cmd_list

function get_patchinfo()
{   

    ssh -p 29418 sharp_secpatch_gr@10.208.60.201 gerrit query --current-patch-set $1 > patch_info_$1.txt
    
    patch_info[change_ref]=$(grep  'ref: ref' patch_info_$1.txt | tr -s ' ' | cut -d' ' -f3)
    echo ${patch_info[change_ref]} is null
    
    patch_info[change_pro]=$(grep  'project: ' patch_info_$1.txt | awk  '{print $2}')
    echo ${patch_info[change_pro]}
    
    echo ${patch_info[*]}
    #cat patch_info_$1.txt
    rm patch_info_$1.txt
}

manifest=`grep "include" .repo/manifest.xml | sed 's/.*"\(.*\)".*/\1/'`
echo $manifest
if [ $? -eq 1 ]; then
    echo "error: .repo is not exist!"
    exit 1
fi


if [ -n "$Gerrit_IDs" ]; then

	#切换或创建SP分支
    echo "$repo forall -c \"$repo start SP .\""
    $repo forall -c "$repo start SP ."

	if [ $? -eq 1 ]; then
 		echo "error: branch switch failed!"
        exit 1	
 	fi


    echo "=====checkout gerrit id:${Gerrit_IDs}...====="

    #数组化,','或‘\n’分割
    change_ids=(${Gerrit_IDs/,})

    #格式化
    echo gerritID: ${change_ids[@]}

    #workdir

    workdir=`pwd`

    #遍历
    echo "getting patchinfo..."
    for gid in ${change_ids[@]}
    do
        #echo $gid
        #获取信息
        get_patchinfo $gid

        cmd_list[$gid]="git fetch ssh://sharp_secpatch_gr@10.208.60.201:29418/${patch_info[change_pro]} ${patch_info[change_ref]} && git cherry-pick FETCH_HEAD"

        pro=${patch_info[change_pro]}


		tmp=""
        

        
        for line in $manifest #${manifest[@]}
        do
            echo line is $line 
            echo "grep $pro .repo/manifests/$line"    
            tmp=`grep "\"${pro#*/}\"" .repo/manifests/$line | awk -F '[\"\"]' '{print \$6}'`
            if [[ $tmp != "" ]]; then
            	echo "res is $tmp"
                break
            fi
            tmp=""
        done
      
        
        

		if [[ $tmp == "" ]]; then
			echo "error: not found project!"
			exit 1	
		fi
	
		xml[${patch_info[change_pro]}]=$tmp

        echo "git fetch ssh://sharp_secpatch_gr@10.208.60.201:29418/${patch_info[change_pro]} ${patch_info[change_ref]} && git cherry-pick FETCH_HEAD"
        #echo "git fetch ssh://lx19060027@hrots514:29418/${patch_info[change_pro]} ${patch_info[change_ref]} && git cherry-pick FETCH_HEAD"
        #cd
        cd ${xml[${patch_info[change_pro]}]}
        if [ $? -eq 1 ]; then
            echo "error: code path is not exist!"
            exit 1
        fi
        #git fetch ssh://lx19060027@hrots514:29418/${patch_info[change_pro]} ${patch_info[change_ref]} && git cherry-pick FETCH_HEAD            
        git fetch ssh://sharp_secpatch_gr@10.208.60.201:29418/${patch_info[change_pro]} ${patch_info[change_ref]} && git cherry-pick FETCH_HEAD
        if [ $? -eq 1 ]; then
            echo "error: cherry pick failed!"
            echo "git cherry-pick --abort"
            git cherry-pick --abort
            exit 1
        fi

        cd $workdir
        #echo ${patch_info[change_pro]} is ${xml[${patch_info[change_pro]}]}

        #cherry-pick
        #echo cd ${xml[${patch_info[change_pro]}]}
        #git fetch ssh://lx19060027@hrots514:29418/${patch_info[change_pro]} ${patch_info[change_ref]} && git cherry-pick FETCH_HEAD            
    done    
fi

#print
#echo ${xml[*]}
#echo ${cmd_list[*]}
#
#for gid in ${change_ids[@]}
#do
#   echo ${cmd_list[$gid]}
#done
echo ""

echo "project   --> path"
for pros in ${!xml[*]}
do
    echo "$pros --> ${xml[$pros]}"
done

echo ""

echo "change_id --> command"
for cmds in ${!cmd_list[*]}
do
    echo "$cmds --> ${cmd_list[$cmds]}"
done


###### 03. build img
echo "03. build img"

cd $code_home
#echo "~/Build_FIH_list/fih_build.sh ${split_project[0]} $module $subModule"
#~/Build_FIH_list/fih_build.sh ${split_project[0]} $module $subModule 


mo=`date +%m`
da=`date +%d`
fulldate=`date +%Y%m%d`
maj=${mo:1:1}
min=${mo:0:1}${da}


case ${split_project[0]} in
	"OF6")
    	cd LINUX/android
		echo "scp zhaoyong@10.74.185.129:~/build_fih_list/build_of6.sh ./"	
		scp zhaoyong@10.74.185.129:~/build_fih_list/build_of6.sh ./
        ./build_of6.sh
		#upload img
        #res=$ret1&&$ret2&&$ret3&&$ret4&&$ret
        #res=0
		if [ $? -eq 0 ]; then 
			echo "OF6 buid success! waiting for uploading imgs..."
			#创建目录
			date=`date -u +%Y%m%d`
			ssh zhaoyong@10.74.185.129 "mkdir -p /home/zhaoyong/RomImage/${split_project[0]}/$date/$BUILD_ID"			

            cd out/target/product/fih_sdm660_64/
            scp ./obj/PACKAGING/product-67JP_intermediates/67JP-product.img ./obj/PACKAGING/odm-67JP_intermediates/67JP-odm.img ./obj/PACKAGING/systemimage_intermediates/system.img ./vbmeta.img ./vendor.img ./boot.img zhaoyong@10.74.185.129:/home/zhaoyong/RomImage/${split_project[0]}/$date/$BUILD_ID
			echo "Rom upload over! Please get rom at ftp://10.74.185.129/home/zhaoyong/RomImage"        
        else
        	echo "OF6 build failed!"
            exit 1	
        fi		
    ;;
    
    "VG2")
    		
    ;;
    
    "OI6")
    	cd LINUX/android
		echo "scp zhaoyong@10.74.185.129:~/build_fih_list/build_oi6.sh ./"	
		scp zhaoyong@10.74.185.129:~/build_fih_list/build_oi6.sh ./
        ./build_oi6.sh
        
        if [ $? -eq 0 ]; then 
			echo "OI6 buid success! waiting for uploading imgs..."      
			#创建目录
			date=`date -u +%Y%m%d`
			ssh zhaoyong@10.74.185.129 "mkdir -p /home/zhaoyong/RomImage/${split_project[0]}/$date/$BUILD_ID"
            cd out/target/product/OI6_BSP
            scp ./vendor.img ./product.img ./vbmeta_system.img ./vbmeta.img ./boot.img ./super.img zhaoyong@10.74.185.129:/home/zhaoyong/RomImage/${split_project[0]}/$date/$BUILD_ID
			echo "Rom upload over! Please get rom at ftp://10.74.185.129/home/zhaoyong/RomImage"        
        else
        	echo "OI6 build failed!"
            exit 1
        fi	
    ;;
    
    "OG6")
    	cd LINUX/android
		echo "scp zhaoyong@10.74.185.129:~/build_fih_list/build_og6.sh ./"	
		scp zhaoyong@10.74.185.129:~/build_fih_list/build_og6.sh ./
        ./build_og6.sh
        
        if [ $? -eq 0 ]; then 
			echo "OG6 buid success! waiting for uploading imgs..."      
			#创建目录
			date=`date -u +%Y%m%d`
			ssh zhaoyong@10.74.185.129 "mkdir -p /home/zhaoyong/RomImage/${split_project[0]}/$date/$BUILD_ID"
            cd out/target/product/OG6_BSP
            scp ./vendor.img ./recovery.img ./vbmeta.img ./boot.img ./dtbo.img zhaoyong@10.74.185.129:/home/zhaoyong/RomImage/${split_project[0]}/$date/$BUILD_ID
            cd ../qssi
            scp system.img product.img vbmeta_system.img zhaoyong@10.74.185.129:/home/zhaoyong/RomImage/${split_project[0]}/$date/$BUILD_ID
            
			echo "Rom upload over! Please get rom at ftp://10.74.185.129/home/zhaoyong/RomImage/${split_project[0]}/$date/$BUILD_ID"        
        else
        	echo "OG6 build failed!"
            exit 1
        fi	
    ;;
    
    *)
	
    ;;
esac   

###### 04. Restore branch
#echo "04. Restore branch"

#切换或创建base分支
#echo "$repo forall -c \"$repo start BASE .\""
#$repo forall -c "$repo start BASE ."
