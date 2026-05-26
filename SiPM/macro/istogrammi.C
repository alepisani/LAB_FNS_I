#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <TGraph.h>
#include <iostream>
#include "txt_reader.cpp"
#include "letturaFile.cpp"

void istogrammi(){
    // ---------------- dati canale zero -------------------
    vector<vector<double>> dataDC0 = txt_reader("../data/txt/Gruppo6_Ch0_708_40dB_DC_staircase.txt");
    vector<vector<double>> dataCosm0 = txt_reader("../data/txt/Gruppo6_Ch0_708_40dB_Cosm_staircase.txt");
    vector<vector<double>> dataSr0 = txt_reader("../data/txt/Gruppo6_Ch0_708_40dB_Sr90_staircase.txt");
    
    //-------------- dati canale uno --------------------
    vector<vector<double>> dataDC1 = txt_reader("../data/txt/Gruppo6_Ch1_7033_39dB_DC_staircase.txt");
    vector<vector<double>> dataCosm1 = txt_reader("../data/txt/Gruppo6_Ch1_7033_39dB_Cosm_staircase.txt");
    vector<vector<double>> dataSr1 = txt_reader("../data/txt/Gruppo6_Ch1_7033_39dB_Sr90_staircase.txt");

    
    TGraph* g1 = new TGraph(dataDC0[0].size(), dataDC0[0].data(), dataDC0[1].data());

    TGraph* g2 = new TGraph(dataCosm0[0].size(), dataCosm0[0].data(), dataCosm0[1].data());

    TGraph* g3 = new TGraph(dataSr0[0].size(), dataSr0[0].data(), dataSr0[1].data());

    g1->SetLineColor(kRed);
    g2->SetLineColor(kBlue);
    g3->SetLineColor(kGreen);

    TMultiGraph* mg = new TMultiGraph();
    mg->Add(g1, "LP");
    mg->Add(g2, "LP");
    mg->Add(g3, "LP");

    TLegend *leg = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg->SetTextSize(0.03);
    leg->SetHeader("Legenda", "C"); // "C" centra il testo del titolo
    leg->AddEntry(g1, "Dark Count", "l");
    leg->AddEntry(g2, "DC e Cosmici", "l");
    leg->AddEntry(g3, "DC, Cosmici e Sorgente", "l");



    TCanvas* c1 = new TCanvas("c1", "staircase plot", 800, 600);
    c1->SetLogy();
    mg->GetXaxis()->SetTitle("Soglia [mV]");
    mg->GetYaxis()->SetTitle("Conteggi");
    mg->Draw("A");
    leg->Draw();
    //c1->SaveAs("../plots/Ch0_DC.png");
}