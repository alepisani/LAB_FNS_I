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

// macro per l'analisi degli spettri di poisson di un SiPM con un LED a tre diverse
// intensità, 2.4, 2.7 e 3.7 unità arbitrarie (ua)

void spettri_poisson(){

    //gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111);

    // lettura file, tutti della stessa lunghezza
    vector<vector<double>> data_24 = doubleReader("../data/txt/708V_30dB_24ua_histo.txt", 8, false);
    vector<vector<double>> data_27 = doubleReader("../data/txt/708V_30dB_27ua_histo.txt", 8, false);
    vector<vector<double>> data_30 = doubleReader("../data/txt/708V_30dB_30ua_pp_histo.txt", 8, false);

    int dpp = 340;  //valore delta picco-picco, pari
    int Npe = 20;   //numero di fotoni equivalenti nell'isotgramma
    int x0 = 5;     // posizione picco 0 p.e., ossia il rumore
    int x1 = x0 + dpp; // 1 fotone equivalente

    //parametri istogrammi (in p.e.)

    double Nlow = -0.5;
    double Nup = Npe + 0.5;
    int nbins = Npe + 1;

    /*
    int xlow = x0 - dpp/2;
    int xup = x0 + Npe*dpp + dpp/2;
    int nbins = Npe + 1;
    */

    // dichiarazione istogrammi

    TH1D* h24 = new TH1D("h24", "", nbins, Nlow, Nup);
    TH1D* h27 = new TH1D("h27", "", nbins, Nlow, Nup);
    TH1D* h30 = new TH1D("h30", "", nbins, Nlow, Nup);

    if( (data_24[1].size()!=data_27[1].size()) || (data_24[1].size()!=data_30[1].size()) || (data_30[1].size()!=data_27[1].size()) ){
        cout << "File con lunghezza diversa! Errore!\n";
        return;
    }

    for(int i=0; i<data_24[1].size(); i++){
        h24->Fill(data_24[0][i]/x1, data_24[1][i]);
        h27->Fill(data_27[0][i]/x1, data_27[1][i]);
        h30->Fill(data_30[0][i]/x1, data_30[1][i]);
    }

/*
    h24->FillN(data_24[0].size(), data_24[0].data()/x1, data_24[1].data());
    h27->FillN(data_27[0].size(), data_27[0].data()/x1, data_27[1].data());
    h30->FillN(data_30[0].size(), data_30[0].data()/x1, data_30[1].data());
    */

    // gestione histo

    h24->SetTitle("Spettro poissoniano a 2.4 ua");
    h24->GetXaxis()->SetTitle("Numero di fotoni equivalenti");
    h24->GetYaxis()->SetTitle("conteggi / p.e.");

    h27->SetTitle("Spettro poissoniano a 2.7 ua");
    h27->GetXaxis()->SetTitle("Numero di fotoni equivalenti");
    h27->GetYaxis()->SetTitle("conteggi / p.e.");

    h30->SetTitle("Spettro poissoniano a 3.0 ua");
    h30->GetXaxis()->SetTitle("Numero di fotoni equivalenti");
    h30->GetYaxis()->SetTitle("conteggi / p.e.");

    // dichiarazione TF1
    /*TF1* poisson24 = new TF1("poisson24", [](double *x, double *par){
        int k = (int)round(x[0]);
        return par[0] * TMath::Poisson(k, par[1]) * TMath::Gaus(x[0], k, par[2], true);
    }, 0, Nup, 3);*/

    //TF1* poisson24 = new TF1("poisson24", "[0] * TMath::Poisson(TMath::Nint(x), [1])* TMath::Gaus(x, TMath::Nint(x), [2], true)", 0, Nup);
    //poisson24->SetParameters(5e5, 1, 0.3);

    TF1* poisson24 = new TF1("poisson24", "[0] * TMath::Poisson(TMath::Nint(x), [1])", 0, Nup);
    poisson24->SetParameters(1.5e5, 1);

    TF1* poisson27 = new TF1("poisson27", "[0] * TMath::Poisson(TMath::Nint(x), [1])", 0, Nup);
    poisson27->SetParameters(1.5e5, 1);

    TF1* poisson30 = new TF1("poisson30", "[0] * TMath::Poisson(TMath::Nint(x), [1])", 0, Nup);
    poisson30->SetParameters(1e6, 4);


    // creazione canvas
    TCanvas* c24 = new TCanvas("c24", "c24", 640,480);
    c24->cd(1);
    c24->SetTicks();
    h24->Fit(poisson24, "R", "", 0.5, 5.5);

    h24->Draw();
    c24->Print("../plots/LED/fit_poisson_24.pdf");


    TCanvas* c27 = new TCanvas("c27", "c27", 640,480);
    c27->cd(1);
    c27->SetTicks();
    h27->Fit(poisson27, "R", "", 0.5, 6.5);

    h27->Draw();
    c27->Print("../plots/LED/fit_poisson_27.pdf");


    TCanvas* c30 = new TCanvas("c30", "c30", 640,480);
    c30->cd(1);
    c30->SetTicks();
    h30->Fit(poisson30, "R", "", 0.5, 7.5);

    h30->Draw();
    c30->Print("../plots/LED/fit_poisson_30.pdf");


        





}