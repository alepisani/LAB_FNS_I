#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "letturaFile.cpp"
#include <vector>
#include <cmath>

void NuovaAnalisi(){

    //------ offset delle misure con il 9 senza il 9--------------

    TH1D* TDC21 = new TH1D("delta21", "offset_21", 60, -100., 200);
    TH1D* TDC31 = new TH1D("delta31", "offset_31", 60, -100., 200);
    TH1D* TDC41 = new TH1D("delta41", "offset_41", 60, -100., 200);
    TH1D* TDC51 = new TH1D("delta51", "offset_51", 60, -100., 200);
    


    vector<vector<int>> dati1 = datReader("../data/doppie/TDC_doppia19_1000.dat", 0);
    vector<vector<int>> dati2 = datReader("../data/doppie/TDC_doppia29_1000.dat", 0);
    vector<vector<int>> dati3 = datReader("../data/doppie/TDC_doppia39_1000.dat", 0);
    vector<vector<int>> dati4 = datReader("../data/doppie/TDC_doppia49_1000.dat", 0);
    vector<vector<int>> dati5 = datReader("../data/doppie/TDC_doppia59_1000.dat", 0);


    for (int i=0; i<1000; i++) {
        TDC21->Fill(dati2[2][i]-dati1[1][i]);
        TDC31->Fill(dati3[3][i]-dati1[1][i]);
        TDC41->Fill(dati4[4][i]-dati1[1][i]);
        TDC51->Fill(dati5[5][i]-dati1[1][i]);
    }


    TF1* gaussiano = new TF1("gaussiano", "gaus", -100, 200);
    gaussiano->SetLineColor(kBlue);
    gaussiano->SetParameter(0,0);
    gaussiano->SetParameter(1,0);
    gaussiano->SetParameter(2,20);


    
    TCanvas* t2t1 = new TCanvas("t2t1", "t2-t1", 800, 600);
    TDC21->Draw();
    TDC21->Fit(gaussiano, "R");
    gStyle->SetOptFit(1111);
    TDC21->GetXaxis()->SetTitle("Canali TDC");
    TDC21->GetYaxis()->SetTitle("Conteggi");
    double T2_T1 = gaussiano->GetParameter(1);

    TCanvas* t3t1 = new TCanvas("t3t1", "t3-t1", 800, 600);
    TDC31->Draw();
    TDC31->Fit(gaussiano, "R");
    gStyle->SetOptFit(1111);
    TDC31->GetXaxis()->SetTitle("Canali TDC");
    TDC31->GetYaxis()->SetTitle("Conteggi");
    double T3_T1 = gaussiano->GetParameter(1);

    TCanvas* t4t1 = new TCanvas("t4t1", "t4-t1", 800, 600);
    TDC41->Draw();
    TDC41->Fit(gaussiano, "R");
    gStyle->SetOptFit(1111);
    TDC41->GetXaxis()->SetTitle("Canali TDC");
    TDC41->GetYaxis()->SetTitle("Conteggi");
    double T4_T1 = gaussiano->GetParameter(1);

    TCanvas* t5t1 = new TCanvas("t5t1", "t5-t1", 800, 600);
    TDC51->Draw();
    TDC51->Fit(gaussiano, "R");
    gStyle->SetOptFit(1111);
    TDC51->GetXaxis()->SetTitle("Canali TDC");
    TDC51->GetYaxis()->SetTitle("Conteggi");
    double T5_T1 = gaussiano->GetParameter(1);
   
    cout<<"offset rispetto all'1 \n" << T2_T1 << "\n" << T3_T1 << "\n" << T4_T1 << "\n" << T5_T1 << endl;

    vector<vector<double>> data = doubleReader("../data/quintupla_lunga/TDC_quintupla_5000.dat", 0);
    vector<double> t24_1; 
    vector<double> t35_1;

    for (int i=0; i<data[0].size(); i++){
   
        t24_1.push_back((data[2][i]-data[4][i]-T2_T1+T4_T1)/4.15);
        t35_1.push_back((data[3][i]-data[5][i]-T3_T1+T5_T1)/4.15);
        
    }

    vector<double> A_1;
    vector<double> B_1;

    for(int i=0; i<5000; i++){
        A_1.push_back((3./20.41)*((t24_1[i]+t35_1[i])/10));  // diviso 10 perchè c in s e tempo in ns
        B_1.push_back((3./21.62)*((t24_1[i]-t35_1[i])/10));
        if(i%20){
            //cout<<"valori di A e B"<<A[i]<<" - "<<B[i]<<endl;
        }
    }

    vector<double> theta_1; 
    vector<double> phi_1;
    

    TH1D* htheta_1 = new TH1D("theta", "distribuzione zenitale", 95, -10, 91);
    //htheta->GetYaxis()->SetRangeUser(0, 250);
    TH1D* hphi_1 = new TH1D("phi", "distribuzione azimutale", 70, 0, 6.3);
    hphi_1->GetYaxis()->SetRangeUser(35,170);

    for(int i=0; i<5000; i++){
        theta_1.push_back((asin(sqrt(A_1[i]*A_1[i]+B_1[i]*B_1[i])))*180/M_PI);
        //phi.push_back(atan(B[i]/A[i]));
        if(A_1[i]*A_1[i]+B_1[i]*B_1[i]<1){   //controllo per evitare il rumore, se c'è qualcosa
            htheta_1->Fill(asin(sqrt(A_1[i]*A_1[i]+B_1[i]*B_1[i]))*180/M_PI);
            //hphi->Fill(atan(B[i]/A[i]));
            double phi_i_1 = (atan2(B_1[i], A_1[i]));
            if(phi_i_1 < 0) phi_i_1 += 2*M_PI;
            hphi_1->Fill(phi_i_1);
        }
    }

     // --------------------- fit per lambda ---------------------
    double x_0 = 1012.990; //cambiare con quella che viene con la pressione quel giorno

    TF1 *f_theta = new TF1( "f_theta", Form("[0]*sin(x*TMath::Pi()/180.)*cos(x*TMath::Pi()/180.)*exp(-%f/([1]*cos(x*TMath::Pi()/180.)))", x_0), 0, 90);
    f_theta->SetParameter(0, 100);  // N iniziale, costante di normalizzazione
    f_theta->SetParameter(1, 120);   // lambda iniziale
    f_theta->SetLineColor(kRed);
    f_theta->SetParName(0, "N");
    f_theta->SetParName(1, "lambda");

    TCanvas* ctheta_1 = new TCanvas ("ctheta1", "theta", 800, 600);
    htheta_1->Draw();  //"E1" nella parentesi se vogliamo punti e non bin
    htheta_1->Fit(f_theta, "R");
    htheta_1->GetXaxis()->SetTitle("Angolo zenitale [gradi]");
    htheta_1->GetXaxis()->SetRangeUser(0, 90);
    htheta_1->GetYaxis()->SetTitle("Conteggi");
    gStyle->SetOptFit(1111);

    TCanvas* cphi_1 = new TCanvas ("cphi1", "phi", 800, 600);
    hphi_1->Draw();
    hphi_1->Fit("pol1");
    TF1 *fit1 = hphi_1->GetFunction("pol1");
    fit1->SetParameter(1,0);
    double q1 = fit1->GetParameter(0);
    double m1 = fit1->GetParameter(1);
    hphi_1->GetXaxis()->SetTitle("Angolo azimutale [rad] ");
    hphi_1->GetYaxis()->SetTitle("Conteggi");
    fit1->SetParName(0, "q1");
    fit1->SetParName(1, "m1");
    //cout<<"p-value "<<fit->GetProb()<<endl;
    gStyle->SetOptFit(1111);

    //------------------------ spessore dello sciame ---------------------------------------
    TH1D* Tsciame1 = new TH1D("Tsciame1", "T=t2-t3+t4-t5", 90, -60, 60);

    for (int i=0; i<5000; i++){
        double T_i1 = (data[2][i]-data[3][i]+data[4][i]-data[5][i]-T2_T1+T3_T1-T4_T1+T5_T1)/4.15;
        Tsciame1->Fill(T_i1);
    }

   
    int    bin_max1  = Tsciame1->GetMaximumBin();
    double max_val1  = Tsciame1->GetMaximum();
    double half_max1 = max_val1 / 2.0;

    // Scansiona a sinistra e a destra del massimo
    int bin_left1  = bin_max1;
    int bin_right1 = bin_max1;

    while (Tsciame1->GetBinContent(bin_left1)  > half_max1 && bin_left1  > 1) bin_left1--;
    while (Tsciame1->GetBinContent(bin_right1) > half_max1 && bin_right1 < Tsciame1->GetNbinsX()) bin_right1++;

    double x_left1  = Tsciame1->GetBinCenter(bin_left1);
    double x_right1 = Tsciame1->GetBinCenter(bin_right1);
    double fwhm1    = x_right1 - x_left1;

    std::cout << "xright, xleft1 = " << x_right1 << "   " << x_left1 << std::endl;
    std::cout << "FWHM1 = " << fwhm1 << std::endl;
    cout << "std dev " << Tsciame1->GetRMS() << " +- " << Tsciame1->GetRMSError() << endl;

    TCanvas* cTsciame1 = new TCanvas("cTsciame1", "T=t2-t3+t4-t5", 800, 600);
    Tsciame1->GetXaxis()->SetTitle("T [ns]");
    Tsciame1->GetYaxis()->SetTitle("Conteggi");
    Tsciame1->Draw();
    Tsciame1->Fit(gaussiano);
    cTsciame1->Update();
    


}
