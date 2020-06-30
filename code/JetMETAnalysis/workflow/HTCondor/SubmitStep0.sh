Proxy_path              = /afs/cern.ch/user/a/adlintul/CMSSW_10_6_1/src/batch/x509up
executable              = step10_ntuple.sh
arguments               = $(Proxy_path) $(ClusterId) $(ProcId) input/step10/filenames_PU_FULL.txt 5 0 PU3
output                  = output/step10.$(ClusterId).$(ProcId).out
error                   = error/step10.$(ClusterId).$(ProcId).err
log                     = log/step10.$(ClusterId).log
WHEN_TO_TRANSFER_OUTPUT = ON_EXIT_OR_EVICT
+SpoolOnEvict           = False
+JobFlavour		= "workday"

queue 343
