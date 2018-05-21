#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TChain.h"
#include "TEfficiency.h"
#include "TTreeReader.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include "TString.h"
#include <sstream>
#include <vector>
#include <iostream>

// a function that converts integers to TString
TString toStringI(int num) {
  ostringstream string;
  string<<num;
  TString string1 = string.str();
  return string1;
}

// a function that converts floats to TString
TString toStringF(float num) {
  ostringstream string;
  string<<num;
  TString string1 = string.str();
  return string1;
}

// Fit version
float getThreshold(TEfficiency *turnon, float eff, float initialOffset, float initialWidth){


  float xmin = turnon->GetTotalHistogram()->GetXaxis()->GetXmin();
  float xmax = turnon->GetTotalHistogram()->GetXaxis()->GetXmax();
  //TF1 *fitFunc = new TF1("fitFunc", "(0.5*TMath::Erf((x-[0])/[1]) + 0.5)", xmin, xmax);
  TF1 *fitFunc = new TF1("fitFunc", "0.97 *( 0.5*TMath::Erf((x-[0])/[1]) + 0.5)", xmin, xmax);
  fitFunc->SetParName(0, "offset");
  fitFunc->SetParName(1, "width");
  fitFunc->SetParameter(0, initialOffset);
  fitFunc->SetParameter(1, initialWidth);

  printf("Fitting to %s\n", turnon->GetName());
  turnon->Fit(fitFunc, "Q");
  float fitNum =  fitFunc->GetParameter(1)*TMath::ErfInverse(2*eff-1) + fitFunc->GetParameter(0);
  
  return fitNum;

}


//loop over all of the TEfficiency plots and add a fit
void tEffFit() {

  //Changables - modify thresholds and ratios
  int threshL = 10; //lower trigger threshold bound in GeV
  int threshH = 120; //higher trigger threshold bound in GeV
  int step = 1; //energy step in GeV
  float aStart = 0.5;
  float aFinish = 0.5;
  float bStart = 1.5;
  float bFinish = 1.5;
  float rStep = 0.1; 
  
  
  
  //Relevant quantities
  int num = ((threshH - threshL) / step) + 1; //number of TEff plots for each ratio
  float weightRatio; 
  float a; //EM contribution
  float b; //Had contribution
  int numRatios = TMath::Nint(((aFinish - aStart) / rStep) + 1);

  //Initialize the array of fitted plots
  TEfficiency* effPlot[num];
  TF1 *fitEff[num];
  
  for(int nR = 0; nR < numRatios; nR++) {
    a = aStart + (nR * rStep); //current EM contribution
    b = bStart - (nR * rStep); //current Had contribution
    weightRatio = a / b;

    TFile *manyE = new TFile("$ROOTSYS/data/data/manyEffR" + toStringF(weightRatio) + ".root");

    //go over all efficiency plots
    for(int i = 0; i < num; i++) {
      int thr = threshL + (i * step); //in GeV
      TString effName = "jetEff_" + toStringI(thr) + "GeV (Ratio:" + toStringF(weightRatio) + ")";
      effPlot[i] = (TEfficiency*)manyE->Get(effName);
      TH1F* passedHistogram = (TH1F*)effPlot[i]->GetPassedHistogram();
      int numBins = passedHistogram->GetXaxis()->GetNbins();

      float eff = 0.95;
      
      getThreshold(effPlot[i], eff, thr * 1500 , 10000.0);
	
    }
    new TCanvas();
    effPlot[10]->Draw();
    new TCanvas();
    effPlot[30]->Draw();
    new TCanvas();
        effPlot[50]->Draw();
    new TCanvas();
    effPlot[70]->Draw();
    new TCanvas();
        effPlot[90]->Draw();
    new TCanvas();
    effPlot[110]->Draw();
    
    
  }
}
  
  /*
  //FOR LOOP OVER MANY RATIOS
  for(int nR = 0; nR < numRatios; nR++) {
  a = aStart + (nR * rStep); //current EM contribution
  b = bStart - (nR * rStep); //current Had contribution
  weightRatio = a / b;

  TFile *manyE = new TFile("$ROOTSYS/data/data/manyEffR" + toStringF(weightRatio) + ".root");

    
  //go over all efficiency plots
  for(int i = 0; i < num; i++) {
  int thr = threshL + (i * step); //in GeV
  TString effName = "jetEff_" + toStringI(thr) + "GeV (Ratio:" + toStringF(weightRatio) + ")";
  effPlot[i] = (TEfficiency*)manyE->Get(effName);
  TH1F* passedHistogram = (TH1F*)effPlot->GetPassedHistogram();
  int numBins = passedHistogram->GetXaxis()->GetNbins();


  //scan over the histogram and find a fit at each energy
  for (int j = 1; j <= numBins; j++) {
  float eff = effPlot[i]->GetEfficiency(j)) 
      

  getThreshold(...)

	
	   fitEff[i] = new TF1("fit", getThreshold, 0, 200);

    }

  
  
      TString fileName = "fitsToEffR" + toStringF(weightRatio) + ".root"; 
      TFile myFile(fileName,"UPDATE");
      
      fitEff[i]->SetName(effName);
      fitEff[i]->SetTitle("title;Energy [GeV];Pass/Total");
      fitEff[i]->SetTitle(effTitle);
      fitEff[i]->Write();
  
      delete fitEff[i];
      fitEff[i] = 0;
      delete triggered[i];
      triggered[i] = 0;
      
    }// end of the if consistency check
  }
}

  */
	
