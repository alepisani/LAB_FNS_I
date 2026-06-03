double calib(double channel){
    // parametri calibrazione
    double p0 = -12.9527;
    double p1 = 0.741858;
    double p2 = 1.81343e-05;
    return channel*channel*p2 + channel*p1 + p0;
}

void energy_res(){

    const int npoints=6;

    double ene[] = {122, 511, 662, 1173, 1275, 1333}; // energie in keV
    double ch[] = {181, 696, 890, 1541, 1668, 1741}; //centroidi in ch
    double e_ch[] = {1, 2, 1, 4, 4, 4};
    double sigma_ch[] = {7.19, 22.44, 25.68, 32.33, 35.3, 34.79};
    //double e_sigma_ch[] = {0.38, 0.18, 0.06, 0.3, 0.6, 0.29};
    double e_sigma_ch[] = {1, 1, 1, 1, 1, 1};
    double res[npoints], e_res[npoints], inv_sq_ene[npoints];
    double sigma[npoints], e_sigma[npoints];



    

    for(int i=0; i<npoints; i++){
        inv_sq_ene[i] = 1/TMath::Sqrt(ene[i]);   // 1/ene, lineare con res
        double chup = ch[i] + sigma_ch[i];
        double chlow = ch[i] - sigma_ch[i];
        sigma[i] =  calib(chup) - calib(chlow);
        e_sigma[i] = calib(ch[i]+e_sigma_ch[i]) - calib(ch[i]-e_sigma_ch[i]);

        double conv = 100*2.355;  // passa da sigma a 100*FWHM

        res[i] = conv*sigma[i]/ch[i];  // risoluzione, res = FWHM/E, in %
        // errore su res = somma in quadratura degli errori relativi * res
        e_res[i] = TMath::Sqrt( TMath::Power(e_ch[i]/ch[i], 2) + TMath::Power(e_sigma[i]/sigma[i], 2))*res[i];
    }

    // plot di risoluzione in funzione dell'energia
    TGraphErrors* gR = new TGraphErrors(npoints, ene, res, 0, e_res);
    gR->SetTitle("Risoluzione energetica in funzione dell'energia");
    gR->GetXaxis()->SetTitle("Energia [keV]");
    gR->GetYaxis()->SetTitle("Risoluzione (FWHM/centroide)");
    gR->SetMarkerStyle(20);
    gR->SetMarkerSize(1);
    gR->SetMarkerColor(2);

    TCanvas* cR = new TCanvas("cR", "cR", 1200, 800);
    cR->cd();
    gR->Draw("APE");
    cR->Print("../../plots/calib_energia/energyres_vs_energy.pdf");

    // plot di risoluzione in funzione di 1/sqrt(E)
    TGraphErrors* gS = new TGraphErrors(npoints, inv_sq_ene, res, 0, e_res);
    gS->SetTitle("Risoluzione energetica in funzione di 1/#sqrt{E}");
    gS->GetXaxis()->SetTitle("1/#sqrt{E} [keV^{-1/2}]");
    gS->GetYaxis()->SetTitle("Risoluzione (FWHM/centroide)");
    gS->SetMarkerStyle(20);
    gS->SetMarkerSize(1);
    gS->SetMarkerColor(2);

    gS->Fit("pol1", "R", "", 0, 0.5);

    TCanvas* cS = new TCanvas("cS", "cS", 1200, 800);
    cS->cd();
    gS->Draw("APE");
    cS->Print("../../plots/calib_energia/energyres_vs_invsqrtene.pdf");



}
