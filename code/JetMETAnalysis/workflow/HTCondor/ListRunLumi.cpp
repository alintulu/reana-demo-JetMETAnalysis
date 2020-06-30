#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

#include "TFile.h"
#include "TTree.h"

int main(int argc, char *argv[])
{
   if(argc != 2)
   {
      cerr << "Usage: " << argv[0] << " InputFileName" << endl;
      return -1;
   }

   TFile File(argv[1]);

   TTree *Tree = (TTree *)File.Get("ak4pf/t");

   long long run, lumi, evt;
   Tree->SetBranchAddress("run", &run);
   Tree->SetBranchAddress("lumi", &lumi);
   Tree->SetBranchAddress("evt", &evt);

   vector<pair<long, long>> AllPairs;
   
   int EntryCount = Tree->GetEntries();
   for(int iE = 0; iE < EntryCount; iE++)
   {
      Tree->GetEntry(iE);
      AllPairs.push_back(pair<long, long>(run, lumi));
   }

   sort(AllPairs.begin(), AllPairs.end());
   AllPairs.erase(unique(AllPairs.begin(), AllPairs.end()), AllPairs.end());

   for(int i = 0; i < (int)AllPairs.size(); i++)
      cout << AllPairs[i].first << " " << AllPairs[i].second << " " << argv[1] << endl;

   File.Close();

   return 0;
}






