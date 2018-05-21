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




void manyEff_Weighted() {

  //Changables - modify thresholds and ratios
  int threshL = 10; //lower trigger threshold bound in GeV
  int threshH = 14; //higher trigger threshold bound in GeV
  int step = 1; //energy step in GeV
  float aStart = 1.1;
  float aFinish = 1.1;
  float bStart = 0.9;
  float bFinish = 0.9;
  float rStep = 0.1; 
  
  
  
  //Relevant quantities
  int num = ((threshH - threshL) / step) + 1; //number of TEff plots for each ratio
  float weightRatio; 
  float a; //EM contribution
  float b; //Had contribution
  int numRatios = TMath::Nint(((aFinish - aStart) / rStep) + 1);

  //Initialize the  histogram of efficiencies
  TEfficiency *eff[num]; 
  TH1* total = new TH1F ("h1", "events", 200, 0, 200000);
  TH1D* triggered[num];
    
  //FOR LOOP OVER MANY RATIOS
  for(int nR = 0; nR < numRatios; nR++) {
    a = aStart + (nR * rStep); //current EM contribution
    b = bStart - (nR * rStep); //current Had contribution
    weightRatio = a / b;
  

  
   
  
    // filling the TH1D array and naming the histograms
    int k = 0;
    for(int v = threshL; v <= threshH; v += step) {
      //TString threshVal = toStringI(v);
      TString histName = "triggered_" + toStringI(v) + "_GeV;" + "Ratio:" + toStringF(weightRatio);
      triggered[k] = new TH1D(histName, histName, 200, 0, 200000);
      k++;
    }
  
  
  
    //Chain the files
    TChain chain("mytree");
    //   chain.Add("$ROOTSYS/data/data/Threshold/user.ngrzesia.signal_thresholded_third_final_OUTPUT.144782079/user.ngrzesia.11619224.OUTPUT._0000*.root");
    chain.Add("$ROOTSYS/data/data/Threshold/user.ngrzesia.mc15_14TeV_gg_hh125_4b_lambda01_19062017_OUTPUT.141435611/user.ngrzesia.*");

    //Create a TTreeReader (call it reader) for the chain
    TTreeReader reader(&chain);

    //Read the data
    TTreeReaderValue<Int_t> bunchN(reader, "distFrontBunchTrain");
    TTreeReaderValue<std::vector<float>> triggerEt(reader,"JFexJet_Et");
    TTreeReaderValue<std::vector<float>> triggerEt_EM(reader,"JFexJet_Et_EM");
    TTreeReaderValue<std::vector<float>> triggerEt_Had(reader,"JFexJet_Et_Had");
    TTreeReaderValue<std::vector<float>> triggerEta(reader,"JFexJet_eta");
    TTreeReaderValue<std::vector<float>> triggerPhi(reader,"JFexJet_phi");
    //TTreeReaderValue<std::vector<float>> offlinePt(reader,"CalibratedAntiKt4LCTopoJets_pt");
    //TTreeReaderValue<std::vector<float>> offlineEta(reader,"CalibratedAntiKt4LCTopoJets_eta");
    //TTreeReaderValue<std::vector<float>> offlinePhi(reader,"CalibratedAntiKt4LCTopoJets_phi");
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
	if (nR == 0) {
	  total->Fill(offlinePt->at(i));
	}
	//determine which trigger entries qualify as close enough to offline
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

	  if(dR > 0.4) continue;
	  else {
	    j = triggerEt->size();
	    //loop to make multiple graphs over many trigger thresholds
	    k = 0;
	    for(int v = threshL ; v <= threshH; v += step) {
	      int thresh = v * 1000; //value in MeV

	      //condition to pass the triggered events	  
	      if (triggerWeightedEt > thresh) {
		triggered[k]->Fill(offlinePt->at(i));
		k++;
	      }
	    }
	  }
	} // end of j for loop
      }  //end of i for loop
    }// end of the reader
    
    //Open a file; make TEff plots and write them
    TString fileName = "manyEffR" + toStringF(weightRatio) + ".root"; 
    //TFile myFile(fileName,"RECREATE");
    for(int i = 0; i < num; i++) {
      if(TEfficiency::CheckConsistency(*triggered[i],*total)) {

      
	int thr = threshL + (i * step); //in GeV
	TString effName = "jetEff_" + toStringI(thr) + "GeV (Ratio:" + toStringF(weightRatio) + ")";
	TString effTitle = "L1 Jet Trigger Efficiency for " + toStringI(thr) + " GeV threshold_R:" + toStringF(weightRatio);
	TCanvas *c1 = new TCanvas;
	eff[i] = new TEfficiency(*triggered[i],*total);
	eff[i]->SetName(effName);
	eff[i]->SetTitle("title;Energy [GeV];Pass/Total");
	eff[i]->SetTitle(effTitle);
	eff[i]->Draw();
	//eff[i]->Write();

	delete eff[i];
	eff[i] = 0;
	delete triggered[i];
	triggered[i] = 0;
      }// end of the if consistency check
    }
  }
} 

	
