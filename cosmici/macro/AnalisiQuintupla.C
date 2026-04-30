#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "letturaFile.cpp"
#include <vector>
#include <cmath>

void AnalisiQuintupla(){
    
    TH1D* deltaT19 = new TH1D("delta19", "offset_19", 100, 0., 200);
    TH1D* deltaT29 = new TH1D("delta29", "offset_29", 100, 0., 200);
    TH1D* deltaT39 = new TH1D("delta39", "offset_39", 100, 0., 200);
    TH1D* deltaT49 = new TH1D("delta49", "offset_49", 100, 0., 200);
    TH1D* deltaT59 = new TH1D("delta59", "offset_59", 100, 0., 200);


    vector<vector<int>> dati1 = datReader("../data/doppie/TDC_doppia19_1000.dat", 0);
    vector<vector<int>> dati2 = datReader("../data/doppie/TDC_doppia29_1000.dat", 0);
    vector<vector<int>> dati3 = datReader("../data/doppie/TDC_doppia39_1000.dat", 0);
    vector<vector<int>> dati4 = datReader("../data/doppie/TDC_doppia49_1000.dat", 0);
    vector<vector<int>> dati5 = datReader("../data/doppie/TDC_doppia59_1000.dat", 0);
  
    
    for (int i=0; i<1000; i++) {
        deltaT19->Fill(dati1[1][i]-dati1[9][i]);
        deltaT29->Fill(dati2[2][i]-dati2[9][i]);
        deltaT39->Fill(dati3[3][i]-dati3[9][i]);
        deltaT49->Fill(dati4[4][i]-dati4[9][i]);
        deltaT59->Fill(dati5[5][i]-dati5[9][i]);

    }

    //------------------ offset rispetto al 9 -----------------
    double T19 = deltaT19->GetMean();
    double T29 = deltaT29->GetMean();
    double T39 = deltaT39->GetMean();
    double T49 = deltaT49->GetMean();
    double T59 = deltaT59->GetMean();

    //----------- ritardi catene elettroniche ------------------
    double T21 = T29-T19; 
    double T31 = T39-T19;
    double T41 = T49-T19;
    double T51 = T59-T19;
    cout << "ritardi delle catene elettroniche" << "\n" << T21 << "\n" << T31 << "\n" << T41 << "\n" << T51 << endl; 

    TF1* solo_gauss = new TF1("solo_gauss", "gaus", 0, 200);
    //solo_gauss->SetParameter(0, 20);
    //solo_gauss->SetParameter(1, 90);
    //solo_gauss->SetParameter(2, 15);
    solo_gauss->SetLineColor(kBlue);
    
    TCanvas* c1 = new TCanvas("c1", "offset_19", 800, 600);
    //deltaT19->Draw();
    deltaT19->Fit(solo_gauss, "R");
    gStyle->SetOptFit(1111);
    
    TCanvas* c2 = new TCanvas("c2", "offset_29", 800, 600);
    //deltaT29->Draw();
    deltaT29->Fit(solo_gauss, "R");
    gStyle->SetOptFit(1111);

    TCanvas* c3 = new TCanvas("c3", "offset_39", 800, 600);
    //deltaT39->Draw();
    deltaT39->Fit(solo_gauss, "R");
    gStyle->SetOptFit(1111);

    TCanvas* c4 = new TCanvas("c4", "offset_49", 800, 600);
    //deltaT49->Draw();
    deltaT49->Fit(solo_gauss, "R");
    gStyle->SetOptFit(1111);

    TCanvas* c5 = new TCanvas("c5", "offset_59", 800, 600);
    //deltaT59->Draw();
    deltaT59->Fit(solo_gauss, "R");
    gStyle->SetOptFit(1111);


    //--------------------- direzioni d'arrivo --------------------------------

    vector<vector<int>> data = datReader("../data/quintupla/TDC_quintupla_1000.dat", 0);
    vector<int> t24; 
    vector<int> t35;

    for (int i=0; i<1000; i++){
        t24.push_back(data[2][i]-data[4][i]-T21+T41);
        t35.push_back(data[3][i]-data[5][i]-T31+T51);
    }

    vector<double> A;
    vector<double> B;

    for(int i=0; i<1000; i++){
        A.push_back((3*pow(10,8)/20)*(t24[i]+t35[i]));
        B.push_back((3*pow(10,8)/20)*(t24[i]-t35[i]));
    }
    
    vector<double> theta; 
    vector<double> phi;
    TH1D* htheta = new TH1D("theta", "distribuzione zenitale",100, 0., 91);
    htheta->GetYaxis()->SetRangeUser(0, 250);
    TH1D* hphi = new TH1D("phi", "distribuzione azimutale", 100, 0., 6.5);

    for(int i=0; i<1000; i++){
        theta.push_back(asin(sqrt(A[i]*A[i]+B[i]*B[i])));
        phi.push_back(atan(B[i]/A[i]));
        htheta->Fill(asin(sqrt(A[i]*A[i]+B[i]*B[i])));
        hphi->Fill(atan(B[i]/A[i]));
    }

    TCanvas* ctheta = new TCanvas ("ctheta", "theta", 600, 800);
    htheta->Draw();

    TCanvas* cphi = new TCanvas ("cphi", "phi", 600, 800);
    hphi->Draw();



    //---------------------- fit per gli offset -----------------------------------------
    
    /*TFile *input_file = TFile::Open("../data/doppie/TDC_doppia19_1000.root");
    TH1D* hist = (TH1D*)input_file->Get("tdc");
    
    TF1* solo_gauss = new TF1("solo_gauss", "gaus", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
    solo_gauss->SetParameter(0, hist->GetMaximum());
    solo_gauss->SetParameter(1, hist->GetMean());
    solo_gauss->SetParameter(2, hist->GetRMS());
    solo_gauss->SetLineColor(kBlue);

    TCanvas *c_gauss = new TCanvas("c4", "solo gauss", 800, 600);
    hist->GetXaxis()->SetTitle("canali TDC");
    hist->GetYaxis()->SetTitle("conteggi");
    hist->Draw();
    hist->Fit(solo_gauss, "R");
    gStyle->SetOptFit(1111);
    cout << "Chi^2:" << solo_gauss->GetChisquare() << ", number of DoF: " << solo_gauss->GetNDF() << " (Probability: " << solo_gauss->GetProb() << ")." << endl;
    */

}