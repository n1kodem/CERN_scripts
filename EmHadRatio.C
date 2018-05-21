#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"
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




void EmHadRatio() {

  //Initialize the 2D histogram
  TH2F *hTwo1 = new TH2F("h1", "EM/total energy vs. PT", 200, 0.0, 200000.0, 101, 0.0, 1.01);
  TH2F *hTwo2 = new TH2F("h2", "Had/total energy vs. PT", 200, 0.0, 200000.0, 101, 0.0, 1.01);

  int c = 0;
  int d = 0;

  
  //Chain the files
  TChain chain("mytree");
  chain.Add("$ROOTSYS/data/data/user.ngrzesia.hh125_4b_pos_thresholds_OUTPUT.143802793/user.ngrzesia.11584734.OUTPUT._0000*");

  //Create a TTreeReader (call it reader) for the chain
  TTreeReader reader(&chain);

    //Read the data
    TTreeReaderValue<Int_t> bunchN(reader, "distFrontBunchTrain");
    TTreeReaderValue<std::vector<float>> triggerEt(reader,"JFexJet_Et");
    TTreeReaderValue<std::vector<float>> triggerEt_EM(reader,"JFexJet_Et_EM");
    TTreeReaderValue<std::vector<float>> triggerEt_Had(reader,"JFexJet_Et_Had");

 
    //1. Retreive Em, Had and compute their ratio as well as their total energy.
    //2. Plot the ratio as a funcion of energy

    
    while (reader.Next()) {
      Int_t  n = *bunchN;
      //only consider n>20 events
      if (n <= 20) continue;
      for (int i = 0; i < triggerEt-> size(); i++) {
	float totalEnergy = triggerEt_EM->at(i) + triggerEt_Had->at(i);
	if ( totalEnergy > 0) {
	  //	continue;
	}
	
	  if ( triggerEt_EM->at(i) < 0 && c < 100) {

	    c++;
	    cout << "EM: " << triggerEt_EM->at(i) << "  Had: "<< triggerEt_Had->at(i)<< endl;
	  }

	  if ( triggerEt_Had->at(i) < 0 && d < 100) {

	    d++;
	    cout << "EM: " << triggerEt_EM->at(i) << "  Had: "<< triggerEt_Had->at(i)<< endl;
	  }
	
	float ratioEm = triggerEt_EM->at(i) / totalEnergy;
	float ratioHad = triggerEt_Had->at(i) / totalEnergy;

	
	hTwo1->Fill(totalEnergy,ratioEm);
	hTwo2->Fill(totalEnergy,ratioHad);

      }
    }// end of the reader
    TCanvas *c1 = new TCanvas();
    c1->SetLogz();
    hTwo1->Draw("COLZ");

    TCanvas *c2 = new TCanvas();
    c2->SetLogz();
    hTwo2->Draw("COLZ");
    
    
    //Open a file; make 2D Histogram plot and write it
    //TString fileName = "EmHadRatio.root"; 
    //TFile myFile(fileName,"RECREATE");
  
}
