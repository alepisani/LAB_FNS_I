/**
 * macro to make the poisson + n-gaussian on led spectrums
 * goals to identify the centroids and delta peaks in order to computer resolution
 * 
 * file to use:
 * ../data/txt/704V_30dB_30ua_histo.txt
 * and similar .txt to this one
*/


#include "txt_reader.cpp"
#include "TH1D.h"
using namespace std;

// ─── Helper function ──────────────────────────────────────────────────────────
void fit_led_spectrum(
    const TString& fname,           
    const TString& canvasName,      
    double delta,
    vector<double> centers
) {
    int nGauss = centers.size();

    // --- Leggi dati ---
    TString fpath = "../data/txt/" + fname;
    vector<vector<double>> data = txt_reader(fpath.Data());

    int nbins = data[0].size();
    TH1D* hist = new TH1D(canvasName, "LED spectrum", nbins, -1000.5, 23004.5);
    for (int i = 0; i < (int)data[0].size(); i++)
        hist->SetBinContent(i + 1, data[1][i]);

    // --- Crea le TF1 ---
    vector<TF1*> gaussians;
    for (int i = 0; i < nGauss; i++) {
        TString gname = Form("g_%s_%d", canvasName.Data(), i);
        TF1* g = new TF1(gname, "gaus(0)", centers[i] - delta, centers[i] + delta);
        gaussians.push_back(g);
    }

    // --- Canvas e draw ---
    TCanvas* can = new TCanvas(canvasName + "_can", "Poisson + Gauss", 800, 600);
    hist->GetXaxis()->SetTitle("Canali");
    hist->GetYaxis()->SetTitle("Conteggi");
    hist->SetTitle(fname);
    hist->GetXaxis()->SetRangeUser(-100, 5000);
    hist->Draw();

    // --- Fit ---
    hist->Fit(gaussians[0], "RQ");
    for (int i = 1; i < nGauss; i++)
        hist->Fit(gaussians[i], "RQ+");

    // --- Estrai parametri e calcola delta picco-picco ---
    vector<double> means;
    printf("\n=== %s ===\n", fname.Data());
    for (int i = 0; i < nGauss; i++) {
        double norm      = gaussians[i]->GetParameter(0);
        double mean      = gaussians[i]->GetParameter(1);
        double sigma     = gaussians[i]->GetParameter(2);
        double norm_err  = gaussians[i]->GetParError(0);
        double mean_err  = gaussians[i]->GetParError(1);
        double sigma_err = gaussians[i]->GetParError(2);
        double chi2      = gaussians[i]->GetChisquare();
        int    ndf       = gaussians[i]->GetNDF();
        double pvalue    = TMath::Prob(chi2, ndf);

        means.push_back(mean);
        printf("Gauss%d:  mean = %.2f +/- %.2f   sigma = %.2f +/- %.2f   norm = %.2f +/- %.2f   pvalue = %.8f\n",
               i+1, mean, mean_err, sigma, sigma_err, norm, norm_err, pvalue);
    }

    // --- Delta picco-picco ---
    vector<double> Dpp;
    for (int i = 0; i < (int)means.size() - 1; i++) {
        double dpp = means[i+1] - means[i];
        Dpp.push_back(dpp);
        cout << "dpp" << i+1 << " = " << dpp << "  ";
    }
    cout << endl;

    double mean_dpp = 0;
    for (double d : Dpp) mean_dpp += d;
    mean_dpp /= Dpp.size();

    // --- Salva su file ---
    TString outname = fname;
    outname.ReplaceAll("_histo.txt", "_deltapp.txt");
    TString outpath = "../data/outcome_fit/" + outname;

    FILE* txt = fopen(outpath.Data(), "w");
    fprintf(txt, "%s\n", fname.Data());
    for (int i = 0; i < (int)Dpp.size(); i++) {
        fprintf(txt, "%.4f", Dpp[i]);
        if (i < (int)Dpp.size() - 1) fprintf(txt, " ");
    }
    fprintf(txt, "\n%.4f\n", mean_dpp);
    fclose(txt);

    printf("Media delta: %.4f  →  salvato in %s\n", mean_dpp, outpath.Data());
}

// ─── Macro principale ─────────────────────────────────────────────────────────
void led_poisson_gauss() {

    // ── File 1: 708V 30dB 30ua ──────────────────────────────────────────────
    fit_led_spectrum(
        "708V_30dB_30ua_histo.txt",
        "c1",
        80,                                     // delta
        {0, 350, 700, 1000, 1350, 1700, 2000}   // centers
    );

    // ── File 2: 708V 30dB 27ua ──────────────────────────────────────────────
    fit_led_spectrum(
        "708V_30dB_27ua_histo.txt",
        "c2",
        80,                                     // delta
        {0, 350, 700, 1000, 1350, 1700, 2000}   // centers
    );

    // ── File 3: 708V 30dB 24ua ──────────────────────────────────────────────
    fit_led_spectrum(
        "708V_30dB_24ua_histo.txt",
        "c3",
        80,                                     // delta
        {0, 350, 700, 1000, 1350, 1700, 2000}   // centers
    );
}