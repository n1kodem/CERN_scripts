#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TCanvas.h"
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


void comboPlotRatioSingle() {

  //Changables - modify thresholds and ratios
  int threshL = 10; //lower trigger threshold bound in GeV
  int threshH = 120; //higher trigger threshold bound in GeV
  int step = 1; //energy step in GeV

  
  //Relevant quantities
  int num = 76;
  float weightRatio; 
  Double_t energy[num];
  Double_t rate[num];
  double x,y;

  float a = 0.5;
  float b = 1.5;
  weightRatio = a / b;
  
  //Open and point to the rate graph
  TFile *trigR = new TFile("$ROOTSYS/data/data/Threshold/trigRatePlots.root");
  TString plotName = "trigRateR" + toStringF(weightRatio);
  TGraphAsymmErrors *rateGraph = (TGraphAsymmErrors*)trigR->Get(plotName);


    
  //Open the many efficiency graphs file
  //TString fileName = "manyE" + toStringF(weightRatio);
  TFile *manyE = new TFile("$ROOTSYS/data/data/Threshold/manyEffR" + toStringF(weightRatio) + ".root");
  /*
    if (a == 0.5) { numBins = 92; }
    if (a == 0.6) { numBins = 103; }
    if (a == 0.7) { numBins = 110; }
    if (a == 0.8) { numBins = 117; }
    if (a > 0.8)  { num = 120; }
  */
    
  //go over all efficiency plots
  for(int i = 0; i < num; i++) {
      int thr = threshL + (i * step); //in GeV
      TString effName = "jetEff_" + toStringI(thr) + "GeV (Ratio:" + toStringF(weightRatio) + ")";
      TEfficiency* effPlot = (TEfficiency*)manyE->Get(effName);
      TH1F* passedHistogram = (TH1F*)effPlot->GetPassedHistogram();
      int numBins = passedHistogram->GetXaxis()->GetNbins();
      
      //retrieve the energy at 95% efficiency using a moving average of 7 bins
      for (int j = 1; j <= numBins - 6; j++) {

	float movingAverage7 = (effPlot->GetEfficiency(j) + effPlot->GetEfficiency(j + 1) + effPlot->GetEfficiency(j + 2) + effPlot->GetEfficiency(j + 3) + effPlot->GetEfficiency(j + 4) + effPlot->GetEfficiency(j + 5) + effPlot->GetEfficiency(j + 6)) / 7;

	if(movingAverage7 >= 0.95) {
	  energy[i] = (j + 3) * 1000; //in MeV;
	  // this can be done because of the binning 1bin/1GeV;
 
	  
	
	  //retrieve the rate corresponding to the energy
	  for(int k = 0; k < rateGraph->GetN(); k++) {
	    rateGraph->GetPoint(k, x, y);
	    if(x >= thr * 1000) {
	      rate[i] = y;
	      printf("Finished %i GeV threshold. Offline ET = %i GeV. Rate = %f \n",thr,j,y);
	      break;
	    }
	  }
	  break;
	}
      }
  }
    
    TGraph *comboPlot = new TGraph(num, energy, rate);
    /*
      TLatex *latex1 = new TLatex(comboPlot->GetX()[0], comboPlot->GetY()[0], "10GeV trigger threshold");
      latex1->SetTextSize(0.03);
      latex1->SetTextColor(kBlue);
      comboPlot->GetListOfFunctions()->Add(latex1);
      TLatex *latex2 = new TLatex(comboPlot->GetX()[9], comboPlot->GetY()[9],  "30GeV trigger threshold");
      latex2->SetTextSize(0.03);
      latex2->SetTextColor(kBlue);
      comboPlot->GetListOfFunctions()->Add(latex2);
      TLatex *latex3 = new TLatex(comboPlot->GetX()[19], comboPlot->GetY()[19], "50");
      latex3->SetTextSize(0.03);
      latex3->SetTextColor(kBlue);
      comboPlot->GetListOfFunctions()->Add(latex3);
      TLatex *latex4 = new TLatex(comboPlot->GetX()[29], comboPlot->GetY()[29],  "70");
      latex4->SetTextSize(0.03);
      latex4->SetTextColor(kBlue);
      comboPlot->GetListOfFunctions()->Add(latex4);
      TLatex *latex5 = new TLatex(comboPlot->GetX()[39], comboPlot->GetY()[39], "90");
      latex5->SetTextSize(0.03);
      latex5->SetTextColor(kBlue);
      comboPlot->GetListOfFunctions()->Add(latex5);
      TLatex *latex6 = new TLatex(comboPlot->GetX()[49], comboPlot->GetY()[49],  "110");
      latex6->SetTextSize(0.03);
      latex6->SetTextColor(kBlue);
      comboPlot->GetListOfFunctions()->Add(latex6);
      comboPlot->SetTitle("Rate for 90% trigger 'turn on' as a function of offline energy");
    */
   
    TCanvas *c1= new TCanvas;
    c1->SetLogy();
    comboPlot->GetXaxis()->SetTitle("Energy [MeV]");
    comboPlot->GetYaxis()->SetTitle("Rate [Hz]");
    comboPlot->SetTitle("Rate for 95% trigger 'turn on' vs. offline Energy. EM/Had Ratio: " + toStringF(weightRatio));
    comboPlot->SetName("comboPlotR" + toStringF(weightRatio));
    //Open a file; make TEff plots and write them
    TString fileName = "comboPlotsMA7.root"; 
    //TFile myFile(fileName,"UPDATE");
    //comboPlot->Write();
    comboPlot->Draw("APL*");    

    
  
}

    



   




    



  

	
