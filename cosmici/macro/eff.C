#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLine.h"
#include "TStyle.h"

using namespace std;

void eff() {

    const int nmisure = 14;

    double T[nmisure]   = {300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300};
    double sT[nmisure]  = {0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001};

    double HV[nmisure]  = {1300, 1349, 1375, 1400, 1450, 1500, 1550, 1601, 1650, 1701, 1751, 1798, 1851, 1901};
    double sHV[nmisure] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};

    // Conteggi SG
    double SG[nmisure]  = {455, 1546, 2333, 3249, 4576, 6570, 15368, 58146, 255503, 910680, 2250209, 4552637, 9583604, 19423319};
    double sSG[nmisure] = {};
    double R1[nmisure]  = {};
    double sR1[nmisure] = {};

    // Coincidenza doppia S1 AND S2
    double AC[nmisure]  = {100, 95, 81, 98, 93, 94, 82, 96, 87, 86, 75, 85, 99, 73};
    double sAC[nmisure] = {};
    double R2[nmisure]  = {};
    double sR2[nmisure] = {};

    // Coincidenza tripla S1 AND S2 AND SG
    double ABC[nmisure]  = {20, 43, 37, 73, 62, 60, 54, 62, 57, 68, 60, 63, 74, 49};
    double sABC[nmisure] = {};
    double R3[nmisure]   = {};
    double sR3[nmisure]  = {};

    // Efficienza
    double EFF[nmisure]  = {};
    double sEFF[nmisure] = {};

    // Stampa errori su HV
    for (int i = 0; i < nmisure; ++i) {
        cout << "\nMISURA DI HV NUMERO " << i
             << " : HV = " << HV[i]
             << "\t sHV = " << sHV[i] << endl;
    }

    cout << "|---------------------------------------------------|" << endl;

    // Errori sui conteggi
    for (int i = 0; i < nmisure; ++i) {
        sSG[i] = sqrt(SG[i]);
        cout << "\nMISURA CONTEGGIO " << i
             << " : SG = " << SG[i]
             << "\t sSG = " << sSG[i] << endl;
    }

    cout << "|---------------------------------------------------|" << endl;

    for (int i = 0; i < nmisure; ++i) {
        sAC[i] = sqrt(AC[i]);
        cout << "\nMISURA CONTEGGIO " << i
             << " : AC = " << AC[i]
             << "\t sAC = " << sAC[i] << endl;
    }

    cout << "|---------------------------------------------------|" << endl;

    for (int i = 0; i < nmisure; ++i) {
        sABC[i] = sqrt(ABC[i]);
        cout << "\nMISURA CONTEGGIO " << i
             << " : ABC = " << ABC[i]
             << "\t sABC = " << sABC[i] << endl;
    }

    cout << "|---------------------------------------------------|" << endl;

    // Rate SG
    for (int i = 0; i < nmisure; ++i) {
        R1[i] = SG[i] / T[i];
        sR1[i] = sqrt(pow((SG[i] / (T[i] * T[i])) * sT[i], 2) + pow((1.0 / T[i]) * sSG[i], 2));

        cout << "\nMISURA DI RATE SG " << i
             << " : R1 = " << R1[i]
             << "\t sR1 = " << sR1[i] << endl;
    }

    cout << "|---------------------------------------------------|" << endl;

    // Rate AC
    for (int i = 0; i < nmisure; ++i) {
        R2[i] = AC[i] / T[i];
        sR2[i] = sqrt(pow((AC[i] / (T[i] * T[i])) * sT[i], 2) + pow((1.0 / T[i]) * sAC[i], 2));

        cout << "\nMISURA DI RATE AC " << i
             << " : R2 = " << R2[i]
             << "\t sR2 = " << sR2[i] << endl;
    }

    cout << "|---------------------------------------------------|" << endl;

    // Rate ABC
    for (int i = 0; i < nmisure; ++i) {
        R3[i] = ABC[i] / T[i];
        sR3[i] = sqrt(pow((ABC[i] / (T[i] * T[i])) * sT[i], 2) + pow((1.0 / T[i]) * sABC[i], 2));

        cout << "\nMISURA DI RATE ABC " << i
             << " : R3 = " << R3[i]
             << "\t sR3 = " << sR3[i] << endl;
    }

    cout << "|---------------------------------------------------|" << endl;

    // Efficienza
    for (int i = 0; i < nmisure; ++i) {
        EFF[i] = R3[i] / R2[i];
        sEFF[i] = sqrt((EFF[i] * (1.0 - EFF[i])) / AC[i]);

        cout << "\nMISURA DI EFFICIENZA " << i
             << " : Eff = " << EFF[i]
             << "\t sEFF = " << sEFF[i] << endl;
    }

    cout << "|---------------------------------------------------|" << endl;

    
    // Punto preso nella notte

    double Tblue = 74590.0;
    double sTblue = 0.001;

    double Ntriple_blue = 3539;
    double Nquad_blue   = 3378;
    double NSG_blue     = 12920898; 

    double sNtriple_blue = sqrt(Ntriple_blue);
    double sNquad_blue   = sqrt(Nquad_blue);
    double sNSG_blue     = sqrt(NSG_blue);

    double EFFblue  = Nquad_blue / Ntriple_blue;
    double sEFFblue = sqrt((EFFblue * (1.0 - EFFblue)) / Ntriple_blue);

    cout << "\nPUNTO BLU:" << endl;
    cout << "Tempo misura = " << Tblue << " +/- " << sTblue << " s" << endl;
    cout << "Conteggi SG = " << NSG_blue << " +/- " << sNSG_blue << endl;
    cout << "Conteggi tripla = " << Ntriple_blue << " +/- " << sNtriple_blue << endl;
    cout << "Conteggi quadrupla = " << Nquad_blue << " +/- " << sNquad_blue << endl;
    cout << "EFFblue = " << EFFblue << " +/- " << sEFFblue << endl;

    TCanvas *cEhv = new TCanvas("cEhv", "Efficiency", 0, 1, 600, 400);
    cEhv->SetFillColor(0);
    cEhv->cd();

    TGraphErrors *gEhv1 = new TGraphErrors(nmisure, HV, EFF, sHV, sEFF);
    gEhv1->SetMarkerSize(0.6);
    gEhv1->SetMarkerStyle(21);
    gEhv1->SetTitle("Eff(HV)");
    gEhv1->GetXaxis()->SetTitle("HV (V)");
    gEhv1->GetYaxis()->SetTitle("Eff");
    gEhv1->Draw("AP");

    gEhv1->GetXaxis()->SetRangeUser(1275, 1925);
    gEhv1->GetYaxis()->SetRangeUser(0.001, 1.10);
    cEhv->Update();

    TF1 *funz1 = new TF1("funz1", "[0]/(1+exp(-((x-[1])/[2])))", 1300, 1925);

    funz1->SetParameter(0, 0.8);
    funz1->SetParameter(1, 1400);
    funz1->SetParameter(2, 500);

    funz1->SetParLimits(0, 0.1, 1.0);
    funz1->SetParLimits(1, 0.0, 1800.0);
    funz1->SetParLimits(2, 50.0, 800.0);

    funz1->SetLineColor(kRed);

    TFitResultPtr fit_res = gEhv1->Fit(funz1, "RMS+");

    fit_res->PrintCovMatrix(cout);
    gStyle->SetOptFit(1111);

    double xsel = 1600.0;
    double ysel = EFFblue;

    TGraph *gpoint = new TGraph(1);
    gpoint->SetPoint(0, xsel, ysel);
    gpoint->SetMarkerStyle(34);
    gpoint->SetMarkerSize(1.6);
    gpoint->SetMarkerColor(kBlue);
    gpoint->Draw("P SAME");

    TLine *lv = new TLine(xsel, 0.0, xsel, ysel);
    lv->SetLineColor(kBlue);
    lv->SetLineStyle(2);
    lv->SetLineWidth(2);
    lv->Draw("SAME");

    TLine *lh = new TLine(1275.0, ysel, xsel, ysel);
    lh->SetLineColor(kBlue);
    lh->SetLineStyle(2);
    lh->SetLineWidth(2);
    lh->Draw("SAME");

    cEhv->Update();

    cout << "Chi^2: " << funz1->GetChisquare()
         << ", number of DoF: " << funz1->GetNDF()
         << " (Probability: " << funz1->GetProb() << ")." << endl;

    cout << "--------------------------------------------------------------------------------------------------------" << endl;
}
