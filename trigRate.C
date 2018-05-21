#include "TFile.h"
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

void trigRate() {

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
  double x,y;
  float triggerWeightedEt;
  float weightRatio; 
  float a; //EM contribution
  float b; //Had contribution
  int numRatios = TMath::Nint(((aFinish - aStart) / rStep) + 1);

  
  //Initialize the  histogram of efficiencies
  TEfficiency* trigRate;
  TGraphAsymmErrors* graph;

  
    a = 1;
    b = 1;
    weightRatio = a / b;
    
  trigRate = new TEfficiency ("rate", "trigger rate;ET[GeV];rate[kHz]", 250,0,250000);
  
  
  //Chain the files
  TChain chain("mytree");
  chain.Add("$ROOTSYS/data/data/Threshold/user.ngrzesia.minbias_thresholded_third_final_OUTPUT.144769803/user.ngrzesia.11619258.OUTPUT._000*");

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
    	//end of if statement for eta 
      
    for (int j = 0; j < 250000; j += 1000) {
      passed = max > j;
      trigRate->Fill(passed, j);
    }	  
    } // end of j for loop
  }// end of the reader
  
  graph = trigRate->CreateGraph();
  for (int i = 0; i < graph->GetN(); i++) {
    graph->GetPoint(i,x,y);
    y = y * 40000000;
    graph->SetPoint(i,x,y);
    graph->SetPointEYhigh(i, 40000000 * (graph->GetErrorYhigh(i)));
    graph->SetPointEYlow(i, 40000000 * (graph->GetErrorYlow(i)));
  }

  TString plotName = "trigRateR" + toStringF(weightRatio);
  TString plotTitle = "Trigger Rate for Ratio: "  + toStringF(weightRatio);
  
  TCanvas *c1= new TCanvas;
  c1->SetLogy();
  graph->GetXaxis()->SetTitle("ET [GeV]");
  graph->GetYaxis()->SetTitle("rate [Hz]");
  graph->SetName(plotName);
  graph->SetTitle(plotTitle);
  graph->Draw();

  // TFile myFile("trigRatePlots.root","UPDATE");
  //graph->Write();
  
}

	
