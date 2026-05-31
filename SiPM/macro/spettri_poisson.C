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
    gStyle->SetOptStat("mr");
    gStyle->SetMarkerStyle(20);
    gStyle->SetMarkerSize(1);
    gStyle->SetMarkerColor(4);
    


    // selezione bin
    bool use_pe_bins = false;
    //selezione fit
    bool fit_binomiale = false;

    // lettura file, tutti della stessa lunghezza
    vector<vector<double>> data_24 = doubleReader("../data/txt/708V_30dB_24ua_histo.txt", 8, false);
    vector<vector<double>> data_27 = doubleReader("../data/txt/708V_30dB_27ua_histo.txt", 8, false);
    vector<vector<double>> data_30 = doubleReader("../data/txt/708V_30dB_30ua_histo.txt", 8, false);


    // parametri delle distribuzioni, per convertire
    // LED a 2.4
    int dpp_24 = 331;
    double x0_24 = 4.2; //offset del picco a 0

    // LED a 2.7
    int dpp_27 = 334;
    double x0_27 = 5;   //offset del picco a 0

    // LED a 3.0
    int dpp_30 = 336;
    double x0_30 = 4.6; //offset del picco a 0

    // le sigma del picco a zero
    double sigma0[] = {29./dpp_24, 28.7/dpp_27, 28.6/dpp_30};

    //parametri istogrammi (N in p.e., x in ch)
    int Npe = 18;       // numero di p.e. nell'istogramma
    double Nlow = -0.5;
    double Nup = Npe + 0.5;
    int nbins = -999;
    double xlow24, xup24;
    double xlow27, xup27;
    double xlow30, xup30;
    if(use_pe_bins) nbins = Npe + 1;
    else {
        int offset = 90;   // taglio sulla parte sinistra dell'istogramma
        nbins = 800;
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


    // range dei fit, in p.e.
    double xmin_24, xmax_24;
    double xmin_27, xmax_27;
    double xmin_30, xmax_30;
    if(use_pe_bins){
        gStyle->SetOptFit(1111);
        xmin_24 = 0.5, xmax_24 = 3;
        xmin_27 = 0.5, xmax_27 = 5;
        xmin_30 = 0.5, xmax_30 = 6;
    } else {
        gStyle->SetOptFit(1111);
        xmin_24 = -0.4, xmax_24 = 8.5;
        xmin_27 = -0.4, xmax_27 = 10.5;
        xmin_30 = -0.4, xmax_30 = 14;
    }

    // dichiarazione istogrammi e parametri

    TH1D *h24, *h27, *h30;
    double mean[3], e_mean[3], mean_ZP[3], e_mean_ZP[3];
    double A0[3], Atot[3], e_A0[3], e_Atot[3];
    double y0[3], e_y0[3];
    double intensities[] = {2.4, 2.7, 3.0};



    if(use_pe_bins){
        h24 = new TH1D("h24", "", nbins, Nlow, Nup);
        h27 = new TH1D("h27", "", nbins, Nlow, Nup);
        h30 = new TH1D("h30", "", nbins, Nlow, Nup);

        h24->SetLineWidth(3);
        h27->SetLineWidth(3);
        h30->SetLineWidth(3);

        // fill in pe
        for(int i=0; i<data_24[1].size(); i++){
            h24->Fill( (data_24[0][i]-x0_24)/dpp_24 , data_24[1][i]);
            h27->Fill( (data_27[0][i]-x0_27)/dpp_27 , data_27[1][i]);
            h30->Fill( (data_30[0][i]-x0_30)/dpp_30 , data_30[1][i]);
        }
        // range
        h24->GetYaxis()->SetRange(50e3);
        h27->GetYaxis()->SetRange(45e3);
        h30->GetYaxis()->SetRange(90e3);

        // parametri della distribuzione
        // 2.4
        mean[0] = h24->GetMean();
        e_mean[0] = h24->GetMeanError();
        A0[0] = h24->IntegralAndError(1, 1, e_A0[0]);
        Atot[0] = h24->IntegralAndError(1, Npe+1, e_Atot[0]);
        //2.8
        mean[1] = h27->GetMean();
        e_mean[1] = h27->GetMeanError();
        A0[1] = h27->IntegralAndError(1, 1, e_A0[1]);
        Atot[1] = h27->IntegralAndError(1, Npe+1, e_Atot[1]);
        //3.0
        mean[2] = h30->GetMean();
        e_mean[2] = h30->GetMeanError();
        A0[2] = h30->IntegralAndError(1, 1, e_A0[2]);
        Atot[2] = h30->IntegralAndError(1, Npe+1, e_Atot[2]);

        for(int i=0; i<3; i++){
            mean_ZP[i] = -TMath::Log(A0[i]/Atot[i]);
            e_mean_ZP[i] = TMath::Sqrt( pow(e_A0[i]/A0[i], 2) + pow(e_Atot[i]/Atot[i], 2) );

            cout << intensities[i] << endl;
            cout << "mean = " << mean[i] << " +- " << e_mean[i] << endl;
            cout << "A0 = " << A0[i] << " +- " << e_A0[i] << ", Atot = " << Atot[i] << " +- " << e_Atot[i]<< endl;
            cout << "mean ZP = " << mean_ZP[i] << " +- " << e_mean_ZP[i] << endl << endl;
        }

    } else {
        h24 = new TH1D("h24", "", nbins, xlow24, xup24);
        h27 = new TH1D("h27", "", nbins, xlow27, xup27);
        h30 = new TH1D("h30", "", nbins, xlow30, xup30);
        
        // fill i ch, poi conversione in p.e.
        for(int i=0; i<data_24[1].size(); i++){
            h24->Fill( (data_24[0][i]) , data_24[1][i]);
            h27->Fill( (data_27[0][i]) , data_27[1][i]);
            h30->Fill( (data_30[0][i]) , data_30[1][i]);
        }
        //conversione asse x in p.e., con offset dell'x0
        h24->GetXaxis()->Set(nbins, (xlow24-x0_24)/dpp_24, (xup24-x0_24)/dpp_24);
        h27->GetXaxis()->Set(nbins, (xlow27-x0_27)/dpp_27, (xup27-x0_27)/dpp_27);
        h30->GetXaxis()->Set(nbins, (xlow30-x0_30)/dpp_30, (xup30-x0_30)/dpp_30);

        //reset statistiche per averle in p.e.
        h24->ResetStats();
        h27->ResetStats();
        h30->ResetStats();

 
        // parametri della distribuzione
        // 2.4
        mean[0] = h24->GetMean();
        e_mean[0] = h24->GetMeanError();
        y0[0] = h24->IntegralAndError(h24->FindBin(0), h24->FindBin(0), e_y0[0]);
        Atot[0] = h24->IntegralAndError(1, nbins, e_Atot[0]);
        //2.8
        mean[1] = h27->GetMean();
        e_mean[1] = h27->GetMeanError();
        y0[1] = h27->IntegralAndError(h27->FindBin(0), h27->FindBin(0), e_y0[1]);
        Atot[1] = h27->IntegralAndError(1, nbins, e_Atot[1]);
        //3.0
        mean[2] = h30->GetMean();
        e_mean[2] = h30->GetMeanError();
        y0[2] = h30->IntegralAndError(h30->FindBin(0), h30->FindBin(0), e_y0[2]);
        Atot[2] = h30->IntegralAndError(1, nbins, e_Atot[2]);

        double width[] = {8./dpp_24, 8./dpp_27, 8./dpp_30}; // larghezza del bin, per normalizzare A0

        for(int i=0; i<3; i++){
            double factor = TMath::Sqrt(2*TMath::Pi())*sigma0[i]/width[i];  //fattore di conversione y->A
            A0[i] = factor*y0[i];
            e_A0[i] = factor*e_y0[i];
 
            mean_ZP[i] = -TMath::Log(A0[i]/Atot[i]);
            e_mean_ZP[i] = TMath::Sqrt( pow(e_A0[i]/A0[i], 2) + pow(e_Atot[i]/Atot[i], 2) );

            cout << intensities[i] << endl;
            cout << "mean = " << mean[i] << " +- " << e_mean[i] << endl;
            cout << "ymax = " << y0[i] << endl;
            cout << "A0 = " << A0[i] << ", Atot = " << Atot[i] << endl;
            cout << "mean ZP = " << mean_ZP[i] << " +- " << e_mean_ZP[i] << endl << endl;
        }
        
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
        
        /*    // fit poisson + gaussiane

        poisson24 = new TF1("poisson24", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1) + gaus(2) + gaus(5) + gaus(8) + gaus(11) + gaus(14) + gaus(17) + gaus(20)", xmin_24, xmax_24);
        double par24[23] = {1000, 3.5,
                        13000, 0, 0.01,
                        12000, 1, 0.01, 
                        12000, 2, 0.02, 
                        1500, 3, 0.03, 
                        800, 4, 0.03,
                        500, 5, 0.1,
                        100, 6, 0.1};
        poisson24->SetParameters(par24);
        const char* parNames24[] = {"B_{0}", "#lambda",
                            "A_{0}", "#mu_{0}", "#sigma_{0}",
                            "A_{1}", "#mu_{1}", "#sigma_{1}",
                            "A_{2}", "#mu_{2}", "#sigma_{2}",
                            "A_{3}", "#mu_{3}", "#sigma_{3}",
                            "A_{4}", "#mu_{4}", "#sigma_{4}",
                            "A_{5}", "#mu_{5}", "#sigma_{5}",
                            "A_{6}", "#mu_{6}", "#sigma_{6}"
                        };
        for(int i=0; i<23; i++) poisson24->SetParName(i, parNames24[i]);   // CAMBIARE IL NUMERO IN BASE AL N. PARAMETRI
        

        poisson27 = new TF1("poisson27", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1) + gaus(2) + gaus(5) + gaus(8) + gaus(11) + gaus(14) + gaus(17) + gaus(20)", xmin_27, xmax_27);
        double par27[23] = {1500, 4,
                        13000, 0, 0.01,
                        13000, 1.05, 0.03, 
                        11000, 2, 0.02, 
                        10000, 3, 0.03, 
                        10000, 4, 0.03,
                        5000, 5, 0.05,
                        3000, 6, 0.1};//,
                        //500, 7, 0.1};
        poisson27->SetParameters(par27);
        const char* parNames27[] = {"B_{0}", "#lambda",
                            "A_{0}", "#mu_{0}", "#sigma_{0}",
                            "A_{1}", "#mu_{1}", "#sigma_{1}",
                            "A_{2}", "#mu_{2}", "#sigma_{2}",
                            "A_{3}", "#mu_{3}", "#sigma_{3}",
                            "A_{4}", "#mu_{4}", "#sigma_{4}",
                            "A_{5}", "#mu_{5}", "#sigma_{5}",
                            "A_{6}", "#mu_{6}", "#sigma_{6}"
                        };
        for(int i=0; i<23; i++) poisson27->SetParName(i, parNames27[i]);

        poisson30 = new TF1("poisson30", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1) + gaus(2) + gaus(5) + gaus(8) + gaus(11) + gaus(14) + gaus(17) + gaus(20) + gaus(23)", xmin_30, xmax_30);
        double par30[26] = {1500, 5,
                            5000, 0, 0.03,
                            12000, 1, 0.03,
                            15000, 2, 0.03,
                            17000, 3, 0.03,
                            15000, 4, 0.03,
                            12000, 5, 0.03,
                            6000, 6, 0.05,
                            2000, 7, 0.12};
        poisson30->SetParameters(par30);
        const char* parNames30[] = {"B_{0}", "#lambda",
                            "A_{0}", "#mu_{0}", "#sigma_{0}",
                            "A_{1}", "#mu_{1}", "#sigma_{1}",
                            "A_{2}", "#mu_{2}", "#sigma_{2}",
                            "A_{3}", "#mu_{3}", "#sigma_{3}",
                            "A_{4}", "#mu_{4}", "#sigma_{4}",
                            "A_{5}", "#mu_{5}", "#sigma_{5}",
                            "A_{6}", "#mu_{6}", "#sigma_{6}",
                            "A_{7}", "#mu_{7}", "#sigma_{7}"
                        };
        for(int i=0; i<26; i++) poisson30->SetParName(i, parNames30[i]);
        */

        // fit multipicco
                poisson24 = new TF1("poisson24", "gaus(0) + gaus(3) + gaus(6) + gaus(9) + gaus(12) + gaus(15) + gaus(18)", xmin_24, xmax_24);
        double par24[21] = {
                        5000, 0, 0.0005,
                        4500, 1, 0.0005, 
                        3000, 2, 0.001, 
                        1800, 3, 0.001, 
                        1000, 4, 0.001,
                        500, 5, 0.001,
                        400, 6, 0.001};
        poisson24->SetParameters(par24);
        const char* parNames24[] = {
                            "A_{0}", "#mu_{0}", "#sigma_{0}",
                            "A_{1}", "#mu_{1}", "#sigma_{1}",
                            "A_{2}", "#mu_{2}", "#sigma_{2}",
                            "A_{3}", "#mu_{3}", "#sigma_{3}",
                            "A_{4}", "#mu_{4}", "#sigma_{4}",
                            "A_{5}", "#mu_{5}", "#sigma_{5}",
                            "A_{6}", "#mu_{6}", "#sigma_{6}"
                        };
        for(int i=0; i<21; i++) poisson24->SetParName(i, parNames24[i]);   // CAMBIARE IL NUMERO IN BASE AL N. PARAMETRI
        

        poisson27 = new TF1("poisson27", "gaus(0) + gaus(3) + gaus(6) + gaus(9) + gaus(12) + gaus(15) + gaus(18)", xmin_27, xmax_27);
        double par27[21] = {
                        13000, 0, 0.01,
                        13000, 1.05, 0.03, 
                        11000, 2, 0.02, 
                        10000, 3, 0.03, 
                        10000, 4, 0.03,
                        5000, 5, 0.05,
                        3000, 6, 0.1};//,
                        //500, 7, 0.1};
        poisson27->SetParameters(par27);
        const char* parNames27[] = {
                            "A_{0}", "#mu_{0}", "#sigma_{0}",
                            "A_{1}", "#mu_{1}", "#sigma_{1}",
                            "A_{2}", "#mu_{2}", "#sigma_{2}",
                            "A_{3}", "#mu_{3}", "#sigma_{3}",
                            "A_{4}", "#mu_{4}", "#sigma_{4}",
                            "A_{5}", "#mu_{5}", "#sigma_{5}",
                            "A_{6}", "#mu_{6}", "#sigma_{6}"
                        };
        for(int i=0; i<21; i++) poisson27->SetParName(i, parNames27[i]);

        poisson30 = new TF1("poisson30", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1) + gaus(2) + gaus(5) + gaus(8) + gaus(11) + gaus(14) + gaus(17) + gaus(20) + gaus(23)", xmin_30, xmax_30);
        double par30[26] = {1500, 5,
                            5000, 0, 0.03,
                            12000, 1, 0.03,
                            15000, 2, 0.03,
                            17000, 3, 0.03,
                            15000, 4, 0.03,
                            12000, 5, 0.03,
                            6000, 6, 0.05,
                            2000, 7, 0.12};
        poisson30->SetParameters(par30);
        const char* parNames30[] = {"B_{0}", "#lambda",
                            "A_{0}", "#mu_{0}", "#sigma_{0}",
                            "A_{1}", "#mu_{1}", "#sigma_{1}",
                            "A_{2}", "#mu_{2}", "#sigma_{2}",
                            "A_{3}", "#mu_{3}", "#sigma_{3}",
                            "A_{4}", "#mu_{4}", "#sigma_{4}",
                            "A_{5}", "#mu_{5}", "#sigma_{5}",
                            "A_{6}", "#mu_{6}", "#sigma_{6}",
                            "A_{7}", "#mu_{7}", "#sigma_{7}"
                        };
        for(int i=0; i<26; i++) poisson30->SetParName(i, parNames30[i]);
    }
    
    h24->SetLineColor(2);
    h27->SetLineColor(2);
    h30->SetLineColor(2);

    // creazione canvas
    TCanvas* c24 = new TCanvas("c24", "c24", 640,480);
    c24->cd(1);
    c24->SetTicks();
    h24->Draw("hist");

    //h24->Fit(poisson24, "WLR", "", xmin_24, xmax_24);
    //poisson24->Draw("same");
    


    TCanvas* c27 = new TCanvas("c27", "c27", 640,480);
    c27->cd(1);
    c27->SetTicks();
    h27->Draw("hist");

    //h27->Fit(poisson27, "WLR", "", xmin_27, xmax_27);
    //poisson27->Draw("same");
    


    TCanvas* c30 = new TCanvas("c30", "c30", 640,480);
    c30->cd(1);
    c30->SetTicks();
    h30->Draw("hist");

    //h30->Fit(poisson30, "WLR", "", xmin_30, xmax_30);
    //poisson30->Draw("same");
    


    if(!use_pe_bins){
        c24->Print("../plots/LED/fit_poisson_24.pdf");
        c27->Print("../plots/LED/fit_poisson_27.pdf");
        c30->Print("../plots/LED/fit_poisson_30.pdf");
    } else {
        c24->Print("../plots/LED/rebinned_fit_poisson_24.pdf");
        c27->Print("../plots/LED/rebinned_fit_poisson_27.pdf");
        c30->Print("../plots/LED/rebinned_fit_poisson_30.pdf");
    }


/*
    //_______FIT CON PUNTI PER POISSONIANA_____
    //dati dai fit gaussiani singoli

    vector<vector<double>> results24 = doubleReader("../data/outcome_fit/708V_30dB_24ua_deltapp.txt", 4, false);
    vector<vector<double>> results27 = doubleReader("../data/outcome_fit/708V_30dB_27ua_deltapp.txt", 4, false);
    vector<vector<double>> results30 = doubleReader("../data/outcome_fit/708V_30dB_30ua_deltapp.txt", 4, false);
        
    double ampiezze24[7], ampiezze27[7], ampiezze30[7];
    double e_amp_24[7], e_amp_27[7], e_amp_30[7];
    double pe[] = {0, 1, 2, 3, 4, 5, 6};
    
    for(int i=0; i<7; i++){
        ampiezze24[i] = results24[5][i]; //colonna 5 = norm
        ampiezze27[i] = results27[5][i];
        ampiezze30[i] = results30[5][i];

        e_amp_24[i] = results24[6][i];
        e_amp_27[i] = results27[6][i];
        e_amp_30[i] = results30[6][i];
    }

    // grafico con le ampiezze delle gaussiane singole
    TGraphErrors* g24 = new TGraphErrors(7, pe, ampiezze24, 0, e_amp_24);
    TGraphErrors* g27 = new TGraphErrors(7, pe, ampiezze27, 0, e_amp_27);
    TGraphErrors* g30 = new TGraphErrors(7, pe, ampiezze30, 0, e_amp_30);

    // definizione altre funzioni poissoniane
    TF1* poisson_g24 = new TF1("poisson_g24", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1)", -0.5, 6.5);
    poisson_g24->SetParameters(5000, 1);
    poisson_g24->SetParNames("A", "#lambda");

    TF1* poisson_g27 = new TF1("poisson_g27", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1)", -0.5, 6.5);
    poisson_g27->SetParameters(10000,2);
    poisson_g27->SetParNames("A", "#lambda");

    TF1* poisson_g30 = new TF1("poisson_g30", "[0] * TMath::Exp(-[1]) * TMath::Power([1], x) / TMath::Gamma(x+1)", -0.5, 6.5);
    poisson_g30->SetParameters(20000, 4);
    poisson_g30->SetParNames("A", "#lambda");

    // definizione funzioni binomiali
    TF1* binom24 = new TF1("binom24", "[2] * TMath::Binomial([0], TMath::Nint(x))*TMath::Power([1],TMath::Nint(x))*TMath::Power(1-[1], [0]-TMath::Nint(x))", -0.5, 6.5);
    binom24->SetParameters(10, 0.1, 10000);

    TF1* binom27= new TF1("binom27", "[2] * TMath::Binomial([0], TMath::Nint(x))*TMath::Power([1],TMath::Nint(x))*TMath::Power(1-[1], [0]-TMath::Nint(x))", -0.5, 6.5);
    binom27->SetParameters(10, 0.5, 10000);

    TF1* binom30 = new TF1("binom30", "[2] * TMath::Binomial([0], TMath::Nint(x))*TMath::Power([1],TMath::Nint(x))*TMath::Power(1-[1], [0]-TMath::Nint(x))", -0.5, 6.5);
    binom30->SetParameters(10, 0.5, 10000);

    // fit e draw delle funzioni
    TCanvas* cg24 = new TCanvas("cg24", "cg24", 640, 480);
    cg24->cd(1);
    cg24->SetTicks();
    if(fit_binomiale) g24->Fit(binom24, "R", "", -0.5, 6.5);
    else g24->Fit(poisson_g24, "R", "", -0.5, 6.5);
    g24->Draw("AP");
    cg24->Print("../plots/LED/scatterplot_24.pdf");

    TCanvas* cg27 = new TCanvas("cg27", "cg27", 640, 480);
    cg27->cd(1);
    cg27->SetTicks();
    if(fit_binomiale) g27->Fit(binom27, "R", "", -0.5, 6.5);
    else g27->Fit(poisson_g27, "R", "", -0.5, 6.5);
    g27->Draw("AP");
    cg27->Print("../plots/LED/scatterplot_27.pdf");

    TCanvas* cg30 = new TCanvas("cg30", "cg30", 640, 480);
    cg30->cd(1);
    cg30->SetTicks();
    if(fit_binomiale) g30->Fit(binom30, "R", "", -0.5, 6.5);
    else g30->Fit(poisson_g30, "R", "", -0.5, 6.5);
    g30->Draw("AP");
    cg30->Print("../plots/LED/scatterplot_30.pdf");
*/

}
