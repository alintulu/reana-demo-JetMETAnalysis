#!/bin/bash

[[ -z "$7" ]] && { echo "Provide seven parameters (7th: PU/noPU)" ; exit 1; }

proxy=$1
cluster_id=$2
job_id=$3
filename=$4
batch=$5
number_events=$6
PU=$7
start=$((job_id*batch))

source /afs/cern.ch/user/a/adlintul/testjec/CMSSW_10_2_5/src/batch/setupCMSSW.sh $proxy

cmsRun JetMETAnalysis/JetAnalyzers/test/run_JRA_cfg.py +i $filename +b $batch +id $job_id +sf $start +ne $number_events +pu $PU

#cmsRun JetMETAnalysis/JetAnalyzers/test/run_JRA_cfg.py +i input/step10/files_1_PU.txt +o TEST{}.root +ne 10 +id 10
