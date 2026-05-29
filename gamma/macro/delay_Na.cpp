
/**
 * the goal is to show in a 100s measure to see that 
 * the delay amplifier gives an extra delay on the measure 
 * in a non linear way
 * 
 * we're going to use the following spectra:
 * gamma/plots/spettri_energia/Spettro_energia_ravvicinato_delay_ampl_0deg_Na_100s.pdf
 * gamma/plots/spettri_energia/Spettro_energia_senza_delay_ampl_0deg_Na.pdf
 * 
*/

#include "TH1D.h"


void delay_Na() {

    const char* file_noDA = "../data/Spettro_energia_senza_delay_ampl_0deg_Na.mca";
    const char* file_DA = "../data/Spettro_energia_ravvicinato_delay_ampl_0deg_Na_100s.mca";
    int nchan = 2048;
    int nlines_start = 12;    
    int nlines_end = 2060;    

    TH1D* hist_noDA = new TH1D("hist_noDA", "spettro Na senza delay amp", nchan, 0, nchan);
    TH1D* hist_DA = new TH1D("hist_DA", "spettro Na con delay amp", nchan, 0, nchan);

    FILE* f_noDA = fopen(file_noDA, "r");
    FILE* f_DA = fopen(file_DA, "r");

    char line_noDA[200];
    int row_noDA = 0, ichan_noDA = 0;
    float y_noDA = 0;
    
    char line_DA[200];
    int row_DA = 0, ichan_DA = 0;
    float y_DA = 0;

    while (fgets(line_noDA, 200, f_noDA)) {

        if (row_noDA >= nlines_start && row_noDA < nlines_end) {
            if (sscanf(line_noDA, "%f", &y_noDA) == 1) {
                hist_noDA->AddBinContent(ichan_noDA + 1, y_noDA);
                ichan_noDA++;
            }
        }

        if (row_noDA >= nlines_end) break;
        row_noDA++;
    }
    fclose(f_noDA);
    
    while (fgets(line_DA, 200, f_DA)) {

        if (row_DA >= nlines_start && row_DA < nlines_end) {
            if (sscanf(line_DA, "%f", &y_DA) == 1) {
                hist_DA->AddBinContent(ichan_DA + 1, y_DA);
                ichan_DA++;
            }
        }

        if (row_DA >= nlines_end) break;
        row_DA++;
    }
    fclose(f_DA);

    TCanvas* c1 = new TCanvas("c1", "Spettro", 800, 600);

    hist_noDA->Scale(1.0 / hist_noDA->Integral());
    hist_DA->Scale(1.0 / hist_DA->Integral());

    double ymax = max(hist_noDA->GetMaximum(), hist_DA->GetMaximum());
    hist_noDA->SetMaximum(ymax * 1.1);

    hist_noDA->GetYaxis()->SetTitle("Conteggi normalizzati");

    hist_noDA->SetLineColor(kBlue+1);
    hist_noDA->Draw("HIST");
    hist_DA->SetLineColor(kRed+1);
    hist_DA->Draw("HIST SAME");

    c1->Update();
    c1->SaveAs("../plots/COIN/Na_0deg_con_senza_delay_amplifier.pdf");


}

