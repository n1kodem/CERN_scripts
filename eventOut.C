#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "Math/LorentzVector.h"
#include <TLorentzVector.h>
#include "TStopwatch.h"
#include "TCanvas.h"



#include <vector>
#include <iostream>
void eventOut() {

  int c = 0;
  int k = 0;
  //Chain the files
  TChain chain("mytree");
  chain.Add("$ROOTSYS/data/data/Threshold/myOutputFile.root");

  //Create a TTreeReader (call it reader) for the chain
  TTreeReader reader(&chain);
  //Read the data
  TTreeReaderValue<Int_t> bunchN(reader, "distFrontBunchTrain");
  TTreeReaderValue<std::vector<float>> triggerEt(reader,"JFexJet_Et");
  TTreeReaderValue<std::vector<float>> triggerEt_EM(reader,"JFexJet_Et_EM");
  TTreeReaderValue<std::vector<float>> triggerEt_Had(reader,"JFexJet_Et_Had");

  while (reader.Next()) {
    k++;
    Int_t  n = *bunchN;
    if (n <= 20) continue;
    if (c < 1000) {
      for (int i = 0; i < triggerEt->size(); i++) {
	printf("total ET: %f;  Et EM: %f ; Et Had: %f.; event # %i \n",triggerEt->at(i),triggerEt_EM->at(i),triggerEt_Had->at(i), k);

	c++;
      }

    }
  }
}
