#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "letturaFile.cpp"

void coinc(){
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111);
    vector<vector<double>> coinc1 = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/gamma/data/coinc/TAC_-22deg_Na_300s_coin.mca", 12, false);
    vector<vector<double>> coinc2 = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/gamma/data/coinc/TAC_Na_fondo_lungo4h_-35deg.mca", 12, false);
    //vector<vector<double>> coinc3 = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/gamma/data/coinc/spettri_puliti/pulito_TAC_-22deg_Na_300s_coin.mca", 12, false);

    double tempo_fondo = 14409.261000;
    double tempo_spettro= 300.;
    double count_spettro=0.;
    double count_fondo=0.;
    

    double fattore_scala= tempo_spettro/tempo_fondo;

    TH1D* coinc_1 = new TH1D("coinc_1", "Spettro originale", coinc1[0].size(), 0, coinc1[0].size()+1);
    TH1D* coinc_2 = new TH1D("coinc_2", "fondo", coinc2[0].size(), 0, coinc2[0].size()+1);
    TH1D* coinc_3 = new TH1D("coinc_3", "Spettro pulito", coinc1[0].size(), 0, coinc1[0].size()+1);

     for (int i = 0; i < coinc1[0].size(); i++){
        count_spettro+=coinc1[0][i];
        count_fondo+=coinc2[0][i];
        coinc_1->SetBinContent(i+1, coinc1[0][i]);
        coinc_2->SetBinContent(i+1, coinc2[0][i]*fattore_scala);
        coinc_3->SetBinContent(i+1, coinc1[0][i]-coinc2[0][i]*fattore_scala);
    }
    double rate_spettro=count_spettro/tempo_spettro;
    double rate_fondo=count_fondo/tempo_fondo;
    double rate_spettropulito=(count_spettro-count_fondo*fattore_scala)/tempo_spettro;

    cout<<"Rate spettro: "<< rate_spettro<<endl;
    cout<<"Rate fondo: "<< rate_fondo<<endl;
    cout<<"Rate spettro pulito: "<< rate_spettropulito<<endl;




    TCanvas* c1 = new TCanvas("c1", "coinc1", 800, 600);
    
    coinc_1->SetLineColor(kBlue);
    coinc_1->Rebin(2);
    coinc_1->SetXTitle("MCA");
    coinc_1->SetYTitle("Conteggi");
    coinc_1->Draw("HIST");
    coinc_3->SetLineColor(kGreen);
    coinc_3->Rebin(2);
    coinc_3->Draw("HIST SAME");  
    coinc_2->SetLineColor(kRed);
    coinc_2->Rebin(2);
    coinc_2->Draw("HIST SAME");

    TLegend *spettri = new TLegend(0.7,0.7,0.9,0.9);
    spettri->AddEntry(coinc_1, "spettro", "l");
    spettri->AddEntry(coinc_2, "fondo", "l");
    spettri->AddEntry(coinc_3, "spettro pulito", "l");
    spettri->SetBorderSize(0);   // toglie bordo
    spettri->SetFillStyle(0);    // trasparente
    spettri->SetTextSize(0.04);  
    spettri->Draw("SAME");

    
    TCanvas* c2 = new TCanvas("c2", "coinc2", 800, 600);
    coinc_2->SetTitle("Fondo");
    coinc_2->SetLineColor(kRed);
    coinc_2->SetXTitle("MCA");
    coinc_2->SetYTitle("Conteggi");
    coinc_2->Draw("HIST ");

    TCanvas* c3 = new TCanvas("c3", "coinc3", 800, 600);
    coinc_3->Draw("HIST ");
    coinc_3->SetLineColor(kGreen);
    coinc_3->SetXTitle("MCA");
    coinc_3->SetYTitle("Conteggi");







    

}