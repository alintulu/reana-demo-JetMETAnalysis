#include "TFile.h"
#include "TGraph2DErrors.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TF2.h"
#include "TAxis.h"

#include <iostream>
#include <fstream>

using namespace std;

void deriveL1() {

  string algo = "ak4pfchs";
  bool batch = true;//false;

  bool fixpar2 = false;//false;
  const int npar2 = 4;
  // FlatL1:
  //double vpar2[npar2] = {4.10637e-02, -1.10540e-01, 1.77933e-01, 2.00194e-02};
  // Non-flat:
  // double vpar2[npar2] = {3.69526e-03, -8.72147e-03, 6.52074e-02, -3.10489e-02};
  // double vpar2[npar2] = {2.23617e-02, -4.06697e-03, 1.01398e-02, 1.46351e-02};
  double vpar2[npar2] = {1.45292e-01, -2.26787e-01, 3.93751e-01, -8.30810e-02};
  TF1 *fp2 = new TF1("fp2","[0]+[1]*cosh(min(abs(x),1.5))"
  "+[2]*max(min(abs(x)-1.5,1.3),0.)"
  "+[3]*max(abs(x)-2.8,0.)",
  -5.2,5.2);

  double etabins[] =
  {-5.191,-4.889,-4.716,-4.538,-4.363,-4.191,-4.013,-3.839,-3.664,-3.489,
    -3.314, -3.139,-2.964,-2.853,-2.65, -2.5,  -2.322,-2.172,-2.043,-1.93,
    -1.83,  -1.74, -1.653,-1.566,-1.479,-1.392,-1.305,-1.218,-1.131,-1.044,
    -0.957, -0.879,-0.783,-0.696,-0.609,-0.522,-0.435,-0.348,-0.261,-0.174,
    -0.087, 0,     0.087, 0.174, 0.261, 0.348, 0.435 ,0.522, 0.609, 0.696,
    0.783,  0.879, 0.957, 1.044, 1.131, 1.218, 1.305 ,1.392, 1.479, 1.566,
    1.653,  1.74,  1.83,  1.93,  2.043, 2.172, 2.322 ,2.5,   2.65,  2.853,
    2.964,  3.139, 3.314, 3.489, 3.664, 3.839, 4.013 ,4.191, 4.363, 4.538,
    4.716,4.889,5.191};
    const int neta = sizeof(etabins)/sizeof(etabins[0])-1;

    const char *a = algo.c_str();
    //TFile *f = new TFile(Form("rootfiles/L1MC/Parameter_%s_nonflat.root",a));
    //TFile *f = new TFile(Form("rootfiles/L1MC/Parameter_%s_flatl1.root",a));
    TFile *f = new TFile("Parameter_ak4pfchs.root");
    assert(f && !f->IsZombie());

    TGraphErrors *gp0 = new TGraphErrors(neta);
    TGraphErrors *gp1 = new TGraphErrors(neta);
    TGraphErrors *gp2 = new TGraphErrors(neta);
    TGraphErrors *gc = new TGraphErrors(neta);

    for (int ieta = 0; ieta != neta; ++ieta) {

      double eta = 0.5*(etabins[ieta+1]+etabins[ieta]);
      double deta = 0.5*(etabins[ieta+1]-etabins[ieta]);

      // Use this to manually check for outliers vs eta
      //if (!batch && fabs(eta + 0.7)>0.1) continue;
      if (!batch && fabs(eta + 2.0)>0.1) continue;

      string s = Form("p_offOverA_EtaVsRhoVsJetPt/"
      "p_offOverA_RhoVsJetPt_%d",ieta+1);
      cout << s << endl << flush;
      TGraph2DErrors *g2 = (TGraph2DErrors*)f->Get(s.c_str());
      if(g2 == NULL)   // if not enough statistics, sometimes graph will be not present
         continue;
      assert(g2);


      // Canvas c1 background (gmu)
      TH1D *hmu = new TH1D(Form("hmu_%d",ieta),
      ";#LT#rho#GT (GeV);#LToffset/A#GT (GeV)",
      100,0,50);
      hmu->SetMinimum(0);
      hmu->SetMaximum(40.);

      // Canvas c2 background (gpt)
      TH1D *hpt = new TH1D(Form("hpt_%d",ieta),
      ";#LT#p_{T,reco}#GT (GeV);#LToffset/A#GT / fit",
      955,5,1000);
      hpt->SetMinimum(0.35);
      hpt->SetMaximum(2.0);

      TGraphErrors *gmu = new TGraphErrors(0);
      TGraphErrors *gpt = new TGraphErrors(0);
      for (int i = 0; i != g2->GetN(); ++i) {

        // Clean data a bit to get general trend of <pTPU> vs <rho>
        if (g2->GetEZ()[i]>0.0001 && g2->GetEZ()[i]<0.2*g2->GetZ()[i] &&
        g2->GetZ()[i]<40 &&
        g2->GetX()[i]>=0 && g2->GetX()[i]<50) {
          int n = gmu->GetN();
          gmu->SetPoint(n, g2->GetX()[i], g2->GetZ()[i]);
          gmu->SetPointError(n, g2->GetEX()[i], g2->GetEZ()[i]);
        }
      }

      // First we fit average offset vs mu
      TCanvas *c1 = (batch ? 0 : new TCanvas("c1","c1",600,600));

      if (!batch) {
        //gmu->Draw("AP");
        hmu->Draw();
        gmu->Draw("SAMEP");
        gmu->GetXaxis()->SetTitle("#LT#rho#GT (GeV)");
        gmu->GetYaxis()->SetTitle("#LToffse/At#GT (GeV)");
      }

      // [1],[2] are the slope, [0]=<rho,NoPU> (!=<rho,UE,dijet>)
      // <rho,UE,dijet> = 2.12 / 2.40 GeV (data, MC), taken by comparing dijet
      // sample to MinBias vs mu (RhoVsNPV_80XreG.pdf)
      // However, <rho,MB> at mu->0 (<NPV> -> 0) is around -0.8 GeV
      // So, <rho,NoPU,dijet>~2.2-0.8=1.4 GeV in Run II (vs 0.76 GeV in Run I)
      // <off> vs <rho> seems non-linear, add quadratic term
      TF1 *fmu1 = new TF1(Form("fmu1_%d",ieta),"[1]*(x-[0])",0,50);
      fmu1->SetParameters(1.4, 0.55);
      fmu1->FixParameter(0,1.4);
      fmu1->SetLineColor(kGreen+1);
      TF1 *fmu2 = new TF1(Form("fmu2_%d",ieta),"[1]*(x-[0])",0,50);
      fmu2->SetParameters(1.4, 0.55);
      fmu2->SetLineColor(kOrange+1);
      TF1 *fmu2b = new TF1(Form("fmu2b_%d",ieta),"[1]*(x-[0])+[2]*pow(x-[0],2)",
      0,50);
      fmu2b->SetParameters(1.4, 0.55, 0.0);
      fmu2b->FixParameter(0,1.4);
      fmu2b->SetLineColor(kMagenta+1);
      TF1 *fmu3 = new TF1(Form("fmu3_%d",ieta),"[1]*(x-[0])+[2]*pow(x-[0],2)",
      0,50);
      fmu3->SetParameters(1.4, 0.55, 0.0);

      gmu->Fit(fmu3, batch ? "QRN" : "RN");
      gmu->Fit(fmu2, batch ? "QRN" : "RN");
      gmu->Fit(fmu2b, batch ? "QRN" : "RN");
      gmu->Fit(fmu1, batch ? "QRN" : "RN");
      if (!batch) fmu3->Draw("SAME");
      if (!batch) fmu2->Draw("SAME");
      if (!batch) fmu2b->Draw("SAME");
      if (!batch) fmu1->Draw("SAME");

      if (!batch) cout << Form("fmu: chi2/NDF = %1.1f / %d\n",
      fmu1->GetChisquare(), fmu1->GetNDF()) << endl;

      if (!batch) c1->SaveAs("pdf/deriveL1_offOverA_vs_rho.pdf");

      // Next, we correct for the known mu dependence and
      // look at the mean offset vs jet pT
      // Data is cleaned more heavily from outliers at this stage
      TCanvas *c2 = (batch ? 0 : new TCanvas("c2","c2",600,600));

      TGraph2DErrors *g2b = new TGraph2DErrors();
      TGraphErrors *gmu2 = new TGraphErrors(0);
      for (int i = 0; i != g2->GetN(); ++i) {

        double rho = g2->GetX()[i];
        double off = fmu1->Eval(rho);

        // Cut outliers beyond this level
        const double sigmacut = 4.0;

        if (g2->GetEZ()[i]>0.0001 && g2->GetEZ()[i]<0.2*g2->GetZ()[i] &&
        g2->GetZ()[i]<40 &&
        g2->GetX()[i]>=0 && g2->GetX()[i]<50 &&
        fabs(g2->GetZ()[i]/off-1)<0.5 &&
        fabs(g2->GetZ()[i]/off-1)<sigmacut*g2->GetEZ()[i]/off) {

          int n = gpt->GetN();
          gpt->SetPoint(n, g2->GetY()[i], g2->GetZ()[i]/off);
          gpt->SetPointError(n, g2->GetEY()[i], g2->GetEZ()[i]/off);

          g2b->SetPoint(n, g2->GetX()[i], g2->GetY()[i], g2->GetZ()[i]);
          g2b->SetPointError(n, g2->GetEX()[i], g2->GetEY()[i], g2->GetEZ()[i]);

          gmu2->SetPoint(n, g2->GetX()[i], g2->GetZ()[i]);
          gmu2->SetPointError(n, g2->GetEX()[i], g2->GetEZ()[i]);
        }
      }

      if (!batch) gPad->SetLogx();
      if (!batch) {
        //gpt->Draw("AP");
        hpt->Draw();
        gpt->Draw("SAMEP");
      }

      // We choose pT=30 GeV as reference point. This is a nice value as 30 GeV
      // is used as minimum jet pT in most analysis, and therefore biases
      // in offset here translate to biases in analysis acceptance.
      // It is furthermore assumed that pT scaling affects all of offset,
      // therefore factorising all of mu dependence into parameter [0].
      TF1 *fpt = new TF1(Form("fpt_%d",ieta),"[0]*(1+[1]*log(x/30.))",5,1000);
      fpt->SetParameters(1,0.01);

      gpt->Fit(fpt, batch ? "QRN" : "RN");
      if (!batch) fpt->Draw("SAME");

      if (!batch) cout << Form("fpt: chi2/NDF = %1.1f / %d\n",
      fpt->GetChisquare(), fpt->GetNDF()) << endl;

      if (!batch) c2->SaveAs("pdf/deriveL1_offOverA_vs_pT_clean.pdf");

      // Draw updated c1
      if (!batch) {
        c1->cd();
        hmu->Draw();
        gmu2->Draw("SAMEPz");

        gmu2->Fit(fmu1,"QRN");
        gmu2->Fit(fmu2,"QRN");
        gmu2->Fit(fmu2b,"QRN");
        gmu2->Fit(fmu3,"QRN");

        fmu3->Draw("SAME");
        fmu2->Draw("SAME");
        fmu2b->Draw("SAME");
        fmu1->Draw("SAME");

        c1->SaveAs("pdf/deriveL1_offOverA_vs_rho_clean.pdf");
      }


      // One more iteration relative to combined mu and pT dependence
      // This is new and needed for |eta|~2 where pT dependence very strong
      TGraph2DErrors *g3b = new TGraph2DErrors();
      TGraphErrors *gpt2 = new TGraphErrors(0);
      TGraphErrors *gmu3 = new TGraphErrors(0);
      for (int i = 0; i != g2->GetN(); ++i) {

        double rho = g2->GetX()[i];
        double pt = g2->GetY()[i];
        double off = fmu1->Eval(rho)*fpt->Eval(pt);;

        // Cut outliers beyond this level
        const double sigmacut2 = 3.0;

        if (g2->GetEZ()[i]>0.0001 && g2->GetEZ()[i]<0.2*g2->GetZ()[i] &&
        g2->GetZ()[i]<40 &&
        g2->GetX()[i]>=0 && g2->GetX()[i]<50 &&
        fabs(g2->GetZ()[i]/off-1)<0.5 &&
        fabs(g2->GetZ()[i]/off-1)<sigmacut2*g2->GetEZ()[i]/off) {

          int n = gpt2->GetN();
          gpt2->SetPoint(n, g2->GetY()[i], g2->GetZ()[i]/off);
          gpt2->SetPointError(n, g2->GetEY()[i], g2->GetEZ()[i]/off);

          g3b->SetPoint(n, g2->GetX()[i], g2->GetY()[i], g2->GetZ()[i]);
          g3b->SetPointError(n, g2->GetEX()[i], g2->GetEY()[i], g2->GetEZ()[i]);

          gmu3->SetPoint(n, g2->GetX()[i], g2->GetZ()[i]);
          gmu3->SetPointError(n, g2->GetEX()[i], g2->GetEZ()[i]);
        }
      }

      gpt2->Fit(fpt, batch ? "QRN" : "RN");

      if (!batch) cout << Form("fpt: chi2/NDF = %1.1f / %d\n",
      fpt->GetChisquare(), fpt->GetNDF()) << endl;

      // Draw updated c2
      if (!batch) {
        hpt->Draw();
        gpt2->Draw("SAMEPz");
        fpt->Draw("SAME");

        c2->SaveAs("pdf/deriveL1_offOverA_vs_pT_clean2.pdf");
      }

      // Draw updated c1
      if (!batch) {
        c1->cd();
        hmu->Draw();
        gmu3->Draw("SAMEPz");

        gmu3->Fit(fmu1,"QRN");
        gmu3->Fit(fmu2,"QRN");
        gmu3->Fit(fmu2b,"QRN");
        gmu3->Fit(fmu3,"QRN");

        fmu3->Draw("SAME");
        fmu2->Draw("SAME");
        fmu2b->Draw("SAME");
        fmu1->Draw("SAME");

        c1->SaveAs("pdf/deriveL1_offOverA_vs_rho_clean2.pdf");
      }


      // Final 2D fit using the factorized 1D fits as starting values
      // for good convergence. The data is the same as in the fit vs pT,
      // which was already heavily cleaned.
      TCanvas *c3 = (batch ? 0 :new TCanvas("c3","c3",600,600));

      g2b->SetMarkerStyle(kOpenCircle);
      if (!batch) g2b->Draw("ERRP");

      TF2 *f2 = new TF2(Form("f2_%d",ieta),
      "[1]*(x-[0])*(1+[2]*log(y/30.))",0,40,10,3000);
      f2->SetParameters(fmu1->GetParameter(0),
      fmu1->GetParameter(1)*fpt->GetParameter(0),
      fpt->GetParameter(1));
      f2->FixParameter(0, fmu1->GetParameter(0));
      if (fixpar2) {
        fp2->SetParameters(vpar2[0],vpar2[1],vpar2[2],vpar2[3]);
        f2->FixParameter(2, fp2->Eval(eta));
      }

      // Constrain pT dependence in HB
      //if (fabs(eta)<1.8) {
      //f2->FixParameter(2, 7.93712e-02 + -3.98163e-02*cosh(eta));
      //}

      g2b->Fit(f2, batch ? "QRN" : "RN");

      if (!batch)
      cout << Form("f2: chi2/NDF = %1.1f / %d\n",
      f2->GetChisquare(), f2->GetNDF()) << endl;

      gp0->SetPoint(ieta, eta, f2->GetParameter(0));
      gp0->SetPointError(ieta, deta, f2->GetParError(0));
      gp1->SetPoint(ieta, eta, f2->GetParameter(1));
      gp1->SetPointError(ieta, deta, f2->GetParError(1));
      gp2->SetPoint(ieta, eta, f2->GetParameter(2));
      gp2->SetPointError(ieta, deta, f2->GetParError(2));
      gc->SetPoint(ieta, eta, f2->GetChisquare() / f2->GetNDF());
      gc->SetPointError(ieta, deta, 1. / sqrt(f2->GetNDF()));

    } // for ieta

    TCanvas *c4 = new TCanvas("c4","c4",1200,300);
    c4->Divide(4,1);

    c4->cd(1);
    gp0->SetTitle(Form("Par0 - %s",a));
    gp0->Draw("AP");
    gp0->GetYaxis()->SetRangeUser(0,1.5);

    c4->cd(2);
    gp1->SetTitle(Form("Par1 - %s",a));
    gp1->Draw("AP");
    gp1->GetYaxis()->SetRangeUser(0,1.5);

    c4->cd(3);
    gp2->SetTitle(Form("Par2 -%s",a));
    gp2->Draw("AP");
    gp2->GetYaxis()->SetRangeUser(-0.5,0.5);

    //TF1 *fp2 = new TF1("fp2","[0]+[1]*cosh(x)",-1.8,1.8);
    //fp2->SetParameters(0.05,-0.01,0.4,0.1);
    fp2->SetParameters(vpar2[0],vpar2[1],vpar2[2],vpar2[3]);
    gp2->Fit(fp2,"RN");
    fp2->Draw("SAME");

    cout << Form("fp2: chi2/NDF = %1.1f / %d\n",
    fp2->GetChisquare(), fp2->GetNDF()) << endl;

    c4->cd(4);
    gc->SetTitle("#chi^{2}/NDF");
    gc->Draw("AP");
    gc->GetYaxis()->SetRangeUser(0,3.0);

    if (batch) c4->SaveAs(Form("deriveL1_filtered_%s.pdf",a));

    // Print out text file
    ofstream fout("ParallelMC_L1FastJet_AK4PFchs.txt");
    fout << "{1         JetEta              3          JetPt           JetA            Rho               max(0.0001,1-y*([1]*(z-[0])*(1+[2]*log(x/30.)))/x)     Correction      L1FastJet}" << endl;
    for (int ieta = 0; ieta != neta; ++ieta) {

      fout << Form("     %6.3f     %6.3f          9           1        6500           0          10           0         200    %9.6f     %9.6f     %9.6f\n",
      etabins[ieta], etabins[ieta+1],
      gp0->GetY()[ieta], gp1->GetY()[ieta], gp2->GetY()[ieta]);
    } // for ieta

  }
