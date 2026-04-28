#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"

void landau_gauss(){

    //TFile *input_file = TFile::Open("../data/ADC3_singola_HV1980_THR40_100000.root");
    TFile *input_file = TFile::Open("../data/ADC4_singola_HV2070_THR106_5000.root");

    //TH1D* hist = (TH1D*)input_file->Get("adc3");
    TH1D* hist = (TH1D*)input_file->Get("adc4");
    hist->Rebin(3);

    

    TF1* langauss = new TF1("langauss", "landau(0) + gaus(3)", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
    TF1* landau = new TF1("landau", "landau(0)", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
    TF1* gauss = new TF1("gauss", "gaus(3)", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
    TF1* solo_landau = new TF1("solo_landau", "landau", 74, 400);

    langauss->SetParameter(0, hist->GetMaximum());
    langauss->SetParameter(1, hist->GetMean());
    langauss->SetParameter(2, hist->GetRMS());
    langauss->SetParameter(3, hist->GetMaximum());
    langauss->SetParameter(4, hist->GetMean());
    langauss->SetParameter(5, hist->GetRMS());

    /*TCanvas *c_tot = new TCanvas("c1", "Landau + Gauss", 800, 600);
    hist->Draw();
    hist->Fit(langauss, "R"); 

    TCanvas *c_sum = new TCanvas("c2", "landau e gauss", 800, 600);
    hist->Draw();
    hist->Fit(landau, "R");
    hist->Fit(gauss, "R");*/

    //-------------- fit della sola landau ---------------

    TCanvas *c_landau = new TCanvas("c3", "solo landau", 800, 600);
    hist->Draw();
    hist->Fit(solo_landau, "R");
    gStyle->SetOptFit(1111);
    cout << "Chi^2:" << solo_landau->GetChisquare() << ", number of DoF: " << solo_landau->GetNDF() << " (Probability: " << solo_landau->GetProb() << ")." << endl;




}