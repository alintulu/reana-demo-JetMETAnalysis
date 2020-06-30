#!/bin/sh --login

#BSUB -q 1nh

WorkDir=$1
File=$2
Output=$3

source $WorkDir/Setup_CMSSW.sh

echo "$WorkDir/RunListRunLumi $2 > $3"
$WorkDir/RunListRunLumi $2 > Result.txt
mv Result.txt $3

