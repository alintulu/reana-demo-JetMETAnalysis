import FWCore.ParameterSet.Config as cms
inputFiles = cms.untracked.vstring(
        'root://cmsxrootd.fnal.gov///store/mc/RunIIAutumn18DRPremix/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/AODSIM/102X_upgrade2018_realistic_v15_ext1-v1/60000/F74D3F1C-3BC1-4E4D-83B4-D7E6F00E3724.root',
        )
source = cms.Source("PoolSource", fileNames = inputFiles )
