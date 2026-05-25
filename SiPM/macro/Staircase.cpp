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

void Staircase(){
     //gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111);

    // lettura file, tutti della stessa lunghezza
    //staircase senza sorgente

    double Nlow = 0.;
    double Nup = 180.;
    int nbins = 180;
    
    vector<vector<double>> data_CSI = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/SiPM/data/txt/Gruppo6_5472V_DC_cryCsI_32dB_staircase.txt", 8, false);
    vector<vector<double>> data_CSI_s = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/SiPM/data/txt/Gruppo6_5472V_DC_cryCsI_32dB_Cs137_staircase.txt", 8, false);
    vector<vector<double>> data_DC = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/SiPM/data/txt/Gruppo6_5472V_DC_empty_32dB_staircase.txt", 8, false);
    vector<vector<double>> data_BGO_s = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/SiPM/data/txt/Gruppo6_5472V_137Cs_cryBGO_32dB_staircase.txt", 8, false);
    vector<vector<double>> data_LYSO_s = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/SiPM/data/txt/Gruppo6_5472V_137Cs_cryLYSO_32dB_staircase.txt", 8, false);
    
   
    TGraph *hcsi = new TGraph(data_CSI[0].size(), data_CSI[0].data(), data_CSI[1].data());
    TGraph *hcsi_s = new TGraph(data_CSI_s[0].size(), data_CSI_s[0].data(), data_CSI_s[1].data());
    TGraph *hdc = new TGraph(data_DC[0].size(), data_DC[0].data(), data_DC[1].data());
    TGraph *hbgo_s = new TGraph(data_BGO_s[0].size(), data_BGO_s[0].data(), data_BGO_s[1].data());
    TGraph *hlyso_s = new TGraph(data_LYSO_s[0].size(), data_LYSO_s[0].data(), data_LYSO_s[1].data());
    
    

    hdc->SetTitle("Staircase Dark Count");
    hdc->GetXaxis()->SetTitle("Soglia [mV]");
    hdc->GetYaxis()->SetTitle("Frequenza [Hz]");

    hcsi_s->SetTitle("Staircase CsI");
    hcsi_s->GetXaxis()->SetTitle("Soglia [mV]");
    hcsi_s->GetYaxis()->SetTitle("Frequenza [Hz]");

    hbgo_s->SetTitle("Staircase BGO");
    hbgo_s->GetXaxis()->SetTitle("Soglia [mV]");
    hbgo_s->GetYaxis()->SetTitle("Frequenza [Hz]");

    hlyso_s->SetTitle("Staircase LYSO");
    hlyso_s->GetXaxis()->SetTitle("Soglia [mV]");
    hlyso_s->GetYaxis()->SetTitle("Frequenza [Hz]");
    
    // creazione canvas
    TCanvas* dc = new TCanvas("dc", "dc", 640,480);
    dc->SetLogy();
    hdc->Draw("AL");
    hdc->SetLineColor(kBlue+1);
    hdc->SetLineWidth(4);

    TCanvas* csi = new TCanvas("csi", "csi", 640,480);
    csi->SetLogy();
    hcsi_s->Draw("AL");
    hdc->Draw("L SAME");
    hcsi->Draw("L SAME");
    hcsi->SetLineColor(kRed);
    hcsi->SetLineWidth(4);
    hdc->SetLineColor(kBlue);
    hdc->SetLineWidth(4);
    hcsi_s->SetLineColor(kGreen+2);
    hcsi_s->SetLineWidth(4);

    TLegend *lcsi = new TLegend(0.7,0.7,0.9,0.9);
    lcsi->AddEntry(hcsi_s, "^{137}Cs ", "l");
    lcsi->AddEntry(hdc, "Dark Count", "l");
    lcsi->AddEntry(hcsi, "Senza sorgente", "l");

    lcsi->SetBorderSize(1);   // toglie bordo
    lcsi->SetFillStyle(0);    // trasparente
    lcsi->Draw();
    

    TCanvas* bgo = new TCanvas("bgo", "bgo", 640,480);
    bgo->SetLogy();
    hbgo_s->Draw("AL");
    hdc->Draw("L SAME");
    hbgo_s->SetLineColor(kGreen+2);
    hbgo_s->SetLineWidth(4);
    //hdc->SetLineColor(kViolet);
    //hdc->SetLineWidth(4);

    TLegend *lbgo = new TLegend(0.7,0.7,0.9,0.9);
    lbgo->AddEntry(hbgo_s, "^{137}Cs ", "l");
    lbgo->AddEntry(hdc, "Dark Count", "l");

    lbgo->SetBorderSize(1);   // toglie bordo
    lbgo->SetFillStyle(0);    // trasparente
    lbgo->Draw();


    TCanvas* lyso = new TCanvas("lyso", "lyso", 640,480);
    lyso->SetLogy();
    hlyso_s->Draw("AL");
    hdc->Draw("L SAME");
    hlyso_s->SetLineColor(kGreen+2);
    hlyso_s->SetLineWidth(4);
    //hdc->SetLineColor(kCyan);
    //hdc->SetLineWidth(4);

    TLegend *llyso = new TLegend(0.7,0.7,0.9,0.9);
    llyso->AddEntry(hlyso_s, "^{137}Cs ", "l");
    llyso->AddEntry(hdc, "Dark Count", "l");

    llyso->SetBorderSize(1);   // toglie bordo
    llyso->SetFillStyle(0);    // trasparente
    llyso->Draw();


}