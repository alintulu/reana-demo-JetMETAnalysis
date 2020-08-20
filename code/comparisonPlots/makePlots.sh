#!/bin/bash

rawPath=https://raw.githubusercontent.com/cms-jet/JECDatabase/master/textFiles
versions='Summer16_07Aug2017_V11_MC Summer16_07Aug2017_V20_MC'
algo='AK4PFchs'
levels='L1FastJet'

for version in $versions; do
    mkdir -p JECDatabase/$version
    for level in $levels; do
        wget $rawPath/$version/$version\_$level\_$algo\.txt -P JECDatabase/$version
        wget $rawPath/$version/$version\_"Uncertainty"\_$algo\.txt -P JECDatabase/$version
    done
done