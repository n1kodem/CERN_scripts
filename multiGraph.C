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


void multiGraph() {

  //Changables - modify thresholds and ratios
  float aStart = 0.5;
  float aFinish = 1.5;
  float bStart = 1.5;
  float bFinish = 0.5;
  float rStep = 0.1; 
  

  float weightRatio; 
  float a; //EM contribution
  float b; //Had contribution

  //Open and point to the rate graph
  TFile *comboPlots = new TFile("$ROOTSYS/data/data/Threshold/comboPlotsMA7.root");


  //Graph1
  a = aStart; b = bStart; weightRatio = a / b;
  TString comboPlotName = "comboPlotR" + toStringF(weightRatio);
  TGraph* comboPlot1 = (TGraph*)comboPlots->Get(comboPlotName);
  comboPlot1->SetName("comboPlotR" + toStringF(weightRatio));
  comboPlot1->SetTitle("EM/Had Ratio: " + toStringF(weightRatio));
 

  //Graph2
  a = aStart + (1 * rStep); b = bStart - (1 * rStep); weightRatio = a / b;
  comboPlotName = "comboPlotR" + toStringF(weightRatio);
  TGraph* comboPlot2 = (TGraph*)comboPlots->Get(comboPlotName);
  comboPlot2->SetName("comboPlotR" + toStringF(weightRatio));
  comboPlot2->SetTitle("EM/Had Ratio: " + toStringF(weightRatio));

  //Graph3
  a = aStart + (2 * rStep); b = bStart - (2 * rStep); weightRatio = a / b;
  comboPlotName = "comboPlotR" + toStringF(weightRatio);
  TGraph* comboPlot3 = (TGraph*)comboPlots->Get(comboPlotName);
  comboPlot3->SetName("comboPlotR" + toStringF(weightRatio));
  comboPlot3->SetTitle("EM/Had Ratio: " + toStringF(weightRatio));

  //Graph4
  a = aStart + (3 * rStep); b = bStart - (3 * rStep); weightRatio = a / b;
  comboPlotName = "comboPlotR" + toStringF(weightRatio);
  TGraph* comboPlot4 = (TGraph*)comboPlots->Get(comboPlotName);
  comboPlot4->SetName("comboPlotR" + toStringF(weightRatio));
  comboPlot4->SetTitle("EM/Had Ratio: " + toStringF(weightRatio));
 

  //Graph5
  a = aStart + (4 * rStep); b = bStart - (4 * rStep); weightRatio = a / b;
  comboPlotName = "comboPlotR" + toStringF(weightRatio);
  TGraph* comboPlot5 = (TGraph*)comboPlots->Get(comboPlotName);
  comboPlot5->SetName("comboPlotR" + toStringF(weightRatio));
  comboPlot5->SetTitle("EM/Had Ratio: " + toStringF(weightRatio));

  //Graph6
  a = aStart + (5 * rStep); b = bStart - (5 * rStep); weightRatio = a / b;
  comboPlotName = "comboPlotR" + toStringF(weightRatio);
  TGraph* comboPlot6 = (TGraph*)comboPlots->Get(comboPlotName);
  comboPlot6->SetName("comboPlotR" + toStringF(weightRatio));
  comboPlot6->SetTitle("EM/Had Ratio: " + toStringF(weightRatio));

  //Graph7
  a = aStart + (6 * rStep); b = bStart - (6 * rStep); weightRatio = a / b;
  comboPlotName = "comboPlotR" + toStringF(weightRatio);
  TGraph* comboPlot7 = (TGraph*)comboPlots->Get(comboPlotName);
  comboPlot7->SetName("comboPlotR" + toStringF(weightRatio));
  comboPlot7->SetTitle("EM/Had Ratio: " + toStringF(weightRatio));

  //Graph8
  a = aStart + (7 * rStep); b = bStart - (7 * rStep); weightRatio = a / b;
  comboPlotName = "comboPlotR" + toStringF(weightRatio);
  TGraph* comboPlot8 = (TGraph*)comboPlots->Get(comboPlotName);
  comboPlot8->SetName("comboPlotR" + toStringF(weightRatio));
  comboPlot8->SetTitle("EM/Had Ratio: " + toStringF(weightRatio));
 

  //Graph9
  a = aStart + (8 * rStep); b = bStart - (8 * rStep); weightRatio = a / b;
  comboPlotName = "comboPlotR" + toStringF(weightRatio);
  TGraph* comboPlot9 = (TGraph*)comboPlots->Get(comboPlotName);
  comboPlot9->SetName("comboPlotR" + toStringF(weightRatio));
  comboPlot9->SetTitle("EM/Had Ratio: " + toStringF(weightRatio));

  //Graph10
  a = aStart + (9 * rStep); b = bStart - (9 * rStep); weightRatio = a / b;
  comboPlotName = "comboPlotR" + toStringF(weightRatio);
  TGraph* comboPlot10 = (TGraph*)comboPlots->Get(comboPlotName);
  comboPlot10->SetName("comboPlotR" + toStringF(weightRatio));
  comboPlot10->SetTitle("EM/Had Ratio: " + toStringF(weightRatio));

  //Graph11
  a = aStart + (10 * rStep); b = bStart - (10 * rStep); weightRatio = a / b;
  comboPlotName = "comboPlotR" + toStringF(weightRatio);
  TGraph* comboPlot11 = (TGraph*)comboPlots->Get(comboPlotName);
  comboPlot11->SetName("comboPlotR" + toStringF(weightRatio));
  comboPlot11->SetTitle("EM/Had Ratio: " + toStringF(weightRatio));

    
    
  /*
  TLatex *latex1 = new TLatex(comboPlot1->GetX()[0], comboPlot1->GetY()[0], "10GeV trigger threshold");
    latex1->SetTextSize(0.03);
    latex1->SetTextColor(kBlue);
    comboPlot1->GetListOfFunctions()->Add(latex1);
  */  

  TCanvas *c = new TCanvas("c","c", 1500, 1000);
  c->SetLogy();
  TMultiGraph *mg = new TMultiGraph("mg","mg");
  comboPlot1->SetMarkerStyle(2);
  comboPlot1->SetDrawOption("P*");
  comboPlot1->SetMarkerColor(1);
  comboPlot1->SetLineWidth(1);
  comboPlot1->SetFillStyle(0);

  comboPlot2->SetMarkerStyle(3);
  comboPlot2->SetDrawOption("P");
  comboPlot2->SetMarkerColor(2);
  comboPlot2->SetLineWidth(1);
  comboPlot2->SetFillStyle(0);

  comboPlot3->SetMarkerStyle(5);
  comboPlot3->SetDrawOption("P");
  comboPlot3->SetMarkerColor(3);
  comboPlot3->SetLineWidth(1);
  comboPlot3->SetFillStyle(0);

  comboPlot4->SetMarkerStyle(20);
  comboPlot4->SetDrawOption("P");
  comboPlot4->SetMarkerColor(4);
  comboPlot4->SetLineWidth(1);
  comboPlot4->SetFillStyle(0);

  comboPlot5->SetMarkerStyle(21);
  comboPlot5->SetDrawOption("P*");
  comboPlot5->SetMarkerColor(3);
  comboPlot5->SetLineWidth(1);
  comboPlot5->SetFillStyle(0);

  comboPlot6->SetMarkerStyle(22);
  comboPlot6->SetDrawOption("P");
  comboPlot6->SetMarkerColor(5);
  comboPlot6->SetLineWidth(1);
  comboPlot6->SetFillStyle(0);

  comboPlot7->SetMarkerStyle(23);
  comboPlot7->SetDrawOption("P");
  comboPlot7->SetMarkerColor(6);
  comboPlot7->SetLineWidth(1);
  comboPlot7->SetFillStyle(0);

  comboPlot8->SetMarkerStyle(27);
  comboPlot8->SetDrawOption("P");
  comboPlot8->SetMarkerColor(3);
  comboPlot8->SetLineWidth(1);
  comboPlot8->SetFillStyle(0);

  comboPlot9->SetMarkerStyle(28);
  comboPlot9->SetDrawOption("P*");
  comboPlot9->SetMarkerColor(7);
  comboPlot9->SetLineWidth(1);
  comboPlot9->SetFillStyle(0);

  comboPlot10->SetMarkerStyle(29);
  comboPlot10->SetDrawOption("P");
  comboPlot10->SetMarkerColor(8);
  comboPlot10->SetLineWidth(1);
  comboPlot10->SetFillStyle(0);

  comboPlot11->SetMarkerStyle(30);
  comboPlot11->SetDrawOption("P*");
  comboPlot11->SetMarkerColor(9);
  comboPlot11->SetLineWidth(1);
  comboPlot11->SetFillStyle(0);

  mg->Add( comboPlot1 );
  mg->Add( comboPlot2 );
  mg->Add( comboPlot3 );
  mg->Add( comboPlot4 );
  mg->Add( comboPlot5 );
  mg->Add( comboPlot6 );
  mg->Add( comboPlot7 );
  mg->Add( comboPlot8 );
  mg->Add( comboPlot9 );
  mg->Add( comboPlot10 );
  mg->Add( comboPlot11 );
  
  mg->Draw("APL");
  mg->SetTitle("Rate for 95% trigger 'turn on' as a function of offline energy");
  mg->GetXaxis()->SetTitle("Offline Et [MeV]");
  mg->GetYaxis()->SetTitle("Rate [Hz]");

  
  c->BuildLegend();
  c->Print("multigraphleg.gif");

    
 
}


    



   




    



  

	
