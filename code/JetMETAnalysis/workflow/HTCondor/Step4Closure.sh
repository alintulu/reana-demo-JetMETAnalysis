#!/bin/sh --login

#BSUB -q 1nh

WorkDir=$1
File=$2
Output=$3
ID=$4

source $WorkDir/Setup_CMSSW.sh

cp $WorkDir/Files/*txt .
cp $WorkDir/Files/My*PileupHistogram.root .

echo Input files are: $File

hadd -k -f Input.root `echo $File | tr ':' ' '`

jet_correction_analyzer_x \
   -inputFilename Input.root \
   -outputDir ./ \
   -path ./ \
   -era ParallelMC \
   -levels 1 2 \
   -useweight true \
   -algs ak4pfchs \
   -drmax 0.2 \
   -evtmax 0 \
   -nbinsrelrsp 200 \
   -relrspmin 0.0 \
   -relrspmax 2.0 \
   -MCPUReWeighting MyMCPileupHistogram.root \
   -DataPUReWeighting MyDataPileupHistogram.root \
   -nrefmax 3

cp Closure_ak4pfchs.root ${Output}/Closure_ak4pfchs${ID}.root

rm *.root
