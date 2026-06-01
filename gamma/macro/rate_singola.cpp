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
#include <cmath> 
#include "letturaFile.cpp"

void rate_singola(){
    vector<double> angoli = {-32, -30, -28, -26, -24, -22, -20, -18, -14, -10, -5, 0, +5, +10, +14, +18, +20, +22, +24, +26, +28, +30, +32}; //angoli acquisiti
    vector<double> tempi = {300, 300, 300, 300, 300, 300, 240, 180, 120, 120, 120, 60, 60, 60, 60, 120, 180, 180, 200, 200, 240, 300, 300, 300};
    vector<double> s_angoli={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    vector<double> count_fisso = {38131, 38575, 38323, 38782, 38787, 38755, 31208, 23080, 15636, 15385, 15713, 7762, 7852, 7790, 7783, 15886, 23814, 23602, 26354, 26002, 30928, 39547, 39164, 39301};
    vector<double> count_mobile = {43917, 43914, 44355, 44108, 43877, 43608, 34907, 26253, 17887, 17863, 17604, 8874, 8897, 8790, 8838, 17904, 26356, 26595, 29574, 29586, 34884, 44496, 44111, 44381};
    vector<double> rate_fisso;
    vector<double> rate_mobile;
    vector<double> s_rate_fisso;
    vector<double> s_rate_mobile;

    for ( int i =0; i<tempi.size(); i++){
        rate_fisso.push_back(count_fisso[i]/tempi[i]);
        rate_mobile.push_back(count_mobile[i]/tempi[i]);
        s_rate_fisso.push_back(sqrt(count_fisso[i])/tempi[i]);
        s_rate_mobile.push_back(sqrt(count_mobile[i])/tempi[i]);
    }


    gStyle->SetOptFit(1111);
    //TF1 *fit_lineare = new TF1("fit_lineare", "pol1", -35, 35);

    TGraphErrors *fisso = new TGraphErrors(
    angoli.size(),
    angoli.data(),
    rate_fisso.data(),
    s_angoli.data(),
    s_rate_fisso.data()
    );
    
    
    

    TCanvas* c1 = new TCanvas("c1", "Rate scintillatore fisso", 800, 600); 

    fisso->SetTitle("Rate scintillatore fisso");
    fisso->GetXaxis()->SetTitle("Angolo [#varphi]");
    fisso->GetYaxis()->SetTitle("Rate [Hz]");
    fisso->SetMarkerStyle(20);
    fisso->SetMarkerColor(kBlue);
    fisso->SetMaximum(150);
    fisso->SetMinimum(110);
    //fisso->Fit(fit_lineare, "R");
    fisso->Draw("AP");
    




    TGraphErrors *mobile = new TGraphErrors(
    angoli.size(),
    angoli.data(),
    rate_mobile.data(),
    s_angoli.data(),
    s_rate_mobile.data()
    );

    TCanvas* c2 = new TCanvas("c2", "Rate scintillatore mobile", 800, 600); 
    mobile->SetTitle("Rate scintillatore mobile");
    mobile->GetXaxis()->SetTitle("Angolo [#varphi]");
    mobile->GetYaxis()->SetTitle("Rate [Hz]");
    mobile->SetMarkerStyle(20);
    mobile->SetMarkerColor(kBlue);
    mobile->SetMaximum(160);
    mobile->SetMinimum(130);
    //mobile->Fit(fit_lineare, "R");
    mobile->Draw("AP");


}