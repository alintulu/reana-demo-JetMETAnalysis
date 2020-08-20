#include <iostream>
#include <vector>
using namespace std;

#include "CommandLine.h"

#include "JetUncertainty.h"

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string Version = CL.Get("Version");
   string Algorithm = CL.Get("Algorithm");

   string Dependent = CL.Get("Dependent");
   double Min = CL.GetDouble("Min");
   double Max = CL.GetDouble("Max");
   int NBin = CL.GetInt("NBin", 1000);

   double FixRho = CL.GetDouble("FixRho", 20);
   double FixPT = CL.GetDouble("FixPT", 100);
   double FixEta = CL.GetDouble("FixEta", 0.0);
   double FixPhi = CL.GetDouble("FixPhi", 0.0);
   double FixArea = CL.GetDouble("FixArea", 0.4 * 0.4 * M_PI);

   enum ModeType {Rho, PT, Eta, Phi, Area} Mode;

   if(Dependent == "Rho")   Mode = Rho;
   if(Dependent == "PT")    Mode = PT;
   if(Dependent == "Eta")   Mode = Eta;
   if(Dependent == "Phi")   Mode = Phi;

   string FileName = "JECDatabase/" + Version + "/" + Version + "_Uncertainty_" + Algorithm + ".txt";
   JetUncertainty JEU(FileName);

   string Tag = Dependent;
   if(Mode != PT)    Tag = Tag + Form("_PT%.02f", FixPT);
   if(Mode != Eta)   Tag = Tag + Form("_Eta%.02f", FixEta);

   //int EtaBinCount = 40;
   //double EtaBinEdge[] = {-5.4, -5.0, -4.4, -4.0, -3.5, -3.0, -2.8, -2.6, -2.4, -2.2, -2.0, -1.8, -1.6, -1.4, -1.2, -1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.5, 4.0, 4.4, 5.0, 5.4};
   int EtaBinCount = 82;
   double EtaBinEdge[] = {-5.191, -4.889, -4.716, -4.538, -4.363, -4.191, -4.013, -3.839, -3.664, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043, -1.93, -1.83, -1.74, -1.653, -1.566, -1.479, -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.879, -0.783, -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087, 0, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191, 4.363, 4.538, 4.716, 4.889, 5.191};
   vector<double> EtaBins;
   for(int i = 0; i < EtaBinCount; i++)
   {
      EtaBins.push_back(EtaBinEdge[i] + 0.0001);
      EtaBins.push_back(EtaBinEdge[i+1] - 0.0001);
   }
   if(Mode == Eta)
      NBin = EtaBins.size() - 1;

   for(int i = 0; i <= NBin; i++)
   {
      double X;
      if(Mode == PT)
         X = exp(log(Min) + (log(Max) - log(Min)) / NBin * i);
      else if(Mode == Eta)
         X = EtaBins[i];
      else
         X = Min + (Max - Min) / NBin * i;

      JEU.SetJetPT(FixPT);
      JEU.SetJetEta(FixEta);
      JEU.SetJetPhi(FixPhi);
      JEU.SetRho(FixRho);
      JEU.SetJetArea(FixArea);

      if(Mode == Rho)   JEU.SetRho(X);
      if(Mode == PT)    JEU.SetJetPT(X);
      if(Mode == Eta)   JEU.SetJetEta(X);
      if(Mode == Phi)   JEU.SetJetPhi(X);
      if(Mode == Area)  JEU.SetJetArea(X);

      double Y = JEU.GetUncertainty().second;

      cout  << X << " " << Y << endl;
   }

   return 0;
}
