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
  
    
    //calibrazione rispetto al 9
    for (int i=0; i<1000; i++) {
        deltaT19->Fill(dati1[1][i]-dati1[9][i]);
        deltaT29->Fill(dati2[2][i]-dati2[9][i]);
        deltaT39->Fill(dati3[3][i]-dati3[9][i]);
        deltaT49->Fill(dati4[4][i]-dati4[9][i]);
        deltaT59->Fill(dati5[5][i]-dati5[9][i]);

    }

    //------------------ offset rispetto al 9 -----------------

    
    TF1* solo_gauss9 = new TF1("solo_gauss9", "gaus", 0, 200);
    solo_gauss9->SetParameter(0, 20);
    solo_gauss9->SetParameter(1, 90);
    solo_gauss9->SetParameter(2, 15);
    solo_gauss9->SetLineColor(kBlue);
    
    TCanvas* c1 = new TCanvas("c1", "offset_19", 800, 600);
    deltaT19->Draw();
    deltaT19->Fit(solo_gauss9, "RQ");
    gStyle->SetOptFit(1111);
    deltaT19->GetXaxis()->SetTitle("Canali TDC");
    deltaT19->GetYaxis()->SetTitle("Conteggi");
    double T19 = solo_gauss9->GetParameter(1);
    //deltaT19->SaveAs("../plots/offset_19.png");
    
    TCanvas* c2 = new TCanvas("c2", "offset_29", 800, 600);
    deltaT29->Draw();
    deltaT29->Fit(solo_gauss9, "RQ");
    gStyle->SetOptFit(1111);
    deltaT29->GetXaxis()->SetTitle("Canali TDC");
    deltaT29->GetYaxis()->SetTitle("Conteggi");
    double T29 = solo_gauss9->GetParameter(1);
    //deltaT29->SaveAs("../plots/offset_29.png");

    TCanvas* c3 = new TCanvas("c3", "offset_39", 800, 600);
    deltaT39->Draw();
    deltaT39->Fit(solo_gauss9, "RQ");
    gStyle->SetOptFit(1111);
    deltaT39->GetXaxis()->SetTitle("Canali TDC");
    deltaT39->GetYaxis()->SetTitle("Conteggi");
    double T39 = solo_gauss9->GetParameter(1);
    //deltaT39->SaveAs("../plots/offset_39.png");

    TCanvas* c4 = new TCanvas("c4", "offset_49", 800, 600);
    //deltaT49->Draw();
    deltaT49->Fit(solo_gauss9, "RQ");
    gStyle->SetOptFit(1111);
    deltaT49->GetXaxis()->SetTitle("Canali TDC");
    deltaT49->GetYaxis()->SetTitle("Conteggi");
    double T49 = solo_gauss9->GetParameter(1);
    //deltaT49->SaveAs("../plots/offset_49.png");

    TCanvas* c5 = new TCanvas("c5", "offset_59", 800, 600);
    deltaT59->Draw();
    deltaT59->Fit(solo_gauss9, "RQ");
    gStyle->SetOptFit(1111);
    deltaT59->GetXaxis()->SetTitle("Canali TDC");
    deltaT59->GetYaxis()->SetTitle("Conteggi");
    double T59 = solo_gauss9->GetParameter(1);
    //deltaT59->SaveAs("../plots/offset_59.png");

    //cout << "offset rispetto al 9" << "\n" << T19 << "\n" << T29 << "\n" << T39 << "\n" << T49 << "\n"<< T59 << endl; 

    
    //----------- ritardi catene elettroniche ------------------
    double T21 = T29-T19; 
    double T31 = T39-T19;
    double T41 = T49-T19;
    double T51 = T59-T19;
    //cout << "ritardi delle catene elettroniche" << "\n" << T21 << "\n" << T31 << "\n" << T41 << "\n" << T51 << endl; 
    

    //--------------------- direzioni d'arrivo --------------------------------

    vector<vector<double>> data = doubleReader("../data/quintupla_lunga/TDC_quintupla_5000.dat", 0);
    vector<double> t24_9; 
    vector<double> t35_9;
    vector<double> t21_9;
    vector<double> t31_9;
    vector<double> t41_9; 
    vector<double> t51_9;
    
    vector<double> t24_1; 
    vector<double> t35_1;
    vector<double> t21_1;
    vector<double> t31_1;
    vector<double> t41_1; 
    vector<double> t51_1;

 //------------------------- offset solo rispetto all'1 ------------------

 TH1D* TDC21 = new TH1D("TDC2-TDC1", "TDC2-TDC1", 150, -300, 300);
 TH1D* TDC31 = new TH1D("TDC3-TDC1", "TDC3-TDC1", 150, -300, 300);
 TH1D* TDC41 = new TH1D("TDC4-TDC1", "TDC4-TDC1", 150, -300, 300);
 TH1D* TDC51 = new TH1D("TDC5-TDC1", "TDC5-TDC1", 150, -300, 300);

for (int i=0; i<data[0].size(); i++) {
    TDC21->Fill(data[2][i]-data[1][i]);
    TDC31->Fill(data[3][i]-data[1][i]);
    TDC41->Fill(data[4][i]-data[1][i]);
    TDC51->Fill(data[5][i]-data[1][i]);
}

    TF1* gaussiano = new TF1("gaussiano", "gaus", -100, 100);
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


    ///------------vediamo i dati delle quintuple-----------------------
    TH1D* TDC1 = new TH1D("TDC1", "Spettro di TDC di coincidenze quintuple (CH1)", 150, 0., 500);
    TH1D* TDC2 = new TH1D("TDC2", "Spettro di TDC di coincidenze quintuple (CH2)", 150, 0., 500);
    TH1D* TDC3 = new TH1D("TDC3", "Spettro di TDC di coincidenze quintuple (CH3)", 150, 0., 500);
    TH1D* TDC4 = new TH1D("TDC4", "Spettro di TDC di coincidenze quintuple (CH4)", 150, 0., 500);
    TH1D* TDC5 = new TH1D("TDC5", "Spettro di TDC di coincidenze quintuple (CH5)", 150, 0., 500);

    for (int i=0; i<data[0].size(); i++){
        t24_9.push_back((data[2][i]-data[4][i]-T21+T41)/4.15); //da ch a ns
        t35_9.push_back((data[3][i]-data[5][i]-T31+T51)/4.15);
        t24_1.push_back((data[2][i]-data[4][i]-T2_T1+T4_T1)/4.15);
        t35_1.push_back((data[3][i]-data[5][i]-T3_T1+T5_T1)/4.15);
        
        TDC1->Fill(data[1][i]);
        TDC2->Fill(data[2][i]);
        TDC3->Fill(data[3][i]);
        TDC4->Fill(data[4][i]);
        TDC5->Fill(data[5][i]);
    }

    TCanvas* tdc1 = new TCanvas("tdc1", "TDC1-quintuple", 800, 600);
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
    TDC5->GetYaxis()->SetTitle("Conteggi");

    //----------- calcolo A, B, theta e phi -----------------

    cout<<"VALORI DI T24 E T35 rispetto 9"<<t24_9[10]<<"\n"<<t24_9[20]<<"\n"<<t35_9[10]<<endl;
    cout<<"VALORI DI T24 E T35 rispetto 1"<<t24_1[10]<<"\n"<<t24_1[20]<<"\n"<<t35_1[10]<<endl;

    vector<double> A_9;
    vector<double> B_9;
    vector<double> A_1;
    vector<double> B_1;

    for(int i=0; i<5000; i++){
        A_9.push_back((3./20.41)*((t24_9[i]+t35_9[i])/10));  // diviso 10 perchè c in s e tempo in ns
        B_9.push_back((3./21.62)*((t24_9[i]-t35_9[i])/10));
        A_1.push_back((3./20.41)*((t24_1[i]+t35_1[i])/10));  // diviso 10 perchè c in s e tempo in ns
        B_1.push_back((3./21.62)*((t24_1[i]-t35_1[i])/10));
        if(i%20){
            //cout<<"valori di A e B"<<A[i]<<" - "<<B[i]<<endl;
        }
    }

    
    
    vector<double> theta_9; 
    vector<double> phi_9;
    vector<double> theta_1; 
    vector<double> phi_1;
    
    TH1D* htheta_9 = new TH1D("theta9", "distribuzione zenitale",80, -10, 91);
    //htheta->GetYaxis()->SetRangeUser(0, 250);
    TH1D* hphi_9 = new TH1D("phi9", "distribuzione azimutale", 50, 0, 6.3);
    hphi_9->GetYaxis()->SetRangeUser(55,180);

    TH1D* htheta_1 = new TH1D("theta1", "distribuzione zenitale",80, -10, 91);
    //htheta->GetYaxis()->SetRangeUser(0, 250);
    TH1D* hphi_1 = new TH1D("phi1", "distribuzione azimutale", 50, 0, 6.3);
    hphi_1->GetYaxis()->SetRangeUser(55,180);

    for(int i=0; i<5000; i++){
        theta_9.push_back((asin(sqrt(A_9[i]*A_9[i]+B_9[i]*B_9[i])))*180/M_PI);
        theta_1.push_back((asin(sqrt(A_1[i]*A_1[i]+B_1[i]*B_1[i])))*180/M_PI);
        //phi.push_back(atan(B[i]/A[i]));
        if(A_9[i]*A_9[i]+B_9[i]*B_9[i]<1){   //controllo per evitare il rumore, se c'è qualcosa
            htheta_9->Fill(asin(sqrt(A_9[i]*A_9[i]+B_9[i]*B_9[i]))*180/M_PI);
            //hphi->Fill(atan(B[i]/A[i]));
            double phi_i_9 = (atan2(B_9[i], A_9[i]));
            if(phi_i_9 < 0) phi_i_9 += 2*M_PI;
            hphi_9->Fill(phi_i_9);
        }

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

    TCanvas* ctheta_9 = new TCanvas ("ctheta9", "theta", 800, 600);
    htheta_9->Draw();  //"E1" nella parentesi se vogliamo punti e non bin
    htheta_9->Fit(f_theta, "R");
    htheta_9->GetXaxis()->SetTitle("Angolo zenitale [gradi]");
    htheta_9->GetXaxis()->SetRangeUser(0, 90);
    htheta_9->GetYaxis()->SetTitle("Conteggi");
    gStyle->SetOptFit(1111);

    TCanvas* ctheta_1 = new TCanvas ("ctheta1", "theta", 800, 600);
    htheta_1->Draw();  //"E1" nella parentesi se vogliamo punti e non bin
    htheta_1->Fit(f_theta, "R");
    htheta_1->GetXaxis()->SetTitle("Angolo zenitale [gradi]");
    htheta_1->GetXaxis()->SetRangeUser(0, 90);
    htheta_1->GetYaxis()->SetTitle("Conteggi");
    gStyle->SetOptFit(1111);

    TCanvas* cphi_9 = new TCanvas ("cphi9", "phi", 800, 600);
    hphi_9->Draw();
    hphi_9->Fit("pol1");
    TF1 *fit9 = hphi_9->GetFunction("pol1");
    fit9->SetParameter(1,0);
    double q9 = fit9->GetParameter(0);
    double m9 = fit9->GetParameter(1);
    hphi_9->GetXaxis()->SetTitle("Angolo azimutale [rad] ");
    hphi_9->GetYaxis()->SetTitle("Conteggi");
    fit9->SetParName(0, "q9");
    fit9->SetParName(1, "m9");
    //cout<<"p-value "<<fit->GetProb()<<endl;
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
    TH1D* Tsciame9 = new TH1D("Tsciame9", "T=t2-t3+t4-t5", 120, -60, 60);
    TH1D* Tsciame1 = new TH1D("Tsciame1", "T=t2-t3+t4-t5", 120, -60, 60);

    for (int i=0; i<5000; i++){
        double T_i9 = (data[2][i]-data[3][i]+data[4][i]-data[5][i]-T21+T31-T41+T51)/4.15;
        double T_i1 = (data[2][i]-data[3][i]+data[4][i]-data[5][i]-T2_T1+T3_T1-T4_T1+T5_T1)/4.15;
        Tsciame9->Fill(T_i9);
        Tsciame1->Fill(T_i1);
    }

    int    bin_max9  = Tsciame9->GetMaximumBin();
    double max_val9  = Tsciame9->GetMaximum();
    double half_max9 = max_val9 / 2.0;

    // Scansiona a sinistra e a destra del massimo
    int bin_left9  = bin_max9;
    int bin_right9 = bin_max9;

    while (Tsciame9->GetBinContent(bin_left9)  > half_max9 && bin_left9  > 1) bin_left9--;
    while (Tsciame9->GetBinContent(bin_right9) > half_max9 && bin_right9 < Tsciame9->GetNbinsX()) bin_right9++;

    double x_left9  = Tsciame9->GetBinCenter(bin_left9);
    double x_right9 = Tsciame9->GetBinCenter(bin_right9);
    double fwhm9    = x_right9 - x_left9;

    std::cout << "xright, xleft9 = " << x_right9 << "   " << x_left9 << std::endl;
    std::cout << "FWHM9 = " << fwhm9 << std::endl;
    cout << "std dev9 " << Tsciame9->GetRMS() << " +- " << Tsciame9->GetRMSError() << endl;

    TCanvas* cTsciame9 = new TCanvas("cTsciame9", "T=t2-t3+t4-t5", 800, 600);
    Tsciame9->GetXaxis()->SetTitle("T [ns]");
    Tsciame9->GetYaxis()->SetTitle("Conteggi");
    Tsciame9->Draw();
    Tsciame9->Fit(gaussiano);
    cTsciame9->Update();
    cTsciame9->SaveAs("../plots/spessore_sciame9.pdf");

//------------------------

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
    cTsciame1->SaveAs("../plots/spessore_sciame1.pdf");







    //---------------------- fit per gli offset -----------------------------------------
    
    /*TFile *input_file = TFile::Open("../data/doppie/TDC_doppia1_1000.root");
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
