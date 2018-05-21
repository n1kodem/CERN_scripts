e#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TChain.h"
#include "TEfficiency.h"
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



void trigRateRatio() {

  //Changables - modify thresholds and ratios
  int threshL = 10; //lower trigger threshold bound in GeV
  int threshH = 120; //higher trigger threshold bound in GeV
  int step = 1; //energy step in GeV
  float aStart = 0.5;
  float aFinish = 1.0;
  float bStart = 1.5;
  float bFinish = 1.0;
  float rStep = 0.1; 
  
  //Relevant quantities
  int num = ((threshH - threshL) / step) + 1; //number of TEff plots for each ratio
  float weightRatio; 
  float a; //EM contribution
  float b; //Had contribution
  int numRatios = TMath::Nint(((aFinish - aStart) / rStep) + 1);
  float triggerWeightedEt;

  
  //Initialize the  histogram of efficiencies
  TEfficiency* trigRate[numRatios];
  TGraphAsymmErrors* graph[numRatios];

  //FOR LOOP OVER MANY RATIOS
  for(int nR = 0; nR < numRatios; nR++) {
    a = aStart + (nR * rStep); //current EM contribution
    b = bStart - (nR * rStep); //current Had contribution
    weightRatio = a / b;
    
  trigRate[nR] = new TEfficiency ("rate", "trigger rate;ET[GeV];rate[kHz]", 250,0,250000);
  
  
  //Chain the files
  TChain chain("mytree");
  //chain.Add("$ROOTSYS/data/data/Threshold/user.ngrzesia.mc15_14TeV_gg_hh125_4b_lambda01_19062017_OUTPUT.141435611/user.ngrzesia.11516841.OUTPUT._0000*.root");

  chain.Add("$ROOTSYS/data/data/Threshold/user.ngrzesia.minbias_thresholded_third_final_OUTPUT.144769803/user.ngrzesia.11619258.OUTPUT._00000*.root");


  //Create a TTreeReader (call it reader) for the chain
  TTreeReader reader(&chain);

  //Read the data
  TTreeReaderValue<Int_t> bunchN(reader, "distFrontBunchTrain");
  TTreeReaderValue<std::vector<float>> triggerEt(reader,"JFexJet_Et");
  TTreeReaderValue<std::vector<float>> triggerEt_EM(reader,"JFexJet_Et_EM");
  TTreeReaderValue<std::vector<float>> triggerEt_Had(reader,"JFexJet_Et_Had");
  TTreeReaderValue<std::vector<float>> triggerEta(reader,"JFexJet_eta");
  bool passed;
  while (reader.Next()) {
    Int_t  n = *bunchN;
    double max = 0.0;
    if (n <= 20) continue;
    for (int i = 0; i < triggerEt->size(); i++) {
      if (fabs(triggerEta->at(i)) < 3.2) {
	triggerWeightedEt = (a * triggerEt_EM->at(i)) + (b * triggerEt_Had->at(i));
	if (triggerWeightedEt > max) max = triggerWeightedEt;
      } //end of i loop
    }	//end of if statement for eta 
      
    for (int j = 0; j < 250000; j += 1000) { //added 000
      passed = max > j;
      trigRate[nR]->Fill(passed, j);
  	  
    } // end of j for loop
  }// end of the reader
  double x, y;
  graph[nR] = trigRate[nR]->CreateGraph();
  for (int i = 0; i < graph[nR]->GetN(); i++) {
    graph[nR]->GetPoint(i,x,y);
    y = y * 40000000;
    graph[nR]->SetPoint(i,x,y);
    graph[nR]->SetPointEYhigh(i, 40000000 * (graph[nR]->GetErrorYhigh(i)));
    graph[nR]->SetPointEYlow(i, 40000000 * (graph[nR]->GetErrorYlow(i)));
  }

  TString plotName = "trigRateR" + toStringF(weightRatio);
  TString plotTitle = "Trigger Rate for Ratio: "  + toStringF(weightRatio);
  TCanvas *c1 = new TCanvas;
  c1->SetLogy();
  graph[nR]->GetXaxis()->SetTitle("ET [GeV]");
  graph[nR]->GetYaxis()->SetTitle("rate [Hz]");
  graph[nR]->Draw();
  graph[nR]->SetName(plotName);
  graph[nR]->SetTitle(plotTitle);
  // TFile myFile("trigRatePlots.root","UPDATE");
  // graph[nR]->Write();
  }
}

	
