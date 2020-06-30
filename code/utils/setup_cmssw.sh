#!/bin/bash

if [ $# -ne 2 ]; then
    echo 'Missing argument, expected CMSSW release and SCRAM ARCH version'
    exit 1
fi

RELEASE=$1
SCRAM_ARCH=$2

cd /home/cmsusr/CMSSW_10_3_3/src
eval `scramv1 runtime -sh`
