double calib(double channel){
    // parametri calibrazione
    double p0 = -12.9527;
    double p1 = 0.741858;
    double p2 = 1.81343e-05;
    return channel*channel*p2 + channel*p1 + p0;
}

void energy_res(){

    gStyle->SetOptFit(1111);

    bool use_ch = false;

    const int npoints=5;

    double ene[] = {511, 662, 1173, 1275, 1333}; // energie in keV
    double ch[] = {696, 890, 1541, 1668, 1741}; //centroidi in ch
    double e_ch[] = {1, 1, 1, 1, 1};
    //double e_ch[] = {2, 1, 4, 4, 4};
    double sigma_ch[] = {22.44, 25.68, 32.33, 35.3, 34.79};
    //double e_sigma_ch[] = {0.18, 0.06, 0.3, 0.6, 0.29};
    double e_sigma_ch[] = {1, 1, 1, 1, 1};
    double res[npoints], e_res[npoints], inv_sq_ene[npoints];
    double sigma[npoints], e_sigma[npoints];



    

    for(int i=0; i<npoints; i++){
        inv_sq_ene[i] = 1/TMath::Sqrt(ene[i]);   // 1/ene, lineare con res
        double chup = ch[i] + sigma_ch[i];
        double chlow = ch[i] - sigma_ch[i];
        sigma[i] =  calib(chup) - calib(chlow);
        e_sigma[i] = calib(ch[i]+e_sigma_ch[i]) - calib(ch[i]-e_sigma_ch[i]);

        double conv = 100;  // percentuale

        if(use_ch) res[i] = conv*sigma_ch[i]/ch[i];  // risoluzione, res = FWHM/E, in %
        else res[i] = conv*sigma[i]/ene[i];
        // errore su res = somma in quadratura degli errori relativi * res
        //e_res[i] = TMath::Sqrt( TMath::Power(e_ch[i]/ch[i], 2) + TMath::Power(e_sigma_ch[i]/sigma_ch[i], 2))*res[i];
        if(use_ch) e_res[i] = conv * TMath::Sqrt( TMath::Power(e_ch[i]*sigma_ch[i]/(ch[i]*ch[i]), 2) + TMath::Power(e_sigma_ch[i]/ch[i], 2));
        else e_res[i] = conv * e_sigma[i]/ene[i];
    }

    // plot di risoluzione in funzione dell'energia
    TGraphErrors* gR = new TGraphErrors(npoints, ene, res, 0, e_res);
    gR->SetTitle("Risoluzione energetica in funzione dell'energia");
    gR->GetXaxis()->SetTitle("Energia [keV]");
    gR->GetYaxis()->SetTitle("Risoluzione (#sigma_{E}/E) %");
    gR->SetMarkerStyle(20);
    gR->SetMarkerSize(1);
    gR->SetMarkerColor(kBlue);

    TF1* inverse = new TF1("inverse", "[0] + [1]/sqrt(x)", 500, 1400);

    gR->Fit(inverse, "R", "", 500, 140);

    TCanvas* cR = new TCanvas("cR", "cR", 1200, 800);
    cR->cd();
    gR->Draw("APE");
    cR->Print("../../plots/calib_energia/energyres_vs_energy.pdf");

    // plot di risoluzione in funzione di 1/sqrt(E)
    TGraphErrors* gS = new TGraphErrors(npoints, inv_sq_ene, res, 0, e_res);
    gS->SetTitle("Risoluzione energetica in funzione di 1/#sqrt{E}");
    gS->GetXaxis()->SetTitle("1/#sqrt{E} [keV^{-1/2}]");
    gS->GetYaxis()->SetTitle("Risoluzione (#sigma_{E}/E) %");
    gS->SetMarkerStyle(20);
    gS->SetMarkerSize(1);
    gS->SetMarkerColor(kBlue);

    TF1* linear = new TF1("linear", "[0] + [1]*x", 0.02, 0.06);

    gS->Fit(linear, "R", "", 0.02, 0.06);

    TCanvas* cS = new TCanvas("cS", "cS", 1200, 800);
    cS->cd();
    gS->Draw("APE");
    cS->Print("../../plots/calib_energia/energyres_vs_invsqrtene.pdf");



}
