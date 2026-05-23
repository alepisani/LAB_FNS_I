/**
 * macro to make the poisson + n-gaussian on led spectrums
 * goals to identify the centroids and delta peaks in order to computer resolution
 * 
 * file to use:
 * ../data/txt/704V_30dB_30ua_histo.txt
 * and similar .txt to this one
*/

#include <iostream>
#include "txt_reader.cpp"
#include "TH1D.h"
using namespace std;


void led_poisson_gauss(){

    //take the data from the files.txt

    vector<vector<double>> data = txt_reader("../data/txt/704V_30dB_30ua_histo.txt");

    vector<double> bin, counts;
    
    TH1D* led_spectrum = new TH1D("spectrum", "LED spectrum", 3000, -996.5, 22996);
    
    for (int i = 0; i < data[0].size(); i++){

        bin.push_back(data[0][i]);
        counts.push_back(data[1][i]);
        led_spectrum->Fill(data[0][i], data[1][i]);

    }

    //landau+gauss fit 
    int xmin1 = -100;
    int xmax1 = 1500;
    TF1* poisson_gauss = new TF1("langauss1", "landau(0) + gaus(3) + gaus(6) + gaus(9) + gaus(12)", xmin1, xmax1);
    
    /*
    langauss1->SetParameter(0, 200);
    langauss1->SetParameter(1, 105);
    langauss1->SetParameter(2, 30);
    langauss1->SetParameter(3, 250);
    langauss1->SetParameter(4, 68);
    langauss1->SetParameter(5, 10);
    */
    

    TCanvas *can1_poisson_gauss = new TCanvas("c1_poisson_gauss", "Poisson + Gauss", 800, 600);
    led_spectrum->GetXaxis()->SetTitle("Canali");
    led_spectrum->GetYaxis()->SetTitle("Conteggi");
    led_spectrum->SetTitle("704V_30dB_30ua_histo.txt");
    led_spectrum->GetXaxis()->SetRangeUser(xmin1, xmax1);
    led_spectrum->Draw();
    led_spectrum->Fit(poisson_gauss, "RQ"); 
    gStyle->SetOptFit(1111);
    


}
 
