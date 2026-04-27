#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"

void landau_gauss(){

    TFile *input_file = TFile::Open("../data/ADC3_singola_HV1980_THR40_100000.root");

    TH1D* hist = (TH1D*)input_file->Get("adc3");
    hist->Rebin(4);

    TF1* langauss = new TF1("langauss", "landau(0) + gaus(3)", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
    TF1* landau = new TF1("landau", "landau(0)", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
    TF1* gauss = new TF1("gauss", "gauss(3)", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());

    langauss->SetParameter(0, hist->GetMaximum());
    langauss->SetParameter(1, hist->GetMean());
    langauss->SetParameter(2, hist->GetRMS());
    langauss->SetParameter(3, hist->GetMaximum());
    langauss->SetParameter(4, hist->GetMean());
    langauss->SetParameter(5, hist->GetRMS());

    TCanvas *c_tot = new TCanvas("c", "Landau + Gauss", 800, 600);
    hist->Draw();
    hist->Fit(langauss, "R"); 

    TCanvas *c_sum = new TCanvas("c", "landau e gauss", 800, 600);
    hist->Draw();
    hist->Fit(landau, "R");
    hist->Fit(gauss, "R");

}