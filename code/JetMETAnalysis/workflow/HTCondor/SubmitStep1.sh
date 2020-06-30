#!/bin/sh

source Setup_FileLocation.sh

mkdir -p $Step1Output/ResultNoPU $Step1Output/ResultWithPU

mkdir -p Log
SubmissionFile=Step1.condor

echo "Universe   = vanilla" > $SubmissionFile
echo "Executable = `pwd`/Step1ListRunLumi.sh" >> $SubmissionFile
echo "+JobFlavour = microcentury" >> $SubmissionFile
echo "should_transfer_files = NO" >> $SubmissionFile
# echo "transfer_output_files = DONE.txt" >> $SubmissionFile
echo >> $SubmissionFile

Input=$NoPUFiles
for i in $Input
do
   echo "Arguments = `pwd` $i $Step1Output/ResultNoPU/`basename $i | sed "s/root/txt/g"`" >> $SubmissionFile
   echo 'Output    = Log/Step1NoPU.out.$(Process)' >> $SubmissionFile
   echo 'Error     = Log/Step1NoPU.err.$(Process)' >> $SubmissionFile
   echo 'Log       = Log/Step1NoPU.log.$(Process)' >> $SubmissionFile
   echo 'Queue' >> $SubmissionFile
done
 
Input=$WithPUFiles
for i in $Input
do
   echo "Arguments = `pwd` $i $Step1Output/ResultWithPU/`basename $i | sed "s/root/txt/g"`" >> $SubmissionFile
   echo 'Output    = Log/Step1WithPU.out.$(Process)' >> $SubmissionFile
   echo 'Error     = Log/Step1WithPU.err.$(Process)' >> $SubmissionFile
   echo 'Log       = Log/Step1WithPU.log.$(Process)' >> $SubmissionFile
   echo 'Queue' >> $SubmissionFile
done

condor_submit $SubmissionFile
