#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

int main()
{
   map<int, vector<string>> LumiMap;

   while(cin)
   {
      char ch[1048576] = "";
      cin.getline(ch, 1048575, '\n');
      stringstream str(ch);

      int Run = -1;
      int Lumi = -1;
      string File = "";

      str >> Run >> Lumi >> File;

      if(Run < 0 || Lumi < 0 || File == "")
         continue;

      if(LumiMap.find(Lumi) == LumiMap.end())
         LumiMap.insert(pair<int, vector<string>>(Lumi, vector<string>()));

      LumiMap[Lumi].push_back(File);
   }

   for(auto iter = LumiMap.begin(); iter != LumiMap.end(); iter++)
   {
      if(iter->second.size() != 2)
         continue;

      cout << iter->first << " " << iter->second[0] << " " << iter->second[1] << endl;
   }

   return 0;
}






