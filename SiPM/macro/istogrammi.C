#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <TGraph.h>
#include <iostream>
#include "txt_reader.cpp"

void istogrammi(){
    vector<vector<double>> data = txt_reader("../data/txt/Gruppo6_Ch0_708_40dB_DC_staircase.txt");
    TH1D* hist = new TH1D("hist", "staircase plot", 100, 0, 205);

    for (int i = 0; i < 195; i++){
        hist->Fill(data[0][i], data[1][i]);
    }

    TCanvas* c1 = new TCanvas("c1", "staircase plot", 800, 600);
    hist->Draw();
    c1->SetLogy();
    c1->SetXTitle("Soglia [mV]");
    c1->SetYTitle("Rate [kHz]");
    //c1->SaveAs("../plots/Ch0_DC.png");
}