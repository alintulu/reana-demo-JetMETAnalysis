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
   // if(argc != 2)
   // {
   //    cerr << "Usage: " << argv[0] << " InputFileName" << endl;
   //    return -1;
   // }

   map<string, vector<string>> FileMap;

   int Bundle = 2;

   ifstream fin(argv[1]);

   if(argc > 2)
      Bundle = atoi(argv[2]);

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

      if(FileMap.find(FilePU) == FileMap.end())
         FileMap.insert(pair<string, vector<string>>(FilePU, vector<string>()));

      FileMap[FilePU].push_back(FileNoPU);
   }

   for(auto iter : FileMap)
   {
      sort(iter.second.begin(), iter.second.end());
      iter.second.erase(unique(iter.second.begin(), iter.second.end()), iter.second.end());
   }

   int BundleCount = 0;
   int Count = 0;

   vector<string> NoPUFileList;
   vector<string> PUFileList;

   for(auto iter : FileMap)
   {
      if(Bundle == 1)
      {
         NoPUFileList.clear();
         PUFileList.clear();

         PUFileList.push_back(iter.first);
         NoPUFileList = iter.second;
      }
      else
      {
         BundleCount = BundleCount + 1;
         
         PUFileList.push_back(iter.first);
         NoPUFileList.insert(NoPUFileList.end(), iter.second.begin(), iter.second.end());

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
   
   cout << "  - " << "\"" << NoPUFile << " " << PUFile << "\"" << endl;

}



