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




void manyEff_R_13() { 
  
  int threshL = 10; //lower trigger threshold bound in GeV
  int threshH = 120; //higher trigger threshold bound in GeV
  int step = 10; //energy step in GeV
  int num = ((threshH - threshL) / step) + 1; //number of plots
  float a = 0.5; //EM contribution
  float b = 1.5; //Had contribution
  float ratio = a / b;
  

  //Initialize the  histogram of efficiencies
  TEfficiency *eff[num]; 
  TH1* total = new TH1F ("h1", "events", 200, 0, 200000);
  TH1D* triggered[num];
  
  // filling the TH1D array and naming the histograms
  int k = 0;
  for(int v = threshL; v <= threshH; v += step) {
    //TString threshVal = toStringI(v);
    TString histName = "triggered_" + toStringI(v) + "_GeV;" + "Ratio:" + toStringF(ratio);
    triggered[k] = new TH1D(histName, histName, 200, 0, 200000);
    k++;
  }
  
  
  
  //Chain the files
  TChain chain("mytree");
   chain.Add("$ROOTSYS/data/data/Threshold/user.ngrzesia.mc15_14TeV_gg_hh125_4b_lambda01_19062017_OUTPUT.141435611/user.ngrzesia.11516841.OUTPUT._00000*.root");
  //chain.Add("$ROOTSYS/data/data/Threshold/user.ngrzesia.signal_thresholded_third_final_OUTPUT.144782079/user.ngrzesia.11619224.OUTPUT._00001*.root");


  //Create a TTreeReader (call it reader) for the chain
  TTreeReader reader(&chain);

  //Read the data
  TTreeReaderValue<Int_t> bunchN(reader, "distFrontBunchTrain");
  TTreeReaderValue<std::vector<float>> triggerEt(reader,"JFexJet_Et");
  TTreeReaderValue<std::vector<float>> triggerEt_EM(reader,"JFexJet_Et_EM");
  TTreeReaderValue<std::vector<float>> triggerEt_Had(reader,"JFexJet_Et_Had");
  TTreeReaderValue<std::vector<float>> triggerEta(reader,"JFexJet_eta");
  TTreeReaderValue<std::vector<float>> triggerPhi(reader,"JFexJet_phi");
  TTreeReaderValue<std::vector<float>> offlinePt(reader,"AntiKt4LCTopoJets_pt");
  TTreeReaderValue<std::vector<float>> offlineEta(reader,"AntiKt4LCTopoJets_eta");
  TTreeReaderValue<std::vector<float>> offlinePhi(reader,"AntiKt4LCTopoJets_phi");
  // TTreeReaderValue<std::vector<float>> offlinePt(reader,"JFexJet_Et");
  //TTreeReaderValue<std::vector<float>> offlineEta(reader,"JFexJet_eta");
  //TTreeReaderValue<std::vector<float>> offlinePhi(reader,"JFexJet_phi");
 
  
  while (reader.Next()) {
    Int_t  n = *bunchN;
    //only consider n>20 events
    if (n <= 20) continue;
    
    //fill with all of the offline events 
    for (int i = 0; i < offlinePt-> size(); i++) {
      if (fabs(offlineEta->at(i)) > 3.2) continue; 
      total->Fill(offlinePt->at(i));
      
      //determine which trigger entries qualify as close enough	to offline
      for (int j = 0; j < triggerEt->size(); j++) {
	float diffEta = fabs(triggerEta->at(j) - offlineEta->at(i));
	float diffPhi = fabs(triggerPhi->at(j) - offlinePhi->at(i));
	//consider phi wrapping
	if (diffPhi >= TMath::Pi()) {
	  diffPhi = (2*TMath::Pi()) - diffPhi;
	}

	//compute the weighted energy
      	float dR = TMath::Sqrt(diffEta*diffEta + diffPhi*diffPhi);
	float triggerWeightedEt = (a * triggerEt_EM->at(j)) + (b * triggerEt_Had->at(j));

	//loop to make multiple graphs over many trigger thresholds
	k = 0;
	for(int v = threshL ; v <= threshH; v += step) {
	  int thresh = v * 1000; //value in eV

	  //condition to pass the triggered events	  
	  if (dR < 0.4 && triggerWeightedEt > thresh) {
	    triggered[k]->Fill(offlinePt->at(i));
	    k++;
	    continue;
	  }
	}
      } // end of j for loop
    }  //end of i for loop
  }// end of the reader

  //Open a file; make TEff plots and write them
  TString fileName = "manyEff;R:" + toStringF(ratio) + ".root"; 
  TFile myFile(fileName,"RECREATE");
  for(int i = 0; i < num; i++) {
    if(TEfficiency::CheckConsistency(*triggered[i],*total)) {

      
      int thr = threshL + (i * step); //in GeV
      TString effName = "jetEff_" + toStringI(thr) + "GeV (Ratio:" + toStringF(ratio) + ")";
      TString effTitle = "L1 Jet Trigger Efficiency for " + toStringI(thr) + " GeV threshold_R:" + toStringF(ratio);
      TCanvas *c1 = new TCanvas;
      eff[i] = new TEfficiency(*triggered[i],*total);
      eff[i]->SetName(effName);
      eff[i]->SetTitle("title;Energy [GeV];Pass/Total");
      eff[i]->SetTitle(effTitle);
      //eff[i]->Write();
      eff[i]->Draw();
     
    }// end of the if consistency check
  }
} 

	
