#!/bin/sh

source Setup_FileLocation.sh

Input=$WithPUFiles
Output=$Step3Output
mkdir -p $Step3Output

mkdir -p Log
SubmissionFile=Step3.condor

echo "Universe   = vanilla" > $SubmissionFile
echo "Executable = `pwd`/Step3ApplyL1.sh" >> $SubmissionFile
echo "+JobFlavour = microcentury" >> $SubmissionFile
echo "should_transfer_files = NO" >> $SubmissionFile
# echo "transfer_output_files = DONE.txt" >> $SubmissionFile
echo >> $SubmissionFile

Count=0
for File in `ls $Input | ./Columnize 20 | tr ' ' ':'`
do
   echo $File

   echo "Arguments = `pwd` $File $Output $Count" >> $SubmissionFile
   echo 'Output    = Log/Step3.out.$(Process)' >> $SubmissionFile
   echo 'Error     = Log/Step3.err.$(Process)' >> $SubmissionFile
   echo 'Log       = Log/Step3.log.$(Process)' >> $SubmissionFile
   echo 'Queue' >> $SubmissionFile
   echo >> $SubmissionFile
   
   Count=`echo $Count | AddConst 1`
done

condor_submit $SubmissionFile
