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

void Spettri(){

    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111);

    vector<vector<double>> CSI_20 = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/SiPM/data/txt/Gruppo6_5472V_137Cs_cryCsI_32dB_thr20_histo.txt", 8, false);
    vector<vector<double>> CSI_50 = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/SiPM/data/txt/Gruppo6_5472V_137Cs_cryCsI_32dB_thr50_histo.txt", 8, false);
    vector<vector<double>> CSI_60 = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/SiPM/data/txt/Gruppo6_5472V_137Cs_cryCsI_32dB_thr60_histo.txt", 8, false);
    vector<vector<double>> LYSO_25 = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/SiPM/data/txt/Gruppo6_5472V_137Cs_cryLYSO_32dB_thr25_histo.txt", 8, false);
    vector<vector<double>> BGO_15 = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/SiPM/data/txt/Gruppo6_5472V_137Cs_cryBGO_32dB_histo.txt", 8, false);

    TH1D* csi_20 = new TH1D("csi_20", "spettro CsI -thr20", 400, -2000, 20000);
    TH1D* csi_50 = new TH1D("csi_50", "spettro CsI -thr50", 250, -2000, 20000);
    TH1D* csi_60 = new TH1D("csi_60", "spettro CsI -thr60", 250, -2000, 20000);
    TH1D* lyso_25 = new TH1D("lyso_25", "spettro LYSO -thr25", 200, -1000, 7000);
    TH1D* bgo_15 = new TH1D("bgo_15", "spettro BGO -thr15", 100, -1000, 3000);

    //Riempiamo gli istogrammi

    for (int i = 0; i < CSI_20[0].size(); i++){
        csi_20->Fill(-CSI_20[0][i],CSI_20[1][i]);
    }

    for (int i = 0; i < CSI_50[0].size(); i++){
        csi_50->Fill(-CSI_50[0][i],CSI_50[1][i]);
    }
    for (int i = 0; i < CSI_60[0].size(); i++){
        csi_60->Fill(-CSI_60[0][i],CSI_60[1][i]);
    }
    for (int i = 0; i < LYSO_25[0].size(); i++){
        lyso_25->Fill(-LYSO_25[0][i], LYSO_25[1][i]);
    }
    for (int i = 0; i < BGO_15[0].size(); i++){
        bgo_15->Fill(-BGO_15[0][i], BGO_15[1][i]);
    }
    

    TCanvas* c1 = new TCanvas("c1", "spettro CsI -thr20", 800, 600);
    csi_20->Draw("HIST");
    csi_20->SetLineColor(kBlue);
    csi_20->SetLineWidth(3);
    csi_20->SetXTitle("ADC [Ch]");
    csi_20->SetYTitle("Conteggi");

    TCanvas* c2 = new TCanvas("c2", "spettro CsI -thr50", 800, 600);
    csi_50->Draw("HIST");
    csi_50->SetLineColor(kBlue);
    csi_50->SetLineWidth(3);
    csi_50->SetXTitle("ADC [Ch]");
    csi_50->SetYTitle("Conteggi");

    TCanvas* c3 = new TCanvas("c3", "spettro CsI -thr60", 800, 600);
    csi_60->Draw("HIST");
    csi_60->SetLineColor(kBlue);
    csi_60->SetLineWidth(3);
    csi_60->SetXTitle("ADC [Ch]");
    csi_60->SetYTitle("Conteggi");

    TCanvas* c4 = new TCanvas("c4", "spettro BGO -thr15", 800, 600);
    bgo_15->Draw("HIST");
    bgo_15->SetLineColor(kBlue);
    bgo_15->SetLineWidth(3);
    bgo_15->SetXTitle("ADC [Ch]");
    bgo_15->SetYTitle("Conteggi");

    TCanvas* c5 = new TCanvas("c5", "spettro LYSO -thr25", 800, 600);
    lyso_25->Draw("HIST");
    lyso_25->SetLineColor(kBlue);
    lyso_25->SetLineWidth(3);
    lyso_25->SetXTitle("ADC [Ch]");
    lyso_25->SetYTitle("Conteggi");

    /*
    TCanvas* c6 = new TCanvas("c5", "Spettri con ^{137}Cs", 800, 600);
    csi_50->Draw("HIST");
    csi_50->SetXTitle("ADC [Ch]");
    csi_50->SetYTitle("Conteggi");
    csi_50->GetXaxis()->SetRangeUser(-1500, 17000); 
    csi_50->SetMinimum(0);                  
    csi_50->SetMaximum(23000);
    csi_50->SetLineColor(kRed);
    csi_50->SetLineWidth(3);
    csi_50->SetTitle("Spettri con ^{137}Cs");
    bgo_15->Draw("HIST SAME");
    bgo_15->SetLineColor(kGreen+2);
    bgo_15->SetLineWidth(3);
    lyso_25->Draw("HIST SAME");
    lyso_25->SetLineColor(kBlue);
    lyso_25->SetLineWidth(3);

    TLegend *spettri = new TLegend(0.7,0.7,0.9,0.9);
    spettri->AddEntry(csi_50, "CsI ", "l");
    spettri->AddEntry(bgo_15, "BGO", "l");
    spettri->AddEntry(lyso_25, "LYSO", "l");

    spettri->SetBorderSize(1);   // toglie bordo
    spettri->SetFillStyle(0);    // trasparente
    spettri->Draw();

    */
    

    




}