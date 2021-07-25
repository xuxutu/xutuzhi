#!/bin/bash

project=$1

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

  #exit $1
}

#VG2|V2R|VG2/dev/SM6250AB|Rennell.LA.2.0

OLD_IFS="$IFS"
IFS="|"
split_project=($project)
echo "split_project: "
echo $split_project
IFS="$OLD_IFS"
download_branch=${split_project[2]}
echo $download_branch

download_manifest=$manifest
echo "download link is $download_branch $download_manifest"
tmp_model_version=${split_project[1]}
model_version=${tmp_model_version: -1}
echo "get actually model_version=$model_version"

if [[ $download_branch == "" ]]; then
  exit_with_email 1 "ERROR: Please input right project!!!"
fi

if [[ $download_manifest == "" ]]; then
  exit_with_email 1 "ERROR: Please input right manifest!!!"
fi
