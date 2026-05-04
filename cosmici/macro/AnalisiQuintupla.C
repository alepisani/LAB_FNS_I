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
  
    
    for (int i=0; i<5000; i++) {
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
    
    /*TCanvas* c1 = new TCanvas("c1", "offset_19", 800, 600);
    //deltaT19->Draw();
    deltaT19->Fit(solo_gauss, "R");
    gStyle->SetOptFit(1111);
    deltaT19->GetXaxis()->SetTitle("Canali TDC");
    deltaT19->GetYaxis()->SetTitle("Conteggi");
    
    TCanvas* c2 = new TCanvas("c2", "offset_29", 800, 600);
    //deltaT29->Draw();
    deltaT29->Fit(solo_gauss, "R");
    gStyle->SetOptFit(1111);
    deltaT29->GetXaxis()->SetTitle("Canali TDC");
    deltaT29->GetYaxis()->SetTitle("Conteggi");

    TCanvas* c3 = new TCanvas("c3", "offset_39", 800, 600);
    //deltaT39->Draw();
    deltaT39->Fit(solo_gauss, "R");
    gStyle->SetOptFit(1111);
    deltaT39->GetXaxis()->SetTitle("Canali TDC");
    deltaT39->GetYaxis()->SetTitle("Conteggi");

    TCanvas* c4 = new TCanvas("c4", "offset_49", 800, 600);
    //deltaT49->Draw();
    deltaT49->Fit(solo_gauss, "R");
    gStyle->SetOptFit(1111);
    deltaT49->GetXaxis()->SetTitle("Canali TDC");
    deltaT49->GetYaxis()->SetTitle("Conteggi");

    TCanvas* c5 = new TCanvas("c5", "offset_59", 800, 600);
    //deltaT59->Draw();
    deltaT59->Fit(solo_gauss, "R");
    gStyle->SetOptFit(1111);
    deltaT59->GetXaxis()->SetTitle("Canali TDC");
    deltaT59->GetYaxis()->SetTitle("Conteggi");*/


    //--------------------- direzioni d'arrivo --------------------------------

    vector<vector<double>> data = doubleReader("../data/quintupla_lunga/TDC_quintupla_5000.dat", 0);
    vector<double> t24; 
    vector<double> t35;

    ///------------vediamo i dati delle quintuple-----------------------
    TH1D* TDC1 = new TH1D("TDC1", "Spettro di TDC di coincidenze quintuple (CH1)", 200, 0., 500);
    TH1D* TDC2 = new TH1D("TDC2", "Spettro di TDC di coincidenze quintuple (CH2)", 200, 0., 500);
    TH1D* TDC3 = new TH1D("TDC3", "Spettro di TDC di coincidenze quintuple (CH3)", 200, 0., 500);
    TH1D* TDC4 = new TH1D("TDC4", "Spettro di TDC di coincidenze quintuple (CH4)", 200, 0., 500);
    TH1D* TDC5 = new TH1D("TDC5", "Spettro di TDC di coincidenze quintuple (CH5)", 200, 0., 500);

    for (int i=0; i<data[0].size(); i++){
        t24.push_back((data[2][i]-data[4][i]-T21+T41)/4.14); //da ch a ns
        t35.push_back((data[3][i]-data[5][i]-T31+T51)/4.14);
        TDC1->Fill(data[1][i]);
        TDC2->Fill(data[2][i]);
        TDC3->Fill(data[3][i]);
        TDC4->Fill(data[4][i]);
        TDC5->Fill(data[5][i]);
    }

    /*TCanvas* tdc1 = new TCanvas("tdc1", "TDC1-quintuple", 800, 600);
    TDC1->Draw();
    gStyle->SetOptFit(1111);
    TDC1->GetXaxis()->SetTitle("Canali TDC");
    TDC1->GetYaxis()->SetTitle("Conteggi");


    TCanvas* tdc2 = new TCanvas("tdc2", "TDC2-quintuple", 800, 600);
    TDC2->Draw();
    gStyle->SetOptFit(1111);
    TDC2->GetXaxis()->SetTitle("Canali TDC");
    TDC2->GetYaxis()->SetTitle("Conteggi");

    TCanvas* tdc3 = new TCanvas("tdc3", "TDC3-quintuple", 800, 600);
    TDC3->Draw();
    gStyle->SetOptFit(1111);
    TDC3->GetXaxis()->SetTitle("Canali TDC");
    TDC3->GetYaxis()->SetTitle("Conteggi");

    TCanvas* tdc4 = new TCanvas("tdc4", "TDC4-quintuple", 800, 600);
    TDC4->Draw();
    gStyle->SetOptFit(1111);
    TDC4->GetXaxis()->SetTitle("Canali TDC");
    TDC4->GetYaxis()->SetTitle("Conteggi");

    TCanvas* tdc5 = new TCanvas("tdc5", "TDC5-quintuple", 800, 600);
    TDC5->Draw();
    gStyle->SetOptFit(1111);
    TDC5->GetXaxis()->SetTitle("Canali TDC");
    TDC5->GetYaxis()->SetTitle("Conteggi");*/

    //----------- calcolo A, B, theta e phi -----------------

    cout<<"VALORI DI T24 E T35"<<t24[10]<<"\n"<<t24[20]<<"\n"<<t35[10]<<endl;

    vector<double> A;
    vector<double> B;

    for(int i=0; i<5000; i++){
        A.push_back((3./21.6)*((t24[i]+t35[i])/10));  // diviso 10 perchè c in s e tempo in ns
        B.push_back((3./20.4)*((t24[i]-t35[i])/10));
        if(i%20){
            cout<<"valori di A e B"<<A[i]<<" - "<<B[i]<<endl;
        }
    }

    cout<<"AAAAAAAAAAAAAAAAA"<<A[10]*A[10]+B[10]*B[10]<<endl;
    
    
    vector<double> theta; 
    vector<double> phi;
    TH1D* htheta = new TH1D("theta", "distribuzione zenitale",80, -10, 91);
    //htheta->GetYaxis()->SetRangeUser(0, 250);
    TH1D* hphi = new TH1D("phi", "distribuzione azimutale", 80, -3.2, 6.5);
    //hphi->GetYaxis()->SetRangeUser(0,70);

    for(int i=0; i<5000; i++){
        theta.push_back((asin(sqrt(A[i]*A[i]+B[i]*B[i])))*180/M_PI);
        //phi.push_back(atan(B[i]/A[i]));
        if(A[i]*A[i]+B[i]*B[i]<1){   //controllo per evitare il rumore, se c'è qualcosa
            htheta->Fill(asin(sqrt(A[i]*A[i]+B[i]*B[i]))*180/M_PI);
            //hphi->Fill(atan(B[i]/A[i]));
            double phi_i = atan2(B[i], A[i]);
            if(phi_i < 0) phi_i += 2*M_PI;
            hphi->Fill(phi_i);
        }

    }

    cout<<"EEEEEEEEEEEE"<<theta[10]<<endl;
    

    /*TCanvas* ctheta = new TCanvas ("ctheta", "theta", 600, 800);
    htheta->Draw();

    TCanvas* cphi = new TCanvas ("cphi", "phi", 600, 800);
    hphi->Draw();*/

    // --------------------- fit per lambda ---------------------
    double x_0 = 1012.990; //cambiare con quella che viene con la pressione quel giorno

    TF1 *f_theta = new TF1( "f_theta", Form("[0]*sin(x*TMath::Pi()/180.)*cos(x*TMath::Pi()/180.)*exp(-%f/([1]*cos(x*TMath::Pi()/180.)))", x_0), 0, 90);
    f_theta->SetParameter(0, 100);  // N iniziale, costante di normalizzazione
    f_theta->SetParameter(1, 120);   // lambda iniziale

    TCanvas* ctheta = new TCanvas ("ctheta", "theta", 800, 600);
    htheta->Draw();  //"E1" nella parentesi se vogliamo punti e non bin
    f_theta->SetLineColor(kRed);
    f_theta->SetParName(0, "N");
    f_theta->SetParName(1, "lambda");
    htheta->Fit(f_theta, "R");
    htheta->GetXaxis()->SetTitle("Angolo zenitale [gradi]");
    htheta->GetXaxis()->SetRangeUser(0, 90);
    htheta->GetYaxis()->SetTitle("Conteggi");
    gStyle->SetOptFit(1111);

    TCanvas* cphi = new TCanvas ("cphi", "phi", 800, 600);
    hphi->Draw();
    hphi->Fit("pol1");
    TF1 *fit = hphi->GetFunction("pol1");
    double q = fit->GetParameter(0);
    double m = fit->GetParameter(1);
    hphi->GetXaxis()->SetTitle("Angolo azimutale [rad] ");
    hphi->GetYaxis()->SetTitle("Conteggi");
    fit->SetParName(0, "q");
    fit->SetParName(1, "m");
    gStyle->SetOptFit(1111);

    //------------------------ spessore dello sciame ---------------------------------------
    TH1D* Tsciame = new TH1D("Tsciame", "T=t2-t3+t4-t5", 100, -100, 100);

    for (int i=0; i<5000; i++){
        double T_i = data[2][i]-data[3][i]+data[4][i]-data[5][i]-T21+T31-T41+T51;
        Tsciame->Fill(T_i);
    }

    TF1* gaussiano = new TF1("gaussiano", "gaus", -100, 100);
    gaussiano->SetLineColor(kBlue);

    TCanvas* cTsciame = new TCanvas("cTsciame", "T=t2-t3+t4-t5", 800, 600);
    Tsciame->Draw();
    Tsciame->Fit(gaussiano);



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