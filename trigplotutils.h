#ifndef TRIGPLOTUTILS_H
#define TRIGPLOTUTILS_H
#include "TEfficiency.h"
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"

// Fit version
float getThreshold(TEfficiency *turnon, float eff, float initialOffset, float initialWidth){


  float xmin = turnon->GetTotalHistogram()->GetXaxis()->GetXmin();
  float xmax = turnon->GetTotalHistogram()->GetXaxis()->GetXmax();
  //TF1 *fitFunc = new TF1("fitFunc", "0.98*(0.5*TMath::Erf((x-[0])/[1]) + 0.5)", xmin, xmax);
  TF1 *fitFunc = new TF1("fitFunc", "0.5*TMath::Erf((x-[0])/[1]) + 0.5", xmin, xmax);
  fitFunc->SetParName(0, "offset");
  fitFunc->SetParName(1, "width");
  fitFunc->SetParameter(0, initialOffset);
  fitFunc->SetParameter(1, initialWidth);

  //printf("Fitting to %s\n", turnon->GetName());
  turnon->Fit(fitFunc, "Q");

  return fitFunc->GetParameter(1)*TMath::ErfInverse(2*eff-1) + fitFunc->GetParameter(0);

}

// Scanning version
float getThreshold(TEfficiency *turnon, float eff){
  const TH1* total = turnon->GetTotalHistogram();
  int nbins = total->GetNbinsX();
  float result = total->GetBinCenter(nbins);
  for(int i = 1; i < nbins; i++){
    if(turnon->GetEfficiency(i) >= eff && turnon->GetEfficiency(i+1) >= eff){
      result = total->GetBinCenter(i);
      break;
    }
  }

  return result;

}

TEfficiency *initializeTurnOn(const char *name, int index, const char *title){
  char num[8];
  std::string nstr;
  sprintf(num, "%d", index);
  nstr = std::string(num);
  return new TEfficiency(std::string(""+std::string(name)+nstr).c_str(), title, 100, 0, 200);
}

float sumBins(TH1 *hist, int startBin, int nBins){
   float total = 0;
   for(int i=startBin; i<=nBins+1; i++) //include overflow bin
      total += hist->GetBinContent(i);
   return total;
}

TEfficiency *grabPlot(TFile *file, const char *name, int index){
  char num[8];
  std::string nstr; sprintf(num, "%d", index);
  nstr = std::string(num);
  return (TEfficiency*)file->Get(std::string(""+std::string(name)+nstr).c_str());
}

// simple sort by descending value, intended for use with small vectors
std::vector<float> sorted(std::vector<float> const* vec){
  std::vector<float> result;
  if(!vec) return result;
  for(unsigned int i=0; i < vec->size(); i++){
    float thisValue = vec->at(i);
    std::vector<float>::iterator it = result.begin();
    while(true){
      if(it == result.end() || thisValue > *it){
        result.insert(it, thisValue);
        break;
      }
      it++;
    }
  }
  return result;
}

float deltaR(float eta1, float eta2, float phi1, float phi2){
  float a = TMath::Sqrt((eta1-eta2)*(eta1-eta2) + (phi1-phi2)*(phi1-phi2));
  float b;
  if(phi1 > phi2)
    b = TMath::Sqrt((eta1-eta2)*(eta1-eta2) + (phi1-phi2-TMath::TwoPi())*(phi1-phi2-TMath::TwoPi()));
  else
    b = TMath::Sqrt((eta1-eta2)*(eta1-eta2) + (phi1-phi2+TMath::TwoPi())*(phi1-phi2+TMath::TwoPi()));

  return a < b ? a : b;

}

#endif

