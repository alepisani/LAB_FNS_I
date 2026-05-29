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


void tac(){
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111);

    vector<vector<double>> tac_01 = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/gamma/data/TAC/TAC_+0deg_Na_300s_coin_0.1us.mca", 12, false);
    vector<vector<double>> tac_03 = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/gamma/data/TAC/TAC_+0deg_Na_300s_coin_0.3us.mca", 12, false);
    vector<vector<double>> tac_05 = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/gamma/data/TAC/TAC_+0deg_Na_300s_coin_0.5us.mca", 12, false);
    vector<vector<double>> tac_07 = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/gamma/data/TAC/TAC_+0deg_Na_300s_coin_0.7us.mca", 12, false);
    vector<vector<double>> tac_09 = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/gamma/data/TAC/TAC_+0deg_Na_300s_coin_0.9us.mca", 12, false);
    vector<vector<double>> tac_11 = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/gamma/data/TAC/TAC_+0deg_Na_300s_coin_1.1us.mca", 12, false);

    TH1D* TAC_01 = new TH1D("TAC_01", "TAC con ritardo 0.1 ua", tac_01[0].size(), 0, tac_01[0].size()+1);
    TH1D* TAC_03 = new TH1D("TAC_01", "TAC con ritardo 0.3 ua", tac_01[0].size(), 0, tac_01[0].size()+1);
    TH1D* TAC_05 = new TH1D("TAC_01", "TAC con ritardo 0.5 ua", tac_01[0].size(), 0, tac_01[0].size()+1);
    TH1D* TAC_07 = new TH1D("TAC_01", "TAC con ritardo 0.7 ua", tac_01[0].size(), 0, tac_01[0].size()+1);
    TH1D* TAC_09 = new TH1D("TAC_01", "TAC con ritardo 0.9 ua", tac_01[0].size(), 0, tac_01[0].size()+1);
    TH1D* TAC_11 = new TH1D("TAC_01", "TAC con ritardo 1.1 ua", tac_01[0].size(), 0, tac_01[0].size()+1);

    /*
    cout<< "01: " << tac_01[0].size() << endl;
    cout<< "03: " << tac_03[0].size() << endl;
    cout<< "05: " << tac_05[0].size() << endl;
    cout<< "07: " << tac_07[0].size() << endl;
    cout<< "09: " << tac_09[0].size() << endl;
    cout<< "11: " << tac_11[0].size() << endl;
    */


    for (int i = 0; i < tac_01[0].size(); i++){
       TAC_01->SetBinContent(i+1, tac_01[0][i]);
       TAC_03->SetBinContent(i+1, tac_03[0][i]);
       TAC_05->SetBinContent(i+1, tac_05[0][i]);
       TAC_07->SetBinContent(i+1, tac_07[0][i]);
       TAC_09->SetBinContent(i+1, tac_09[0][i]);
       TAC_11->SetBinContent(i+1, tac_11[0][i]);

    }

    /*
    //--------------------------------- SPETTRI TAC SOVRAPPOSTI ------------------------------
    TCanvas* c1 = new TCanvas("c1", "TAC con ritardo 0.1 #mu s", 800, 600);
    TAC_01->Draw("HIST");
    TAC_01->SetLineColor(kBlue);
    TAC_01->SetLineWidth(3);
    TAC_01->SetXTitle("MCA [Ch]");
    TAC_01->SetYTitle("Conteggi");
    TAC_01->SetMaximum(100);
    TAC_01->SetTitle("Distribuzioni TAC");

    TAC_03->Draw("HIST SAME");
    TAC_03->SetLineColor(kRed);
    TAC_03->SetLineWidth(3);

    TAC_05->Draw("HIST SAME");
    TAC_05->SetLineColor(kGreen+2);
    TAC_05->SetLineWidth(3);

    TAC_07->Draw("HIST SAME");
    TAC_07->SetLineColor(kMagenta);
    TAC_07->SetLineWidth(3);

    TAC_09->Draw("HIST SAME");
    TAC_09->SetLineColor(kOrange+1);
    TAC_09->SetLineWidth(3);

    TAC_11->Draw("HIST SAME");
    TAC_11->SetLineColor(kCyan+2);
    TAC_11->SetLineWidth(3);


    TLegend *ritardi = new TLegend(0.7,0.65,0.9,0.9,"Ritardi");
    ritardi->AddEntry(TAC_01, "0.1 #mu s ", "l");
    ritardi->AddEntry(TAC_03, "0.3 #mu s", "l");
    ritardi->AddEntry(TAC_05, "0.5 #mu s", "l");
    ritardi->AddEntry(TAC_07, "0.7 #mu s", "l");
    ritardi->AddEntry(TAC_09, "0.9 #mu s", "l");
    ritardi->AddEntry(TAC_11, "1.1 #mu s", "l");
    ritardi->SetBorderSize(0);   // toglie bordo
    ritardi->SetFillStyle(0);    // trasparente
    ritardi->SetHeader("Ritardi", "C");  // C = centrato
    ritardi->SetTextSize(0.04);  
    ritardi->Draw("SAME");

*/
    //---------------------------------------------------------------FIT GAUSSIANI ------------------------

    TF1* gauss01 = new TF1("gauss01", "gaus(0) + pol2(3)", 280, 365);
    gauss01->SetLineColor(kRed);
    gauss01->SetParameter(0,74);
    gauss01->SetParameter(1,312);
    gauss01->SetParameter(2,13);

    TCanvas* c2 = new TCanvas("c2", "TAC con ritardo 0.1 ua -fit", 800, 600);
    TAC_01->Draw("HIST");
    TAC_01->Rebin(2);
    TAC_01->SetLineColor(kBlue);
    TAC_01->SetLineWidth(3);
    TAC_01->SetXTitle("MCA [Ch]");
    TAC_01->SetYTitle("Conteggi");
    gStyle->SetOptFit(1111);
    TAC_01->Fit(gauss01, "R 0");
    TAC_01->GetXaxis()->SetRangeUser(200, 500);
    gauss01->Draw("SAME");
    gauss01->SetLineWidth(3);
    double t1 = gauss01->GetParameter(1);
    

    TF1* gauss03 = new TF1("gauss03", "gaus(0) + pol2(3)", 480, 560);
    gauss03->SetLineColor(kRed);
    gauss03->SetParameter(0,70);
    gauss03->SetParameter(1,520);
    gauss03->SetParameter(2,13);

    TCanvas* c3 = new TCanvas("c3", "TAC con ritardo 0.3 ua -fit", 800, 600);
    TAC_03->Draw("HIST");
    TAC_03->Rebin(2);
    TAC_03->SetLineColor(kBlue);
    TAC_03->SetLineWidth(3);
    TAC_03->SetXTitle("MCA [Ch]");
    TAC_03->SetYTitle("Conteggi");
    gStyle->SetOptFit(1111);
    TAC_03->Fit(gauss03, "R 0");
    TAC_03->GetXaxis()->SetRangeUser(400, 700);
    gauss03->Draw("SAME");
    gauss03->SetLineWidth(3);
    double t3 = gauss03->GetParameter(1);
    

    TF1* gauss05 = new TF1("gauss05", "gaus(0) + pol2(3)", 660, 760);
    gauss05->SetLineColor(kRed);
    gauss05->SetParameter(0,70);
    gauss05->SetParameter(1,700);
    gauss05->SetParameter(2,13);

    TCanvas* c4 = new TCanvas("c4", "TAC con ritardo 0.5 ua -fit", 800, 600);
    TAC_05->Draw("HIST");
    TAC_05->Rebin(2);
    TAC_05->SetLineColor(kBlue);
    TAC_05->SetLineWidth(3);
    TAC_05->SetXTitle("MCA [Ch]");
    TAC_05->SetYTitle("Conteggi");
    gStyle->SetOptFit(1111);
    TAC_05->Fit(gauss05, "R 0");
    TAC_05->GetXaxis()->SetRangeUser(600, 900);
    gauss05->Draw("SAME");
    gauss05->SetLineWidth(3);
    double t5 = gauss05->GetParameter(1);
    

    TF1* gauss07 = new TF1("gauss07", "gaus(0) + pol2(3)", 850, 950);
    gauss07->SetLineColor(kRed);
    gauss07->SetParameter(0,0);
    gauss07->SetParameter(1,900);
    gauss07->SetParameter(2,13);

    TCanvas* c5 = new TCanvas("c5", "TAC con ritardo 0.7 ua -fit", 800, 600);
    TAC_07->Draw("HIST");
    TAC_07->Rebin(2);
    TAC_07->SetLineColor(kBlue);
    TAC_07->SetLineWidth(3);
    TAC_07->SetXTitle("MCA [Ch]");
    TAC_07->SetYTitle("Conteggi");
    gStyle->SetOptFit(1111);
    TAC_07->Fit(gauss07, "R 0");
    TAC_07->GetXaxis()->SetRangeUser(800, 1050);
    gauss07->Draw("SAME");
    gauss07->SetLineWidth(3);
    double t7 = gauss07->GetParameter(1);
    

    TF1* gauss09 = new TF1("gauss09", "gaus(0) + pol2(3)", 1060, 1130);
    gauss09->SetLineColor(kRed);
    gauss09->SetParameter(0,71);
    gauss09->SetParameter(1,1086);
    gauss09->SetParameter(2,13);
    

    TCanvas* c6 = new TCanvas("c6", "TAC con ritardo 0.9 ua -fit", 800, 600);
    TAC_09->Draw("HIST");
    TAC_09->Rebin(2);
    TAC_09->SetLineColor(kBlue);
    TAC_09->SetLineWidth(3);
    TAC_09->SetXTitle("MCA [Ch]");
    TAC_09->SetYTitle("Conteggi");
    gStyle->SetOptFit(1111);
    TAC_09->Fit(gauss09, "R 0");
    TAC_09->GetXaxis()->SetRangeUser(1000, 1300);
    gauss09->Draw("SAME");
    gauss09->SetLineWidth(3);
    double t9 = gauss09->GetParameter(1);
    

    TF1* gauss11 = new TF1("gauss11", "gaus(0) + pol2(3)", 1255, 1320);
    gauss11->SetLineColor(kRed);
    gauss11->SetParameter(0,260);
    gauss11->SetParameter(1,1270);
    gauss11->SetParameter(2,20);

    TCanvas* c7 = new TCanvas("c7", "TAC con ritardo 1.1 ua -fit", 800, 600);
    TAC_11->Draw("HIST");
    TAC_11->Rebin(2);
    TAC_11->SetLineColor(kBlue);
    TAC_11->SetLineWidth(3);
    TAC_11->SetXTitle("MCA [Ch]");
    TAC_11->SetYTitle("Conteggi");
    gStyle->SetOptFit(1111);
    TAC_11->Fit(gauss11, "R 0");
    TAC_11->GetXaxis()->SetRangeUser(1200, 1500);
    gauss11->Draw("SAME");
    gauss11->SetLineWidth(3);
    double t11 = gauss11->GetParameter(1);
    
/*
    int n=6;
    double ritardi[] = {0.1, 0.3, 0.5, 0.7, 0.9, 1.1}; //\mus
    double MCA[] = {t1,t3,t5,t7,t9,t11};
    double s_ritardi[] = {0, 0, 0, 0, 0, 0};
    double s_MCA[] = {17.6, 17.41, 17.61, 17.19, 17.36, 16.85};


    gStyle->SetOptFit(1111);
    TCanvas* tac = new TCanvas("tac", "Calibrazione TAC", 800, 600);
    TGraphErrors *cal_tac = new TGraphErrors(n, ritardi, MCA, s_ritardi, s_MCA);
    cal_tac->SetTitle("Calibrazione TAC");
    cal_tac->GetXaxis()->SetTitle("Ritardi [ua]");
    cal_tac->GetYaxis()->SetTitle("MCA [Ch]");

    cal_tac->SetMarkerStyle(20);
    cal_tac->SetMarkerColor(kBlue);
    cal_tac->SetLineColor(kBlue);   
    
    TF1 *fit_lineare = new TF1("fit_lineare", "pol1", 0, 1000);
    cal_tac->Fit(fit_lineare, "R");
    tac->SetGrid();
    cal_tac->Draw("AP");
    

*/



}