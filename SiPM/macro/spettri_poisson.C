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
    gStyle->SetOptFit(0);

    // selezione bin
    bool use_pe_bins = false;
    bool use_binomial = true;

    // lettura file, tutti della stessa lunghezza
    vector<vector<double>> data_24 = doubleReader("../data/txt/708V_30dB_24ua_histo.txt", 8, false);
    vector<vector<double>> data_27 = doubleReader("../data/txt/708V_30dB_27ua_histo.txt", 8, false);
    vector<vector<double>> data_30 = doubleReader("../data/txt/708V_30dB_30ua_histo.txt", 8, false);

    int dpp = 335;  //valore delta picco-picco, pari
    int Npe = 15;   //numero di fotoni equivalenti nell'istogramma
    int x0 = 0;     // posizione picco 0 p.e., ossia il rumore
    int x1 = x0 + dpp; // 1 fotone equivalente

    //parametri istogrammi (in p.e.)

    double Nlow = -0.3;
    double Nup = Npe + 0.5;
    int nbins = -999;
    double xlow, xup;
    if(use_pe_bins) nbins = Npe + 1;
    else {
        //auto limit = upper_bound(data_24[0].begin(), data_24[0].end(), Npe*x1);
        //int index = distance(data_24[0].begin(), limit);
        int offset = 90;
        nbins = 720;
        xlow = (data_24[0][0]-4. + 8*offset)/x1;
        xup = (xlow*x1 + 8*nbins + 4.)/x1;
        
    }


    // range dei fit
    double xmin_24 = 0, xmax_24 = 8.5;
    double xmin_27 = 0, xmax_27 = 10.5;
    double xmin_30 = 0.5, xmax_30 = 14;


    // dichiarazione istogrammi

    TH1D *h24, *h27, *h30;

    if(use_pe_bins){
        h24 = new TH1D("h24", "", nbins, Nlow, Nup);
        h27 = new TH1D("h27", "", nbins, Nlow, Nup);
        h30 = new TH1D("h30", "", nbins, Nlow, Nup);
    } else {
        h24 = new TH1D("h24", "", nbins, xlow, xup);
        h27 = new TH1D("h27", "", nbins, xlow, xup);
        h30 = new TH1D("h30", "", nbins, xlow, xup);
    }

    //if( (data_24[1].size()!=data_27[1].size()) || (data_24[1].size()!=data_30[1].size()) || (data_30[1].size()!=data_27[1].size()) ){
      //  cout << "File con lunghezza diversa! Errore!\n";
      //  return;
    //}

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
    }, 0, Nup, 3);*/    //TF1* poisson24 = new TF1("poisson24", "[0] * TMath::Poisson(TMath::Nint(x), [1])* TMath::Gaus(x, TMath::Nint(x), [2], true)", 0, Nup);
    //poisson24->SetParameters(5e5, 1, 0.3);

    //[0] * TMath::Poisson(TMath::Nint(x), [1])

    // dichiarazione funzioni
    TF1 *poisson24, *poisson27, *poisson30;

    if(use_pe_bins){
        poisson24 = new TF1("poisson24", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1)", xmin_24, xmax_24);
        poisson24->SetParameters(1.5e5, 1);

        poisson27 = new TF1("poisson27", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1)", xmin_27, xmax_27);
        poisson27->SetParameters(1.5e5,3);

        poisson30 = new TF1("poisson30", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1)", xmin_30, xmax_30);
        poisson30->SetParameters(1e3, 5);
    } 
    else if (use_binomial){
        poisson24 = new TF1("poisson24", "TMath::Binomial([0], TMath::Nint(x))*TMath::Power([1],x)*TMath::Power(1-[1], [0]-x)", xmin_24, xmax_24);
        poisson27 = new TF1("poisson27", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1)", xmin_27, xmax_27);
        poisson27->SetParameters(1.5e5,3);

        poisson30 = new TF1("poisson30", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1)", xmin_30, xmax_30);
        poisson30->SetParameters(1e3, 5);
    }
    
    
    else {

        // fit delle gaussiane singole

        // 2.4
        /*
        int ngaus24 = 5;         // da cambiare in base a quante gaussiane ci sono nella TF1 finale
        const int npar24 = 17; //2 + 3*npar24
        double par24[npar24];
        //inizializzazione parametri poissoniana
        par24[0] = 2400;
        par24[1] = 3.5;  
        // trova parametri gaussiane
        for(int i=0; i<ngaus24; i++){
            TF1 gaus_sing("gaus_sing", "gaus", i-0.15, i+0.15);
            h24->Fit(&gaus_sing);
            gaus_sing.GetParameters(&par24[2+3*i]);
        }
            */

        poisson24 = new TF1("poisson24", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1) + gaus(2) + gaus(5) + gaus(8) + gaus(11) + gaus(14) + gaus(17)", xmin_24, xmax_24);
        double par24[20] = {2400, 3.5,
                        13000, 0, 0.01,
                        12000, 1, 0.01, 
                        12000, 2, 0.02, 
                        8000, 3, 0.03, 
                        4000, 4, 0.03,
                        1000, 5, 0.1};
        poisson24->SetParameters(par24);

        poisson27 = new TF1("poisson27", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1) + gaus(2) + gaus(5) + gaus(8) + gaus(11) + gaus(14) + gaus(17) + gaus(20)", xmin_27, xmax_27);
        double par27[23] = {2000, 4,
                        13000, 0, 0.01,
                        12000, 1, 0.01, 
                        11000, 2.2, 0.02, 
                        10000, 3, 0.03, 
                        10000, 4, 0.03,
                        800, 5.1, 0.08,
                        500, 5.9, 0.2};//,
                        //500, 7, 0.1};
        poisson27->SetParameters(par27);

        poisson30 = new TF1("poisson30", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1) + gaus(2) + gaus(5) + gaus(8) + gaus(11) + gaus(14) + gaus(17) + gaus(20) + gaus(23)", xmin_30, xmax_30);
        /*double par30[26] = {7000, 5,
                            5000, 0, 0.03,
                            10000, 1, 0.03,



        }*/
        //poisson30->SetParameters(1e6, 4, 0.3);
    }

    // creazione canvas
    TCanvas* c24 = new TCanvas("c24", "c24", 640,480);
    c24->cd(1);
    c24->SetTicks();
    h24->Draw("hist");

    h24->Fit(poisson24, "R", "", xmin_24, xmax_24);
    poisson24->Draw("same");
    


    TCanvas* c27 = new TCanvas("c27", "c27", 640,480);
    c27->cd(1);
    c27->SetTicks();
    h27->Draw("hist");

    h27->Fit(poisson27, "R", "", xmin_27, xmax_27);
    poisson27->Draw("same");
    


    TCanvas* c30 = new TCanvas("c30", "c30", 640,480);
    c30->cd(1);
    c30->SetTicks();
    h30->Draw("hist");

    h30->Fit(poisson30, "R", "", xmin_30, xmax_30);
    poisson30->Draw("same");
    


    if(!use_pe_bins){
        c24->Print("../plots/LED/fit_poisson_24.pdf");
        c27->Print("../plots/LED/fit_poisson_27.pdf");
        c30->Print("../plots/LED/fit_poisson_30.pdf");
    } else {
        c24->Print("../plots/LED/rebinned_fit_poisson_24.pdf");
        c27->Print("../plots/LED/rebinned_fit_poisson_27.pdf");
        c30->Print("../plots/LED/rebinned_fit_poisson_30.pdf");
    }

        





}
