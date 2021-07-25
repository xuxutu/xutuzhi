#!/bin/bash

str=$1
old_IFS="$IFS"
IFS="|"
#split_str=($str)
split_str=$str
echo $split_str

IFS="$old_IFS"
br=${split_str[2]}

echo "heh"

echo ${split_str[0]}
echo ${split_str[1]}
echo ${split_str[2]}
echo ${split_str[3]}
