#!/bin/bash

#用于所有机型的cherry-pick

declare -A patch_info
declare -A xml
declare -A cmd_list

function get_patchinfo()
{   

    ssh -p 29418 sharp_secpatch_gr@10.208.60.201 gerrit query --current-patch-set $1 > patch_info_$1.txt
    
    patch_info[change_ref]=$(grep  'ref: ref' patch_info_$1.txt | tr -s ' ' | cut -d' ' -f3)
    #echo ${patch_info[change_ref]}
    
    patch_info[change_pro]=$(grep  'project: ' patch_info_$1.txt | awk  '{print $2}')
    #echo ${patch_info[change_pro]}
    
    #echo ${patch_info[*]}
    #cat patch_info_$1.txt
    rm patch_info_$1.txt
}


Gerrit_IDs="359018"    #gerrit id 可以是一个或者多个


manifest=`grep "include" .repo/manifest.xml | sed 's/.*"\(.*\)".*/\1/'`
if [ $? -eq 1 ]; then
    echo "error: .repo is not exist!"
    exit 1
fi


if [ -n "$Gerrit_IDs" ]; then
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
        for line in $manifest
        do
            echo $line
            #grep $pro .repo/manifests/$line    
            tmp=`grep "\"${pro#*/}\"" .repo/manifests/$line | awk -F '[\"\"]' '{print \$6}'`
            if [[ $tmp != "" ]]; then
                break
            fi
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
