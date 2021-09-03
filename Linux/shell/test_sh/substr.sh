#!/bin/bash


mo=`date +%m`
da=`date +%d`
fulldate=`date +%Y%m%d`

echo mo is $mo
maj=${mo:1:1}
echo maj is $maj

echo da is $da
min=${mo:0:1}${da}
echo min is $min

