#include "txt_reader.cpp"
#include "TH1D.h"

void led_gain_resolution(){
    
    
    //-------------------GAIN------------------------

    /**
     * dataset to use:
     *  data/txt/704V_30dB_30ua_histo.txt
     *  data/txt/708V_30dB_30ua_bis_pp_histo.txt
     *  data/txt/710V_30dB_30ua_pp_histo.txt
     *  data/txt/712V_30dB_30ua_histo.txt
     * 
     * for each file.txt:
     *  - make the hist
     *  - find the mean of each peak with a gaussian fit
     *  - find the average Delta peak-peak with a weighted mean
     *  - with this Delta value compute the Gain_sensor
     *  - with the 4 Gain_sensor values make a Gain vs VBias plot with a linear fit
    */

    //fill the data
    vector<vector<double>> data_704 = txt_reader("../data/txt/704V_30dB_30ua_histo.txt");
    vector<vector<double>> data_708 = txt_reader("../data/txt/708V_30dB_30ua_bis_pp_histo.txt");
    vector<vector<double>> data_710 = txt_reader("../data/txt/710V_30dB_30ua_pp_histo.txt");
    vector<vector<double>> data_712 = txt_reader("../data/txt/712V_30dB_30ua_histo.txt");

    //fill the histo with the data
    int nbins_704 = data_704[0].size();
    TH1D* hist_704 = new TH1D("led704", "LED spectrum", nbins_704, -1000.5, 23004.5);
    for (int i = 0; i < (int)data_704[0].size(); i++){

        hist_704->SetBinContent(i + 1, data_704[1][i]);

    }
        
    int nbins_708 = data_708[0].size();
    TH1D* hist_708 = new TH1D("led708", "LED spectrum", nbins_708, -1000.5, 23004.5);
    for (int i = 0; i < (int)data_708[0].size(); i++){

        hist_708->SetBinContent(i + 1, data_708[1][i]);

    }

    int nbins_710 = data_710[0].size();
    TH1D* hist_710 = new TH1D("led710", "LED spectrum", nbins_710, -1000.5, 23004.5);
    for (int i = 0; i < (int)data_710[0].size(); i++){

        hist_710->SetBinContent(i + 1, data_710[1][i]);

    }

    int nbins_712 = data_712[0].size();
    TH1D* hist_712 = new TH1D("led712", "LED spectrum", nbins_712, -1000.5, 23004.5);
    for (int i = 0; i < (int)data_712[0].size(); i++){

        hist_712->SetBinContent(i + 1, data_712[1][i]);

    }

    //create plot on canvas
    TCanvas* can_704 = new TCanvas("can704", "can704", 800, 600);
    hist_704->GetXaxis()->SetTitle("Canali ADC");
    hist_704->GetYaxis()->SetTitle("Conteggi");
    hist_704->SetTitle("Spettro Digitizer - 70.4V");
    hist_704->GetXaxis()->SetRangeUser(-150, 1700);
    hist_704->Draw();
    
    TCanvas* can_708 = new TCanvas("can708", "can708", 800, 600);
    hist_708->GetXaxis()->SetTitle("Canali ADC");
    hist_708->GetYaxis()->SetTitle("Conteggi");
    hist_708->SetTitle("Spettro Digitizer - 70.8V");
    hist_708->GetXaxis()->SetRangeUser(-150, 3000);
    hist_708->Draw();
    
    TCanvas* can_710 = new TCanvas("can710", "can710", 800, 600);
    hist_710->GetXaxis()->SetTitle("Canali ADC");
    hist_710->GetYaxis()->SetTitle("Conteggi");
    hist_710->SetTitle("Spettro Digitizer - 71.0V");
    hist_710->GetXaxis()->SetRangeUser(-150, 3500);
    hist_710->Draw();
    
    TCanvas* can_712 = new TCanvas("can712", "can712", 800, 600);
    hist_712->GetXaxis()->SetTitle("Canali ADC");
    hist_712->GetYaxis()->SetTitle("Conteggi");
    hist_712->SetTitle("Spettro Digitizer - 71.2V");
    hist_712->GetXaxis()->SetRangeUser(-150, 4000);
    hist_712->Draw();
    
    
    TCanvas* can_all = new TCanvas("can_all", "Spettri LED", 1400, 1200);
    can_all->Divide(2, 2); 

    can_all->cd(1);
    hist_704->GetXaxis()->SetTitle("Canali ADC");
    hist_704->GetYaxis()->SetTitle("Conteggi");
    hist_704->SetTitle("Spettro Digitizer - 70.4V");
    hist_704->GetXaxis()->SetRangeUser(-150, 1700);
    hist_704->Draw();

    can_all->cd(2);
    hist_708->GetXaxis()->SetTitle("Canali ADC");
    hist_708->GetYaxis()->SetTitle("Conteggi");
    hist_708->SetTitle("Spettro Digitizer - 70.8V");
    hist_708->GetXaxis()->SetRangeUser(-150, 3000);
    hist_708->Draw();

    can_all->cd(3);
    hist_710->GetXaxis()->SetTitle("Canali ADC");
    hist_710->GetYaxis()->SetTitle("Conteggi");
    hist_710->SetTitle("Spettro Digitizer - 71.0V");
    hist_710->GetXaxis()->SetRangeUser(-150, 3500);
    hist_710->Draw();

    can_all->cd(4);
    hist_712->GetXaxis()->SetTitle("Canali ADC");
    hist_712->GetYaxis()->SetTitle("Conteggi");
    hist_712->SetTitle("Spettro Digitizer - 71.2V");
    hist_712->GetXaxis()->SetRangeUser(-150, 4000);
    hist_712->Draw();


    //fit gaussiani Vbias = 70.4V
    int nGauss_704 = 6;
    vector<TF1*> gaussians_704;
    double sigma_704 = 50;
    vector<double> centers_704 = {0, 180, 350, 520, 680, 860};
    vector<double> mean_704;
    vector<double> err_mean_704;
    vector<double> sigma_704_v;
    vector<double> variance_704;
    vector<double> err_variance_704;

    for (int i = 0; i < nGauss_704; i++) {
        
        TString gname = Form("g_%d", i);
        TF1* g = new TF1(gname, "gaus(0)", centers_704[i] - sigma_704, centers_704[i] + sigma_704);
        gaussians_704.push_back(g);
        if(i == 0){
            hist_704->Fit(gaussians_704[0], "RQ");
            mean_704.push_back(gaussians_704[i]->GetParameter(1));   
            err_mean_704.push_back(gaussians_704[i]->GetParError(1));
            double sigma    = gaussians_704[i]->GetParameter(2);
            sigma_704_v.push_back(sigma);
            double err_sigma = gaussians_704[i]->GetParError(2);
            double variance = sigma * sigma;
            variance_704.push_back(variance);
            err_variance_704.push_back(2 * sigma * err_sigma);
        }
        else{
            hist_704->Fit(gaussians_704[i], "RQ+");
            mean_704.push_back(gaussians_704[i]->GetParameter(1));   
            err_mean_704.push_back(gaussians_704[i]->GetParError(1));
            double sigma    = gaussians_704[i]->GetParameter(2);
            sigma_704_v.push_back(sigma);
            double err_sigma = gaussians_704[i]->GetParError(2);
            double variance = sigma * sigma;
            variance_704.push_back(variance);
            err_variance_704.push_back(2 * sigma * err_sigma);
        }
        
    }
    //saving the plot
    can_704->Update();
    can_704->SaveAs("../plots/LED/spettri_704_gain.pdf");
    
    //fit gaussiani Vbias = 70.8V
    int nGauss_708 = 8;
    vector<TF1*> gaussians_708;
    double sigma_708 = 70;
    vector<double> centers_708 = {0, 300, 580, 870, 1160, 1440, 1720, 2000};
    vector<double> mean_708;
    vector<double> err_mean_708;
    vector<double> sigma_708_v;
    vector<double> variance_708;
    vector<double> err_variance_708;

    for (int i = 0; i < nGauss_708; i++) {
        
        TString gname = Form("g_%d", i);
        TF1* g = new TF1(gname, "gaus(0)", centers_708[i] - sigma_708, centers_708[i] + sigma_708);
        gaussians_708.push_back(g);
        if(i == 0){
            hist_708->Fit(gaussians_708[0], "RQ");
            mean_708.push_back(gaussians_708[i]->GetParameter(1));   
            err_mean_708.push_back(gaussians_708[i]->GetParError(1));
            double sigma    = gaussians_708[i]->GetParameter(2);
            sigma_708_v.push_back(sigma);
            double err_sigma = gaussians_708[i]->GetParError(2);
            double variance = sigma * sigma;
            variance_708.push_back(variance);
            err_variance_708.push_back(2 * sigma * err_sigma);

        }
        else{
            hist_708->Fit(gaussians_708[i], "RQ+");
            mean_708.push_back(gaussians_708[i]->GetParameter(1));   
            err_mean_708.push_back(gaussians_708[i]->GetParError(1));
            double sigma    = gaussians_708[i]->GetParameter(2);
            sigma_708_v.push_back(sigma);
            double err_sigma = gaussians_708[i]->GetParError(2);
            double variance = sigma * sigma;
            variance_708.push_back(variance);
            err_variance_708.push_back(2 * sigma * err_sigma);
        }
        
    }
    //saving the plot
    can_708->Update();
    can_708->SaveAs("../plots/LED/spettri_708_gain.pdf");
        
    //fit gaussiani Vbias = 71.0V
    int nGauss_710 = 8;
    vector<TF1*> gaussians_710;
    double sigma_710 = 70;
    vector<double> centers_710 = {0, 350, 700, 1050, 1390, 1720, 2070, 2410};
    vector<double> mean_710;
    vector<double> err_mean_710;
    vector<double> sigma_710_v;
    vector<double> variance_710;
    vector<double> err_variance_710;

    for (int i = 0; i < nGauss_710; i++) {
        
        TString gname = Form("g_%d", i);
        TF1* g = new TF1(gname, "gaus(0)", centers_710[i] - sigma_710, centers_710[i] + sigma_710);
        gaussians_710.push_back(g);
        if(i == 0){
            hist_710->Fit(gaussians_710[0], "RQ");
            mean_710.push_back(gaussians_710[i]->GetParameter(1));   
            err_mean_710.push_back(gaussians_710[i]->GetParError(1));
            double sigma    = gaussians_710[i]->GetParameter(2);
            sigma_710_v.push_back(sigma);
            double err_sigma = gaussians_710[i]->GetParError(2);
            double variance = sigma * sigma;
            variance_710.push_back(variance);
            err_variance_710.push_back(2 * sigma * err_sigma);
        }
        else{
            hist_710->Fit(gaussians_710[i], "RQ+");
            mean_710.push_back(gaussians_710[i]->GetParameter(1));   
            err_mean_710.push_back(gaussians_710[i]->GetParError(1));
            double sigma    = gaussians_710[i]->GetParameter(2);
            sigma_710_v.push_back(sigma);
            double err_sigma = gaussians_710[i]->GetParError(2);
            double variance = sigma * sigma;
            variance_710.push_back(variance);
            err_variance_710.push_back(2 * sigma * err_sigma);
        }
        
    }
    //saving the plot
    can_710->Update();
    can_710->SaveAs("../plots/LED/spettri_710_gain.pdf");

    //fit gaussiani Vbias = 71.2V
    int nGauss_712 = 9;
    vector<TF1*> gaussians_712;
    double sigma_712 = 90;
    vector<double> centers_712 = {0, 420, 820, 1220, 1620, 2020, 2400, 2820, 3200};
    vector<double> mean_712;
    vector<double> err_mean_712;
    vector<double> sigma_712_v;
    vector<double> variance_712;
    vector<double> err_variance_712;

    for (int i = 0; i < nGauss_712; i++) {
        
        TString gname = Form("g_%d", i);
        TF1* g = new TF1(gname, "gaus(0)", centers_712[i] - sigma_712, centers_712[i] + sigma_712);
        gaussians_712.push_back(g);
        if(i == 0){
            hist_712->Fit(gaussians_712[0], "RQ");
            mean_712.push_back(gaussians_712[i]->GetParameter(1));   
            err_mean_712.push_back(gaussians_712[i]->GetParError(1));
            double sigma    = gaussians_712[i]->GetParameter(2);
            sigma_712_v.push_back(sigma);
            double err_sigma = gaussians_712[i]->GetParError(2);
            double variance = sigma * sigma;
            variance_712.push_back(variance);
            err_variance_712.push_back(2 * sigma * err_sigma);
        }
        else{
            hist_712->Fit(gaussians_712[i], "RQ+");
            mean_712.push_back(gaussians_712[i]->GetParameter(1));   
            err_mean_712.push_back(gaussians_712[i]->GetParError(1));
            double sigma    = gaussians_712[i]->GetParameter(2);
            sigma_712_v.push_back(sigma);
            double err_sigma = gaussians_712[i]->GetParError(2);
            double variance = sigma * sigma;
            variance_712.push_back(variance);
            err_variance_712.push_back(2 * sigma * err_sigma);
        }
        
    }
    //saving the plot
    can_712->Update();
    can_712->SaveAs("../plots/LED/spettri_712_gain.pdf");

    //compute delta peak-peak value
    vector<double> mean_ddp;
    vector<double> err_mean_ddp;
    
    //70.4V
    vector<double> Dpp_704;
    for (int i = 0; i < (int)mean_704.size() - 1; i++) {
        double dpp = mean_704[i+1] - mean_704[i];
        Dpp_704.push_back(dpp);
    }
    double mean_dpp_704 = accumulate(Dpp_704.begin(), Dpp_704.end(), 0.0) / Dpp_704.size();
    mean_ddp.push_back(mean_dpp_704);

    // Calcolo Deviazione Standard per 70.4V
    double sum_sq_704 = 0;
    for(double d : Dpp_704) sum_sq_704 += (d - mean_dpp_704) * (d - mean_dpp_704);
    err_mean_ddp.push_back(sqrt(sum_sq_704 / (Dpp_704.size() - 1)));

    can_all->cd(1); 
    can_all->cd(2);
    can_all->cd(3);
    can_all->cd(4);
    can_all->Update();
    can_all->SaveAs("../plots/LED/spettri_all_gain.pdf");

    //70.8V
    vector<double> Dpp_708;
    for (int i = 0; i < (int)mean_708.size() - 1; i++) {
        double dpp = mean_708[i+1] - mean_708[i];
        Dpp_708.push_back(dpp);
    }
    double mean_dpp_708 = accumulate(Dpp_708.begin(), Dpp_708.end(), 0.0) / Dpp_708.size();
    mean_ddp.push_back(mean_dpp_708);

    // Calcolo Deviazione Standard per 70.8V
    double sum_sq_708 = 0;
    for(double d : Dpp_708) sum_sq_708 += (d - mean_dpp_708) * (d - mean_dpp_708);
    err_mean_ddp.push_back(sqrt(sum_sq_708 / (Dpp_708.size() - 1)));


    //71.0V
    vector<double> Dpp_710;
    for (int i = 0; i < (int)mean_710.size() - 1; i++) {
        double dpp = mean_710[i+1] - mean_710[i];
        Dpp_710.push_back(dpp);
    }
    double mean_dpp_710 = accumulate(Dpp_710.begin(), Dpp_710.end(), 0.0) / Dpp_710.size();
    mean_ddp.push_back(mean_dpp_710);

    // Calcolo Deviazione Standard per 71.0V
    double sum_sq_710 = 0;
    for(double d : Dpp_710) sum_sq_710 += (d - mean_dpp_710) * (d - mean_dpp_710);
    err_mean_ddp.push_back(sqrt(sum_sq_710 / (Dpp_710.size() - 1)));


    //71.2V
    vector<double> Dpp_712;
    for (int i = 0; i < (int)mean_712.size() - 1; i++) {
        double dpp = mean_712[i+1] - mean_712[i];
        Dpp_712.push_back(dpp);
    }
    double mean_dpp_712 = accumulate(Dpp_712.begin(), Dpp_712.end(), 0.0) / Dpp_712.size();
    mean_ddp.push_back(mean_dpp_712);

    // Calcolo Deviazione Standard per 71.2V
    double sum_sq_712 = 0;
    for(double d : Dpp_712) sum_sq_712 += (d - mean_dpp_712) * (d - mean_dpp_712);
    err_mean_ddp.push_back(sqrt(sum_sq_712 / (Dpp_712.size() - 1)));


    //compute gain_sensors
    double electron_charge = 0.00016; // fC
    double conversion_factor = 1.235; // fC/Channel 
    vector<double> gain_sensor;
    vector<double> err_gain;
    for(int i = 0; i < 4; i++){
        gain_sensor.push_back((mean_ddp[i]*conversion_factor)/(electron_charge));
        err_gain.push_back((err_mean_ddp[i]*conversion_factor)/(electron_charge)); 
    }
    vector<double> Vbias = {70.4, 70.8, 71.0, 71.2}; // Volt
    vector<double> err_Vbias = {0.0, 0.0, 0.0, 0.0}; 

    //linear fit gain vs vbias
    gStyle->SetOptFit(1111);
    
    TGraphErrors *graph_gain_v = new TGraphErrors(4, Vbias.data(), gain_sensor.data(), err_Vbias.data(), err_gain.data());
    graph_gain_v->GetXaxis()->SetTitle("V_{bias} [V]");
    graph_gain_v->GetYaxis()->SetTitle("Gain_{sensor}");
    graph_gain_v->SetTitle("Gain_{sensor} at 30dB Vs V_{bias}");
    
    TCanvas* c_gain_v = new TCanvas ("gain_v","Gain_Vbias", 800, 600);
    TF1 *l1 = new TF1("l1","pol1", 70., 72.);
    l1->SetLineColor(kRed);
    l1->SetLineWidth(2);
  
    graph_gain_v->Fit("l1","R");
    graph_gain_v->SetMarkerStyle(20);
    graph_gain_v->SetMarkerSize(1.2);
    graph_gain_v->SetMarkerColor(kBlue);
    graph_gain_v->Draw("AP");
    c_gain_v->Update();
    c_gain_v->SaveAs("../plots/LED/gain.pdf");

    //printing the result from the gaussian fit 704
    printf("\nspettro 70.4V\n pe, mean, sigma\n");
    for(int i = 0; i < nGauss_704; i++){

        if(i == nGauss_704) printf("pe: %d;     mean: %f\n", i, mean_704[i]);
        else printf("pe: %d;     mean: %f,     dpp: %f\n", i, mean_704[i], Dpp_704[i]);

    }
    printf("mean value of dpp for 70.4V: %f\n", mean_dpp_704);

    //printing the result from the gaussian fit 708
    printf("\nspettro 70.8V\n pe, mean, sigma\n");
    for(int i = 0; i < nGauss_708; i++){

        if(i == nGauss_708) printf("pe: %d;     mean: %f\n", i, mean_708[i]);
        else printf("pe: %d;     mean: %f,     dpp: %f\n", i, mean_708[i], Dpp_708[i]);

    }
    printf("mean value of dpp for 70.8V: %f\n", mean_dpp_708);

    //printing the result from the gaussian fit 710
    printf("\nspettro 71.0V\n pe, mean, sigma\n");
    for(int i = 0; i < nGauss_710; i++){

        if(i == nGauss_710) printf("pe: %d;     mean: %f\n", i, mean_710[i]);
        else printf("pe: %d;     mean: %f,     dpp: %f\n", i, mean_710[i], Dpp_710[i]);

    }
    printf("mean value of dpp for 71.0V: %f\n", mean_dpp_710);

    //printing the result from the gaussian fit 712
    printf("\nspettro 71.2V\n pe, mean, sigma\n");
    for(int i = 0; i < nGauss_712; i++){

        if(i == nGauss_712) printf("pe: %d;     mean: %f\n", i, mean_712[i]);
        else printf("pe: %d;     mean: %f,     dpp: %f\n", i, mean_712[i], Dpp_712[i]);

    }
    printf("mean value of dpp for 71.2V: %f\n", mean_dpp_712);



    //----------------RESOLUTION---------------------
    
    /**
     * let's consider the dataset: data/txt/708V_30dB_30ua_bis_pp_histo.txt
     * estimate the variance of each gaussian fit
     * make a linear plot of Variance Vs Npe
     * eventually ompute resolution
    */

    vector<double> Npe = {0, 1, 2, 3, 4, 5, 6, 7};
    vector<double> err_Npe = {0, 0, 0, 0, 0, 0, 0};

    TGraphErrors *graph_resolution = new TGraphErrors((int)variance_708.size(), Npe.data(), variance_708.data(), err_Npe.data(), err_variance_708.data());
    graph_resolution->GetXaxis()->SetTitle("N_{p.e.}");
    graph_resolution->GetYaxis()->SetTitle("#sigma^{2}");
    graph_resolution->SetTitle("#sigma^{2} Vs N_{p.e.}");
    
    TCanvas* c_res = new TCanvas ("resolution","res", 800, 600);
    TF1 *l2 = new TF1("l2","pol1", -1, 8);
    l2->SetLineColor(kRed);
    l2->SetLineWidth(2);
  
    graph_resolution->Fit("l2","R");
    graph_resolution->SetMarkerStyle(20);
    graph_resolution->SetMarkerSize(1.2);
    graph_resolution->SetMarkerColor(kBlue);
    graph_resolution->Draw("AP");
    c_res->Update();
    c_res->SaveAs("../plots/LED/resolution.pdf");

    //---------------------Resolution Vs Vbias---------------------


    /**
     * further analysis to understand how it goes the resolution
     * against the Vbias.
    */

    cout << endl;
    double resolution_704 = (mean_dpp_704) / (sqrt(variance_704[1] - variance_704[0]));
    double delta_var_704 = variance_704[1] - variance_704[0];
    double err_delta_var_704 = sqrt(err_variance_704[1]*err_variance_704[1] + err_variance_704[0]*err_variance_704[0]);
    double err_resolution_704 = sqrt(
        (err_mean_ddp[0]*err_mean_ddp[0]) / delta_var_704 + 
        (mean_dpp_704*mean_dpp_704 * err_delta_var_704*err_delta_var_704) / (4*delta_var_704*delta_var_704*delta_var_704)
    );
    cout << "resolution_704 = " << resolution_704 << " +/- " << err_resolution_704 << endl;
    
    cout << endl;
    double resolution_708 = (mean_dpp_708) / (sqrt(variance_708[1] - variance_708[0]));
    double delta_var_708 = variance_708[1] - variance_708[0];
    double err_delta_var_708 = sqrt(err_variance_708[1]*err_variance_708[1] + err_variance_708[0]*err_variance_708[0]);
    double err_resolution_708 = sqrt(
        (err_mean_ddp[1]*err_mean_ddp[1]) / delta_var_708 + 
        (mean_dpp_708*mean_dpp_708 * err_delta_var_708*err_delta_var_708) / (4*delta_var_708*delta_var_708*delta_var_708)
    );
    cout << "resolution_708 = " << resolution_708 << " +/- " << err_resolution_708 << endl;

    cout << endl;
    double resolution_710 = (mean_dpp_710) / (sqrt(variance_710[1] - variance_710[0]));
    double delta_var_710 = variance_710[1] - variance_710[0];
    double err_delta_var_710 = sqrt(err_variance_710[1]*err_variance_710[1] + err_variance_710[0]*err_variance_710[0]);
    double err_resolution_710 = sqrt(
        (err_mean_ddp[2]*err_mean_ddp[2]) / delta_var_710 + 
        (mean_dpp_710*mean_dpp_710 * err_delta_var_710*err_delta_var_710) / (4*delta_var_710*delta_var_710*delta_var_710)
    );
    cout << "resolution_710 = " << resolution_710 << " +/- " << err_resolution_710 << endl;

    cout << endl;
    double resolution_712 = (mean_dpp_712) / (sqrt(variance_712[1] - variance_712[0]));
    double delta_var_712 = variance_712[1] - variance_712[0];
    double err_delta_var_712 = sqrt(err_variance_712[1]*err_variance_712[1] + err_variance_712[0]*err_variance_712[0]);
    double err_resolution_712 = sqrt(
        (err_mean_ddp[3]*err_mean_ddp[3]) / delta_var_712 + 
        (mean_dpp_712*mean_dpp_712 * err_delta_var_712*err_delta_var_712) / (4*delta_var_712*delta_var_712*delta_var_712)
    );
    cout << "resolution_712 = " << resolution_712 << " +/- " << err_resolution_712 << endl;


    vector<double> res = {resolution_704, resolution_708, resolution_710, resolution_712};
    vector<double> err_res = {err_resolution_704, err_resolution_708, err_resolution_710, err_resolution_712};

    TGraphErrors *res_vbias = new TGraphErrors(4, Vbias.data(), res.data(), 0, err_res.data());
    res_vbias->GetXaxis()->SetTitle("V_{bias} [V]");
    res_vbias->GetYaxis()->SetTitle("Resolution");
    res_vbias->SetTitle("Resolution Vs V_{bias}");
    
    TCanvas* c_resvbias = new TCanvas ("resolutionbias","resbias", 800, 600);
  
    res_vbias->SetMarkerStyle(20);
    res_vbias->SetMarkerSize(1.2);
    res_vbias->SetMarkerColor(kBlue);
    res_vbias->Draw("APC");
    c_resvbias->Update();
    c_resvbias->SaveAs("../plots/LED/resolution_Vbias.pdf");


}