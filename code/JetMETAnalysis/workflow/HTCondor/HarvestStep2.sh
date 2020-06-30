#!/bin/sh

source Setup_FileLocation.sh

hadd -k -f $Step2Output/output_ak4pfchs.root $Step2Output/Result_*root

jet_synchfit_x \
   -inputDir  $Step2Output/ \
   -outputDir `pwd`/Files/ \
   -algo1 ak4pfchs \
   -algo2 ak4pfchs \
   -highPU false \
   -useNPU false \
   -functionType standard \
   -era ParallelMCPreDeriveL1



