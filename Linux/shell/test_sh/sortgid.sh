#!/bin/bash

gerritID=`egrep -o '[0-9]{5}' gerrit.txt`

i=0

for gid in $gerritID
do
	array[i]=$gid
	echo -e "${array[i]} \c"
	let i++
done

#sort
echo ""
size=${#array[*]}

tmp=0;

for((i = 0; i < size; i++))
do
	for((j = i + 1; j < size; j++))
	do
		#echo  ${array[i]}
		if [ ${array[i]} -gt ${array[j]} ]; then
			#tmp=${array[i]}
			tmp=${array[i]}
			array[i]=${array[j]}
			array[j]=$tmp	
		fi
	done
done

echo "排序后:"

for((i = 0; i < size; i++))
do
	echo -e "${array[i]} \c"
done

echo ""

