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


    // selezione bin
    bool use_pe_bins = true;

    // lettura file, tutti della stessa lunghezza
    vector<vector<double>> data_24 = doubleReader("../data/txt/708V_30dB_24ua_histo.txt", 8, false);
    vector<vector<double>> data_27 = doubleReader("../data/txt/708V_30dB_27ua_histo.txt", 8, false);
    vector<vector<double>> data_30 = doubleReader("../data/txt/708V_30dB_30ua_histo.txt", 8, false);


    // parametri delle distribuzioni, per convertire
    // LED a 2.4
    int dpp_24 = 331;
    int x0_24 = 4; //offset del picco a 0

    // LED a 2.7
    int dpp_27 = 334;
    int x0_27 = 5;   //offset del picco a 0

    // LED a 3.0
    int dpp_30 = 336;
    int x0_30 = 5; //offset del picco a 0

    //parametri istogrammi (N in p.e., x in ch)
    int Npe = 15;       // numero di p.e. nell'istogramma
    double Nlow = -0.5;
    double Nup = Npe + 0.5;
    int nbins = -999;
    double xlow24, xup24;
    double xlow27, xup27;
    double xlow30, xup30;
    if(use_pe_bins) nbins = Npe + 1;
    else {
        int offset = 90;   // taglio sulla parte sinistra dell'istogramma
        nbins = 720;
        //Led 2.4
        xlow24 = (data_24[0][0]-4. + 8*offset);
        xup24 = xlow24 + (8*nbins);
        //Led 2.7
        xlow27 = (data_27[0][0]-4. + 8*offset);
        xup27 = xlow27 + (8*nbins);
        //Led 3.0
        xlow30 = (data_30[0][0]-4. + 8*offset);
        xup30 = xlow30 + (8*nbins);
        
    }


    // range dei fit
    double xmin_24, xmax_24;
    double xmin_27, xmax_27;
    double xmin_30, xmax_30;
    if(use_pe_bins){
        gStyle->SetOptFit(1111);
        xmin_24 = 0.5, xmax_24 = 10.5;
        xmin_27 = 0.5, xmax_27 = 10.5;
        xmin_30 = 0.5, xmax_30 = 10.5;
    } else {
        gStyle->SetOptFit(0);
        xmin_24 = -0.4, xmax_24 = 8.5;
        xmin_27 = -0.4, xmax_27 = 10.5;
        xmin_30 = -0.4, xmax_30 = 14;
    }

    // dichiarazione istogrammi

    TH1D *h24, *h27, *h30;

    if(use_pe_bins){
        h24 = new TH1D("h24", "", nbins, Nlow, Nup);
        h27 = new TH1D("h27", "", nbins, Nlow, Nup);
        h30 = new TH1D("h30", "", nbins, Nlow, Nup);

        // fill in pe
        for(int i=0; i<data_24[1].size(); i++){
        h24->Fill( (data_24[0][i]-x0_24)/dpp_24 , data_24[1][i]);
        h27->Fill( (data_27[0][i]-x0_27)/dpp_27 , data_27[1][i]);
        h30->Fill( (data_30[0][i]-x0_30)/dpp_30 , data_30[1][i]);
    }

    } else {
        h24 = new TH1D("h24", "", nbins, xlow24, xup24);
        h27 = new TH1D("h27", "", nbins, xlow27, xup27);
        h30 = new TH1D("h30", "", nbins, xlow30, xup30);
        
        // fill i ch, poi conversione in p.e.
        for(int i=0; i<data_24[1].size(); i++){
            h24->Fill( (data_24[0][i]) , data_24[1][i]);
            h27->Fill( (data_27[0][i]) , data_27[1][i]);
            h30->Fill( (data_30[0][i]), data_30[1][i]);
        }
        //conversione asse x in p.e., con offset dell'x0
        h24->GetXaxis()->Set(nbins, (xlow24-x0_24)/dpp_24, (xup24-x0_24)/dpp_24);
        h27->GetXaxis()->Set(nbins, (xlow27-x0_27)/dpp_27, (xup27-x0_27)/dpp_27);
        h30->GetXaxis()->Set(nbins, (xlow30-x0_30)/dpp_30, (xup30-x0_30)/dpp_30);
        
    }

    //if( (data_24[1].size()!=data_27[1].size()) || (data_24[1].size()!=data_30[1].size()) || (data_30[1].size()!=data_27[1].size()) ){
      //  cout << "File con lunghezza diversa! Errore!\n";
      //  return;
    //}

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
        poisson24->SetParameters(1e5, 1);

        poisson27 = new TF1("poisson27", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1)", xmin_27, xmax_27);
        poisson27->SetParameters(1e5,3);

        poisson30 = new TF1("poisson30", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1)", xmin_30, xmax_30);
        poisson30->SetParameters(1e5, 5);
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
        double par30[26] = {7000, 5,
                            5000, 0, 0.03,
                            12000, 1, 0.03,
                            15000, 2, 0.03,
                            17000, 3, 0.03,
                            15000, 4, 0.03,
                            12000, 5, 0.03,
                            10000, 6, 0.03,
                            7000, 7, 0.03};
        poisson30->SetParameters(par30);
    }

    // creazione canvas
    TCanvas* c24 = new TCanvas("c24", "c24", 640,480);
    c24->cd(1);
    c24->SetTicks();
    h24->Draw("hist");

    h24->Fit(poisson24, "WLR", "", xmin_24, xmax_24);
    poisson24->Draw("same");
    


    TCanvas* c27 = new TCanvas("c27", "c27", 640,480);
    c27->cd(1);
    c27->SetTicks();
    h27->Draw("hist");

    h27->Fit(poisson27, "WLR", "", xmin_27, xmax_27);
    poisson27->Draw("same");
    


    TCanvas* c30 = new TCanvas("c30", "c30", 640,480);
    c30->cd(1);
    c30->SetTicks();
    h30->Draw("hist");

    h30->Fit(poisson30, "WLR", "", xmin_30, xmax_30);
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
