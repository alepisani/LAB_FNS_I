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
    
    
    // bin width è 8, dal tuo file (-996.5, -988.5, ...)
    double xmin = -1000.5;
    double xmax = 23004.5;  // aggiusta se necessario
    int nbins = data[0].size();

    TH1D* led_spectrum = new TH1D("spectrum", "LED spectrum", nbins, xmin, xmax);

    for (int i = 0; i < (int)data[0].size(); i++) {
        
        led_spectrum->SetBinContent(i + 1, data[1][i]); // i+1 perché ROOT parte da bin 1
    
    }

    //poisson+gauss fit 
    int xmin1 = -100;
    int xmax1 = 1500;
    TF1* poisson_gauss = new TF1("poisson_gauss", "[0] * TMath::Poisson(x, [1]) + gaus(2) + gaus(5) + gaus(8) + gaus(11) + gaus(14) + gaus(17) + gaus(20)", xmin1, xmax1);
    
    poisson_gauss->SetParameter(3, 5000);
    poisson_gauss->SetParameter(4, 0);
    poisson_gauss->SetParameter(5, 20);
    
    poisson_gauss->SetParameter(6, 9000);
    poisson_gauss->SetParameter(7, 180);
    poisson_gauss->SetParameter(8, 20);

    poisson_gauss->SetParameter(9, 8000);
    poisson_gauss->SetParameter(10, 350);
    poisson_gauss->SetParameter(11, 20);

    poisson_gauss->SetParameter(12, 6000);
    poisson_gauss->SetParameter(13, 550);
    poisson_gauss->SetParameter(14, 20);

    poisson_gauss->SetParameter(15, 3000);
    poisson_gauss->SetParameter(16, 650);
    poisson_gauss->SetParameter(17, 20);

    poisson_gauss->SetParameter(18, 1500);
    poisson_gauss->SetParameter(19, 850);
    poisson_gauss->SetParameter(20, 20);

    poisson_gauss->SetParameter(21, 1000);
    poisson_gauss->SetParameter(22, 950);
    poisson_gauss->SetParameter(23, 20);
    

    TCanvas *can1_poisson_gauss = new TCanvas("c1_poisson_gauss", "Poisson + Gauss", 800, 600);
    led_spectrum->GetXaxis()->SetTitle("Canali");
    led_spectrum->GetYaxis()->SetTitle("Conteggi");
    led_spectrum->SetTitle("704V_30dB_30ua_histo.txt");
    led_spectrum->GetXaxis()->SetRangeUser(xmin1, xmax1);
    led_spectrum->Draw();
    led_spectrum->Fit(poisson_gauss, "R"); 
    gStyle->SetOptFit(1111);
    


}
 
