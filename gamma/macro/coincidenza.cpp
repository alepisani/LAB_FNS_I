

void coincidenza(){

    const int nentries = 24;
    double angle[nentries] = {-32, -30, -28, -26, -24, -22, -20, -18, -16, -14, -10, -5, 0, 5, 10, 14, 18, 20, 22, 24, 26, 28, 30, 32};
    double rate[nentries]  = {0.26, 0.25, 0.33, 0.31, 0.53, 1.34, 3.14, 5.76, 9.89, 13.41, 21.88, 34.65, 43.85, 39.45, 29.12, 18.85, 9.87, 6.52, 3.42, 1.54, 0.61, 0.31, 0.25, 0.26};
    double err_angle[nentries] = {};
    double err_rate[nentries] = {};  
    
    for(int i = 0; i < nentries; i++){
    
        err_angle[i] = 0.5; //errore associato alla misura dell'angolo, 1 grado
    
    }
    
    TGraphErrors *tg = new TGraphErrors(nentries, angle, rate, err_angle, 0);
    
    tg->SetTitle("Back-to-back #gamma coincidence;Angle [deg];Rate [Hz]");
    tg->SetMarkerStyle(20);
    tg->SetMarkerColor(kBlue);
    tg->Fit("gaus");
    
    TCanvas *c1 = new TCanvas("c1", "rate_angle", 800, 600);
    c1->SetGrid();
    
    gStyle->SetOptFit(1111);
    tg->Draw("AP");  

}


    
    

