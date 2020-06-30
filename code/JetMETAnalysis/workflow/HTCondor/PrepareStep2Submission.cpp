#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
using namespace std;

int main(int argc, char *argv[]);
void PrintJob(vector<string> &NoPU, vector<string> &PU, int Count);

int main(int argc, char *argv[])
{
   map<string, vector<string>> FileMap;

   int Bundle = 5;

   if(argc > 1)
      Bundle = atoi(argv[1]);

   ifstream fin("Files/MatchedFiles");

   while(fin)
   {
      char ch[1048576] = "";
      fin.getline(ch, 1048575, '\n');
      stringstream str(ch);

      int Lumi = -1;
      string FileNoPU = "";
      string FilePU = "";

      str >> Lumi >> FileNoPU >> FilePU;

      if(Lumi < 0 || FileNoPU == "" || FilePU == "")
         continue;

      if(FileMap.find(FileNoPU) == FileMap.end())
         FileMap.insert(pair<string, vector<string>>(FileNoPU, vector<string>()));

      FileMap[FileNoPU].push_back(FilePU);
   }

   for(auto iter : FileMap)
   {
      sort(iter.second.begin(), iter.second.end());
      iter.second.erase(unique(iter.second.begin(), iter.second.end()), iter.second.end());
   }

   int BundleCount = 0;
   int Count = 0;

   cout << "#!/bin/sh" << endl;
   cout << endl;
   cout << "source Setup_FileLocation.sh" << endl;
   cout << "mkdir -p $Step2Output" << endl;
   cout << endl;
   cout << "mkdir -p Log" << endl;
   cout << "SubmissionFile=Step2.condor" << endl;
   cout << endl;
   cout << "echo \"Universe   = vanilla\" > $SubmissionFile" << endl;
   cout << "echo \"Executable = `pwd`/Step2PUMatching.sh\" >> $SubmissionFile" << endl;
   cout << "echo \"+JobFlavour = microcentury\" >> $SubmissionFile" << endl;
   cout << "echo \"should_transfer_files = NO\" >> $SubmissionFile" << endl;
   cout << "echo >> $SubmissionFile" << endl;
   cout << endl;

   vector<string> NoPUFileList;
   vector<string> PUFileList;

   for(auto iter : FileMap)
   {
      if(Bundle == 1)
      {
         NoPUFileList.clear();
         PUFileList.clear();

         NoPUFileList.push_back(iter.first);
         PUFileList = iter.second;
      }
      else
      {
         BundleCount = BundleCount + 1;
         
         NoPUFileList.push_back(iter.first);
         PUFileList.insert(PUFileList.end(), iter.second.begin(), iter.second.end());

         if(BundleCount % Bundle != 0)
            continue;
      }

      PrintJob(NoPUFileList, PUFileList, Count);

      Count = Count + 1;

      NoPUFileList.clear();
      PUFileList.clear();
   }
   if(NoPUFileList.size() > 0 && PUFileList.size() > 0)
      PrintJob(NoPUFileList, PUFileList, Count);

   cout << "condor_submit $SubmissionFile" << endl;

   fin.close();

   return 0;
}

void PrintJob(vector<string> &NoPU, vector<string> &PU, int Count)
{
   if(NoPU.size() == 0 | PU.size() == 0)
      return;

   sort(NoPU.begin(), NoPU.end());
   sort(PU.begin(), PU.end());

   NoPU.erase(unique(NoPU.begin(), NoPU.end()), NoPU.end());
   PU.erase(unique(PU.begin(), PU.end()), PU.end());

   string NoPUFile = "";
   string PUFile = "";

   NoPUFile = "";
   for(auto File : NoPU)
   {
      if(NoPUFile != "")
         NoPUFile = NoPUFile + ":";
      NoPUFile = NoPUFile + File;
   }
   PUFile = "";
   for(auto File : PU)
   {
      if(PUFile != "")
         PUFile = PUFile + ":";
      PUFile = PUFile + File;
   }

   cout << "echo \"Arguments = `pwd` "
      << NoPUFile << " " << PUFile << " $Step2Output " << Count << "\" >> $SubmissionFile" << endl;
   cout << "echo 'Output    = Log/Step2.out.$(Process)' >> $SubmissionFile" << endl;
   cout << "echo 'Error     = Log/Step2.err.$(Process)' >> $SubmissionFile" << endl;
   cout << "echo 'Log       = Log/Step2.log.$(Process)' >> $SubmissionFile" << endl;
   cout << "echo 'Queue' >> $SubmissionFile" << endl;
   cout << endl;
}



