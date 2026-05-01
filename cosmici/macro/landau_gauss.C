#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"

/**
 * analisi dati al fine di trovare il picco di energia rilasciata dal muone all'interno dello scintillatore.
 * I dati provengono da un ADC (analog to digital converter), prende il segnale uscente dal PMT che sarà 
 * una corrente (che risulta in qualche modo proporzionale all'energia depositata nello scintillatore)
 * la quale viene mandata all'ADC e restutuisce un segnale digitale in canali. 
 * L'obiettivo è misurare il picco di energia rilasciata tenendo presente il noise intrinseco che lo 
 * ricaviamo dalla misura di piedistallo (misure di ADC su un dato scintillatore quando sto triggerando su 
 * qualcos'altro, quindi di fatto quello che vedo non è segnale fisico ma noise).
 * Per ricavare il picco si compie un fit Landau+Gauss.
 * Landau perché l'energia depositata segue una Landau e il rumore che sto acquisendo in questa data 
 * misura segue una distribuzione gaussiana.
 * Queste misure vengono fatte per il rivelatore 3-4-6 per trovare il punto di lavoro del PMT
 * I fit di Landau+Gauss vengono fatti per 1-2-3-4-5
 * 
 * NB: CAPIRE MEGLIO COME VARIA IL PLOT AL VARIARE DELLA TENSIONE E DELLA THRESHOLD!!!
 * 
 * Per l'analisi si usino i seguenti file.root:
 * scintillatore 1: ADC1_singola_HV2080_THR60_20000.root
 * scintillatore 2: ADC2_singola_HV2050_THR62_20000.root
 * scintillatore 3: ADC3_singola_HV1980_THR40_100000.root
 * scintillatore 4: ADC4_singola_HV2070_THR106_5000.root         //da capire meglio quale usare
 * scintillatore 5: ADC5_singola_HV2050_THR44_20000.root
 * scintillatore 6: ADC6_singola_HV1810_THR40_10000.root
 */


void landau_gauss(){
        
    /**
     * che valori prende la landau+gauss?
     * par0_landau = altezza della landau --> valore massimo dell'hist
     * par1_landau = posizione del picco landau --> media (non ottimale, buona approx)
     * par2_landau = larghezza --> RMS*0.5 dato che abbiamo delle code infinite
     * par3_gauss = altezza --> valore massimo dell'hist (abbiamo preso dati in modo che succedeva ciò)
     * par4_gauss = posizione picco gauss --> media (non ottimale, buona approx)
     * par5_gauss = larghezza --> RMS*0.3, da definire meglio
     */
    
    //-----------ADC-1-------------------------------------------------------------------------------------------------//
    
    //open file
    TFile *input_file1 = TFile::Open("../data/singole/ADC1_singola_HV2080_THR60_20000.root");
    
    //take the right ADC histos
    TH1D* hist1_before_rebinning = (TH1D*)input_file1->Get("adc1");
    TH1D* hist1 = (TH1D*)hist1_before_rebinning->Rebin(1, "hist1");

    //landau+gauss fit 
    int xmin1 = 0;
    int xmax1 = 500;
    TF1* langauss1 = new TF1("langauss1", "landau(0) + gaus(3)", xmin1, xmax1);
    langauss1->SetLineWidth(2);
    langauss1->SetLineColor(kRed);
    
    langauss1->SetParameter(0, hist1->GetMaximum());
    langauss1->SetParameter(1, hist1->GetMean());
    langauss1->SetParameter(2, hist1->GetRMS()*0.3);
    langauss1->SetParameter(3, hist1->GetMaximum());
    langauss1->SetParameter(4, hist1->GetMean());
    langauss1->SetParameter(5, hist1->GetRMS()*0.3);
    
    
    TCanvas *can1_langauss = new TCanvas("c1_langauss", "Landau + Gauss", 800, 600);
    hist1->GetXaxis()->SetTitle("Canali");
    hist1->GetYaxis()->SetTitle("Conteggi");
    hist1->SetTitle("ADC1_singola_HV2080_THR60_EV20000");
    hist1->GetXaxis()->SetRangeUser(xmin1, xmax1);
    hist1->Draw();
    hist1->Fit(langauss1, "R"); 
    gStyle->SetOptFit(1111);

    //with the parameter obtained from the fit we can draw the landau and gaussian separately
    //landau
    TF1* landau1 = new TF1("landau1", "landau(0)", xmin1, xmax1);
    landau1->SetParameter(0, langauss1->GetParameter(0));
    landau1->SetParameter(1, langauss1->GetParameter(1));
    landau1->SetParameter(2, langauss1->GetParameter(2));
    landau1->SetLineColor(kBlue);
    landau1->Draw("same");

    //gaussian
    TF1* gaus1 = new TF1("gaus1", "gaus(0)", xmin1, xmax1);
    gaus1->SetParameter(0, langauss1->GetParameter(3));
    gaus1->SetParameter(1, langauss1->GetParameter(4));
    gaus1->SetParameter(2, langauss1->GetParameter(5));
    gaus1->SetLineColor(kGreen);
    gaus1->Draw("same");

    can1_langauss->Update();
    can1_langauss->SaveAs("../plots/ADC1_singola.pdf");    

    
    //-----------ADC-2-------------------------------------------------------------------------------------------------//
    
    
    //open file
    TFile *input_file2 = TFile::Open("../data/singole/ADC2_singola_HV2050_THR62_20000.root");
    
    //take the right ADC histos
    TH1D* hist2_before_rebinning = (TH1D*)input_file2->Get("adc2");
    TH1D* hist2 = (TH1D*)hist2_before_rebinning->Rebin(1, "hist2");

    //landau+gauss fit 
    int xmin2 = 0;
    int xmax2 = 500;
    TF1* langauss2 = new TF1("langauss", "landau(0) + gaus(3)", xmin2, xmax2);
    
    langauss2->SetParameter(0, hist2->GetMaximum());
    langauss2->SetParameter(1, hist2->GetMean());
    langauss2->SetParameter(2, hist2->GetRMS()*0.3);
    langauss2->SetParameter(3, hist2->GetMaximum());
    langauss2->SetParameter(4, hist2->GetMean());
    langauss2->SetParameter(5, hist2->GetRMS()*0.3);
    
    
    TCanvas *can2_langauss = new TCanvas("c2_langauss", "Landau + Gauss", 800, 600);
    hist2->GetXaxis()->SetTitle("Canali");
    hist2->GetYaxis()->SetTitle("Conteggi");
    hist2->SetTitle("ADC2_singola_HV2050_THR62_EV20000");
    hist2->GetXaxis()->SetRangeUser(xmin2, xmax2);
    hist2->Draw();
    hist2->Fit(langauss2, "R"); 
    gStyle->SetOptFit(1111);    
    
    //with the parameter obtained from the fit we can draw the landau and gaussian separately
    //landau
    TF1* landau2 = new TF1("landau2", "landau(0)", xmin2, xmax2);
    landau2->SetParameter(0, langauss2->GetParameter(0));
    landau2->SetParameter(1, langauss2->GetParameter(1));
    landau2->SetParameter(2, langauss2->GetParameter(2));
    landau2->SetLineColor(kBlue);
    landau2->Draw("same");

    //gaussian
    TF1* gaus2 = new TF1("gaus2", "gaus(0)", xmin2, xmax2);
    gaus2->SetParameter(0, langauss2->GetParameter(3));
    gaus2->SetParameter(1, langauss2->GetParameter(4));
    gaus2->SetParameter(2, langauss2->GetParameter(5));
    gaus2->SetLineColor(kGreen);
    gaus2->Draw("same");

    can2_langauss->Update();
    can2_langauss->SaveAs("../plots/ADC2_singola.pdf");    
    
    //-----------ADC-3-------------------------------------------------------------------------------------------------//
    
    //open file
    TFile *input_file3 = TFile::Open("../data/singole/ADC3_singola_HV1980_THR40_100000.root");  
    
    //take the right ADC histos
    TH1D* hist3_before_rebinning = (TH1D*)input_file3->Get("adc3");
    TH1D* hist3 = (TH1D*)hist3_before_rebinning->Rebin(1, "hist3");

    //landau+gauss fit 
    int xmin3 = 0;
    int xmax3 = 500;
    TF1* langauss3 = new TF1("langauss", "landau(0) + gaus(3)", xmin3, xmax3);
    
    langauss3->SetParameter(0, hist3->GetMaximum());
    langauss3->SetParameter(1, hist3->GetMean());
    langauss3->SetParameter(2, hist3->GetRMS()*0.3);
    langauss3->SetParameter(3, hist3->GetMaximum());
    langauss3->SetParameter(4, hist3->GetMean());
    langauss3->SetParameter(5, hist3->GetRMS()*0.3);
    
    
    TCanvas *can3_langauss = new TCanvas("c3_langauss", "Landau + Gauss", 800, 600);
    hist3->GetXaxis()->SetTitle("Canali");
    hist3->GetYaxis()->SetTitle("Conteggi");
    hist3->SetTitle("ADC3_singola_HV1980_THR40_EV100000");
    hist3->GetXaxis()->SetRangeUser(xmin3, xmax3);
    hist3->Draw();
    hist3->Fit(langauss3, "R"); 
    gStyle->SetOptFit(1111);    
    
    //with the parameter obtained from the fit we can draw the landau and gaussian separately
    //landau
    TF1* landau3 = new TF1("landau3", "landau(0)", xmin3, xmax3);
    landau3->SetParameter(0, langauss3->GetParameter(0));
    landau3->SetParameter(1, langauss3->GetParameter(1));
    landau3->SetParameter(2, langauss3->GetParameter(2));
    landau3->SetLineColor(kBlue);
    landau3->Draw("same");

    //gaussian
    TF1* gaus3 = new TF1("gaus3", "gaus(0)", xmin3, xmax3);
    gaus3->SetParameter(0, langauss3->GetParameter(3));
    gaus3->SetParameter(1, langauss3->GetParameter(4));
    gaus3->SetParameter(2, langauss3->GetParameter(5));
    gaus3->SetLineColor(kGreen);
    gaus3->Draw("same");

    can3_langauss->Update();
    can3_langauss->SaveAs("../plots/ADC3_singola.pdf");    
    
    //-----------ADC-4-------------------------------------------------------------------------------------------------//
    
    //open file
    TFile *input_file4 = TFile::Open("../data/singole/ADC4_singola_HV2070_THR106_5000.root");
    
    //take the right ADC histos
    TH1D* hist4_before_rebinning = (TH1D*)input_file4->Get("adc4");
    TH1D* hist4 = (TH1D*)hist4_before_rebinning->Rebin(1, "hist4");

    //landau+gauss fit 
    int xmin4 = 0;
    int xmax4 = 500;
    TF1* langauss4 = new TF1("langauss", "landau(0) + gaus(3)", xmin4, xmax4);
    
    langauss4->SetParameter(0, hist4->GetMaximum());
    langauss4->SetParameter(1, hist4->GetMean());
    langauss4->SetParameter(2, hist4->GetRMS()*0.3);
    langauss4->SetParameter(3, hist4->GetMaximum());
    langauss4->SetParameter(4, hist4->GetMean());
    langauss4->SetParameter(5, hist4->GetRMS()*0.3);
    
    
    TCanvas *can4_langauss = new TCanvas("c4_langauss", "Landau + Gauss", 800, 600);
    hist4->GetXaxis()->SetTitle("Canali");
    hist4->GetYaxis()->SetTitle("Conteggi");
    hist4->SetTitle("ADC4_singola_HV2070_THR106_EV5000");
    hist4->GetXaxis()->SetRangeUser(xmin4, xmax4);
    hist4->Draw();
    hist4->Fit(langauss4, "R"); 
    gStyle->SetOptFit(1111);    
    
    //with the parameter obtained from the fit we can draw the landau and gaussian separately
    //landau
    TF1* landau4 = new TF1("landau4", "landau(0)", xmin4, xmax4);
    landau4->SetParameter(0, langauss4->GetParameter(0));
    landau4->SetParameter(1, langauss4->GetParameter(1));
    landau4->SetParameter(2, langauss4->GetParameter(2));
    landau4->SetLineColor(kBlue);
    landau4->Draw("same");

    //gaussian
    TF1* gaus4 = new TF1("gaus4", "gaus(0)", xmin4, xmax4);
    gaus4->SetParameter(0, langauss4->GetParameter(3));
    gaus4->SetParameter(1, langauss4->GetParameter(4));
    gaus4->SetParameter(2, langauss4->GetParameter(5));
    gaus4->SetLineColor(kGreen);
    gaus4->Draw("same");

    can4_langauss->Update();
    can4_langauss->SaveAs("../plots/ADC4_singola.pdf");    
    
    //-----------ADC-5-------------------------------------------------------------------------------------------------//
    
    //open file
    TFile *input_file5 = TFile::Open("../data/singole/ADC5_singola_HV2050_THR44_20000.root");
    
    //take the right ADC histos
    TH1D* hist5_before_rebinning = (TH1D*)input_file5->Get("adc5");
    TH1D* hist5 = (TH1D*)hist5_before_rebinning->Rebin(1, "hist5");

    //landau+gauss fit 
    int xmin5 = 0;
    int xmax5 = 500;
    TF1* langauss5 = new TF1("langauss", "landau(0) + gaus(3)", xmin5, xmax5);
    
    langauss5->SetParameter(0, hist5->GetMaximum());
    langauss5->SetParameter(1, hist5->GetMean());
    langauss5->SetParameter(2, hist5->GetRMS()*0.3);
    langauss5->SetParameter(3, hist5->GetMaximum());
    langauss5->SetParameter(4, hist5->GetMean());
    langauss5->SetParameter(5, hist5->GetRMS()*0.3);
    
    
    TCanvas *can5_langauss = new TCanvas("c5_langauss", "Landau + Gauss", 800, 600);
    hist5->GetXaxis()->SetTitle("Canali");
    hist5->GetYaxis()->SetTitle("Conteggi");
    hist5->SetTitle("ADC5_singola_HV2050_THR44_EV20000");
    hist5->GetXaxis()->SetRangeUser(xmin5, xmax5);
    hist5->Draw();
    hist5->Fit(langauss5, "R"); 
    gStyle->SetOptFit(1111);    
    
    //with the parameter obtained from the fit we can draw the landau and gaussian separately
    //landau
    TF1* landau5 = new TF1("landau5", "landau(0)", xmin5, xmax5);
    landau5->SetParameter(0, langauss5->GetParameter(0));
    landau5->SetParameter(1, langauss5->GetParameter(1));
    landau5->SetParameter(2, langauss5->GetParameter(2));
    landau5->SetLineColor(kBlue);
    landau5->Draw("same");

    //gaussian
    TF1* gaus5 = new TF1("gaus5", "gaus(0)", xmin5, xmax5);
    gaus5->SetParameter(0, langauss5->GetParameter(3));
    gaus5->SetParameter(1, langauss5->GetParameter(4));
    gaus5->SetParameter(2, langauss5->GetParameter(5));
    gaus5->SetLineColor(kGreen);
    gaus5->Draw("same");

    can5_langauss->Update();
    can5_langauss->SaveAs("../plots/ADC5_singola.pdf");
    
    
    //-----------ADC-6-------------------------------------------------------------------------------------------------//
    
    //open file
    TFile *input_file6 = TFile::Open("../data/singole/ADC6_singola_HV1810_THR40_10000.root");
    
    //take the right ADC histos
    TH1D* hist6_before_rebinning = (TH1D*)input_file6->Get("adc6");
    TH1D* hist6 = (TH1D*)hist6_before_rebinning->Rebin(1, "hist6");

    //landau+gauss fit 
    int xmin6 = 0;
    int xmax6 = 500;
    TF1* langauss6 = new TF1("langauss", "landau(0) + gaus(3)", xmin6, xmax6);
    
    langauss6->SetParameter(0, hist6->GetMaximum());
    langauss6->SetParameter(1, hist6->GetMean());
    langauss6->SetParameter(2, hist6->GetRMS()*0.3);
    langauss6->SetParameter(3, hist6->GetMaximum());
    langauss6->SetParameter(4, hist6->GetMean());
    langauss6->SetParameter(5, hist6->GetRMS()*0.3);
    
    
    TCanvas *can6_langauss = new TCanvas("c6_langauss", "Landau + Gauss", 800, 600);
    hist6->GetXaxis()->SetTitle("Canali");
    hist6->GetYaxis()->SetTitle("Conteggi");
    hist6->SetTitle("ADC6_singola_HV1810_THR40_EV10000");
    hist6->GetXaxis()->SetRangeUser(xmin6, xmax6);
    hist6->Draw();
    hist6->Fit(langauss6, "R"); 
    gStyle->SetOptFit(1111);    
    
    //with the parameter obtained from the fit we can draw the landau and gaussian separately
    //landau
    TF1* landau6 = new TF1("landau6", "landau(0)", xmin6, xmax6);
    landau6->SetParameter(0, langauss6->GetParameter(0));
    landau6->SetParameter(1, langauss6->GetParameter(1));
    landau6->SetParameter(2, langauss6->GetParameter(2));
    landau6->SetLineColor(kBlue);
    landau6->Draw("same");

    //gaussian
    TF1* gaus6 = new TF1("gaus6", "gaus(0)", xmin6, xmax6);
    gaus6->SetParameter(0, langauss6->GetParameter(3));
    gaus6->SetParameter(1, langauss6->GetParameter(4));
    gaus6->SetParameter(2, langauss6->GetParameter(5));
    gaus6->SetLineColor(kGreen);
    gaus6->Draw("same");

    can6_langauss->Update();
    can6_langauss->SaveAs("../plots/ADC6_singola.pdf");
    
}
