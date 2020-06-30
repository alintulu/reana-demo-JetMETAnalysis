#!/bin/sh

source Setup_FileLocation.sh

cat $Step1Output/*/*txt | ./RunMatchFiles > Files/MatchedFiles




