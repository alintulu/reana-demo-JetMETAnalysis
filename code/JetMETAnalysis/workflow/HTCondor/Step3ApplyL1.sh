#!/bin/sh --login

#BSUB -q 1nh

WorkDir=$1
Files=$2
Output=$3
ID=$4

source $WorkDir/Setup_CMSSW.sh

cp $WorkDir/Files/*txt .
cp $WorkDir/Files/My*PileupHistogram.root .

echo Input files are: $Files

hadd -k -f Input.root `echo $Files | tr ':' ' '`

jet_apply_jec_x \
   -input Input.root \
   -output JRA_jecl1.root \
   -jecpath ./ \
   -era ParallelMC \
   -levels 1 \
   -algs ak4pfchs \
   -L1FastJet true \
   -saveitree false

cp $CMSSW_BASE/src/JetMETAnalysis/JetAnalyzers/config/jra_dr_finebinning.config jra.config

jet_response_analyzer_x jra.config \
   -input JRA_jecl1.root \
   -nbinsabsrsp 0 \
   -nbinsetarsp 0 \
   -nbinsphirsp 0 \
   -nbinsrelrsp 200 \
   -doflavor false \
   -flavorDefinition phys \
   -MCPUReWeighting MyMCPileupHistogram.root \
   -MCPUHistoName pileup \
   -DataPUReWeighting MyDataPileupHistogram.root \
   -DataPUHistoName pileup \
   -output jra.root \
   -useweight true \
   -nrefmax 3 \
   -algs ak4pfchsl1 \
   -drmax 0.2 \
   -relrspmin 0.0 \
   -relrspmax 2.0

cp jra.root ${Output}/JRA_jecl1${ID}.root

rm *.root
