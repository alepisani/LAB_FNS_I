#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include <cmath>

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

    // Sopprime la visualizzazione a schermo
  //gROOT->SetBatch(kTRUE);

  // File ROOT di output con tutti i fit
  TFile *output_file = new TFile("../plots/landgauss/all_fits.root", "RECREATE");
        
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
  TFile *input_file1 = TFile::Open("../data/singole/landgauss_analysis/ADC1_singola_HV2080_THR60_20000.root");
  
  //take the right ADC histos
  TH1D* hist1_before_rebinning = (TH1D*)input_file1->Get("adc1");
  TH1D* hist1 = (TH1D*)hist1_before_rebinning->Rebin(1, "hist1");

  //landau+gauss fit 
  int xmin1 = 40;
  int xmax1 = 300;
  TF1* langauss1 = new TF1("langauss1", "landau(0) + gaus(3)", xmin1, xmax1);
  langauss1->SetLineWidth(2);
  langauss1->SetLineColor(kRed);
  
  langauss1->SetParameter(0, 200);
  langauss1->SetParameter(1, 105);
  langauss1->SetParameter(2, 30);
  langauss1->SetParameter(3, 250);
  langauss1->SetParameter(4, 68);
  langauss1->SetParameter(5, 10);

  
  
  
  TCanvas *can1_langauss = new TCanvas("c1_langauss", "Landau + Gauss", 800, 600);
  hist1->GetXaxis()->SetTitle("Canali");
  hist1->GetYaxis()->SetTitle("Conteggi");
  hist1->SetTitle("ADC1_singola_HV2080_THR60_EV20000");
  hist1->GetXaxis()->SetRangeUser(xmin1, xmax1);
  hist1->Draw();
  hist1->Fit(langauss1, "RQ"); 
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
  gaus1->SetLineColor(kGreen+2);
  gaus1->Draw("same");

  can1_langauss->Update();
  can1_langauss->SaveAs("../plots/landgauss/ADC1_singola.pdf");   
    

  //-----------ADC-2-------------------------------------------------------------------------------------------------//
  
  
  //open file
  TFile *input_file2 = TFile::Open("../data/singole/landgauss_analysis/ADC2_singola_HV2050_THR62_20000.root");
  
  //take the right ADC histos
  TH1D* hist2_before_rebinning = (TH1D*)input_file2->Get("adc2");
  TH1D* hist2 = (TH1D*)hist2_before_rebinning->Rebin(2, "hist2");
  
  //landau+gauss fit 
  int xmin2 = 70;
  int xmax2 = 500;
  TF1* langauss2 = new TF1("langauss", "landau(0) + gaus(3)", xmin2, xmax2);
  langauss2->SetLineWidth(2);
  langauss2->SetLineColor(kRed);
  
  langauss2->SetParameter(0, 220);
  langauss2->SetParameter(1, 180);
  langauss2->SetParameter(2, 30);
  langauss2->SetParameter(3, 120);
  langauss2->SetParameter(4, 100);
  langauss2->SetParameter(5, 10);
  
  
  TCanvas *can2_langauss = new TCanvas("c2_langauss", "Landau + Gauss", 800, 600);
  hist2->GetXaxis()->SetTitle("Canali");
  hist2->GetYaxis()->SetTitle("Conteggi");
  hist2->SetTitle("ADC2_singola_HV2050_THR62_EV20000");
  hist2->GetXaxis()->SetRangeUser(xmin2, xmax2);
  hist2->Draw();
  hist2->Fit(langauss2, "RQ"); 
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
  gaus2->SetLineColor(kGreen+2);
  gaus2->Draw("same");
  
  can2_langauss->Update();
  can2_langauss->SaveAs("../plots/landgauss/ADC2_singola.pdf");    
  
  //-----------ADC-3-------------------------------------------------------------------------------------------------//
  
  
  //open file
  TFile *input_file3 = TFile::Open("../data/singole/landgauss_analysis/ADC3_singola_HV1980_THR40_100000.root");  
  
  //take the right ADC histos
  TH1D* hist3_before_rebinning = (TH1D*)input_file3->Get("adc3");
  TH1D* hist3 = (TH1D*)hist3_before_rebinning->Rebin(1, "hist3");
  
  //landau+gauss fit 
  int xmin3 = 25;
  int xmax3 = 300;
  TF1* langauss3 = new TF1("langauss", "landau(0) + gaus(3)", xmin3, xmax3);
  langauss3->SetLineWidth(2);
  langauss3->SetLineColor(kRed);
  
  langauss3->SetParameter(0, 1400);
  langauss3->SetParameter(1, 115);
  langauss3->SetParameter(2, 20);
  langauss3->SetParameter(3, 1800);
  langauss3->SetParameter(4, 75);
  langauss3->SetParameter(5, 10);
  
  
  TCanvas *can3_langauss = new TCanvas("c3_langauss", "Landau + Gauss", 800, 600);
  hist3->GetXaxis()->SetTitle("Canali");
  hist3->GetYaxis()->SetTitle("Conteggi");
  hist3->SetTitle("ADC3_singola_HV1980_THR40_EV100000");
  hist3->GetXaxis()->SetRangeUser(xmin3, xmax3);
  hist3->Draw();
  hist3->Fit(langauss3, "RQ"); 
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
  gaus3->SetLineColor(kGreen+2);
  gaus3->Draw("same");
  
  can3_langauss->Update();
  can3_langauss->SaveAs("../plots/landgauss/ADC3_singola.pdf");    
  
  //-----------ADC-4-------------------------------------------------------------------------------------------------//
  
  //open file
  TFile *input_file4 = TFile::Open("../data/singole/landgauss_analysis/ADC4_singola_HV2070_THR106_5000.root");
  
  //take the right ADC histos
  TH1D* hist4_before_rebinning = (TH1D*)input_file4->Get("adc4");
  TH1D* hist4 = (TH1D*)hist4_before_rebinning->Rebin(2, "hist4");
  
  //landau+gauss fit 
  int xmin4 = 25;
  int xmax4 = 230;
  TF1* langauss4 = new TF1("langauss", "landau(0) + gaus(3)", xmin4, xmax4);
  langauss4->SetLineWidth(2);
  langauss4->SetLineColor(kRed);
  
  langauss4->SetParameter(0, hist4->GetMaximum());
  langauss4->SetParameter(1, 80);
  langauss4->SetParameter(2, 30);
  langauss4->SetParameter(3, 100);
  langauss4->SetParameter(4, 55);
  langauss4->SetParameter(5, 5);
  
  
  TCanvas *can4_langauss = new TCanvas("c4_langauss", "Landau + Gauss", 800, 600);
  hist4->GetXaxis()->SetTitle("Canali");
  hist4->GetYaxis()->SetTitle("Conteggi");
  hist4->SetTitle("ADC4_singola_HV2070_THR106_EV5000");
  hist4->GetXaxis()->SetRangeUser(xmin4, xmax4);
  hist4->Draw();
  hist4->Fit(langauss4, "RQ"); 
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
  gaus4->SetLineColor(kGreen+2);
  gaus4->Draw("same");
  
  can4_langauss->Update();
  can4_langauss->SaveAs("../plots/landgauss/ADC4_singola.pdf");    
  
  //-----------ADC-5-------------------------------------------------------------------------------------------------//
  
  //open file
  TFile *input_file5 = TFile::Open("../data/singole/landgauss_analysis/ADC5_singola_HV2050_THR44_20000.root");
  
  //take the right ADC histos
  TH1D* hist5_before_rebinning = (TH1D*)input_file5->Get("adc5");
  TH1D* hist5 = (TH1D*)hist5_before_rebinning->Rebin(1, "hist5");
  
  //landau+gauss fit 
  int xmin5 = 5;
  int xmax5 = 500;
  TF1* langauss5 = new TF1("langauss", "landau(0) + gaus(3)", xmin5, xmax5);
  langauss5->SetLineWidth(2);
  langauss5->SetLineColor(kRed);
  
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
  hist5->Fit(langauss5, "RQ"); 
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
  gaus5->SetLineColor(kGreen+2);
  gaus5->Draw("same");
  
  can5_langauss->Update();
  can5_langauss->SaveAs("../plots/landgauss/ADC5_singola.pdf");
  
  
  //-----------ADC-6-------------------------------------------------------------------------------------------------//
  
  //open file
  TFile *input_file6 = TFile::Open("../data/singole/landgauss_analysis/ADC6_singola_HV1810_THR40_10000.root");
  
  //take the right ADC histos
  TH1D* hist6_before_rebinning = (TH1D*)input_file6->Get("adc6");
  TH1D* hist6 = (TH1D*)hist6_before_rebinning->Rebin(4, "hist6");
  
  //landau+gauss fit 
  int xmin6 = 70;
  int xmax6 = 700;
  TF1* langauss6 = new TF1("langauss", "landau(0) + gaus(3) + gaus(6)", xmin6, xmax6);
  langauss6->SetLineWidth(2);
  langauss6->SetLineColor(kRed);
  
  langauss6->SetParameter(0, 200);
  langauss6->SetParameter(1, 260);
  langauss6->SetParameter(2, 40);
  langauss6->SetParameter(3, 100);
  langauss6->SetParameter(4, 120);
  langauss6->SetParameter(5, 20);
  langauss6->SetParameter(6, 100);
  langauss6->SetParameter(7, 90);
  langauss6->SetParameter(8, 10);
  
  
  TCanvas *can6_langauss = new TCanvas("c6_langauss", "Landau + Gauss", 800, 600);
  hist6->GetXaxis()->SetTitle("Canali");
  hist6->GetYaxis()->SetTitle("Conteggi");
  hist6->SetTitle("ADC6_singola_HV1810_THR40_EV10000");
  hist6->GetXaxis()->SetRangeUser(xmin6, xmax6);
  hist6->Draw();
  hist6->Fit(langauss6, "RQ"); 
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
  TF1* gaus61 = new TF1("gaus61", "gaus(0)", xmin6, xmax6);
  gaus61->SetParameter(0, langauss6->GetParameter(3));
  gaus61->SetParameter(1, langauss6->GetParameter(4));
  gaus61->SetParameter(2, langauss6->GetParameter(5));
  gaus61->SetLineColor(kGreen+2);
  gaus61->Draw("same");

  TF1* gaus62 = new TF1("gaus62", "gaus(0)", xmin6, xmax6);
  gaus62->SetParameter(0, langauss6->GetParameter(6));
  gaus62->SetParameter(1, langauss6->GetParameter(7));
  gaus62->SetParameter(2, langauss6->GetParameter(8));
  gaus62->SetLineColor(kGreen+2);
  gaus62->Draw("same");
  
  can6_langauss->Update();
  can6_langauss->SaveAs("../plots/landgauss/ADC6_singola.pdf");

  //----misura_piedistalli---------------------------------------------------------------------------------
   
  /**
  * si sceglie di usare il file con più statistica che risulta ADC3 per il piedistallo di 1-2-4-5-6
  * per misurare il piedistallo di ADC3 si sceglie ADC2
  */

  //Piedistallo1----------------------------------

  TH1D* histP1_before_rebinning = (TH1D*)input_file3->Get("adc1");
  TH1D* histP1 = (TH1D*)histP1_before_rebinning->Rebin(1, "histP1");

  int xminP1 = 10;
  int xmaxP1 = 60;

  TF1* gausP1 = new TF1("gausP1", "gaus(0)", xminP1, xmaxP1);

  TCanvas *canP1 = new TCanvas("cP1", "GaussP1", 800, 600);
  histP1->GetXaxis()->SetTitle("Canali");
  histP1->GetYaxis()->SetTitle("Conteggi");
  histP1->SetTitle("Piedistallo 1");
  histP1->GetXaxis()->SetRangeUser(xminP1, xmaxP1);
  histP1->Draw();
  histP1->Fit(gausP1, "RQ"); 
  gStyle->SetOptFit(1111);   

  canP1->Update();
  canP1->SaveAs("../plots/landgauss/Piedistallo1.pdf");
  
  //Piedistallo2----------------------------------

  TH1D* histP2_before_rebinning = (TH1D*)input_file3->Get("adc2");
  TH1D* histP2 = (TH1D*)histP2_before_rebinning->Rebin(1, "histP2");

  int xminP2 = 0;
  int xmaxP2 = 70;

  TF1* gausP2 = new TF1("gausP2", "gaus(0)", xminP2, xmaxP2);

  TCanvas *canP2 = new TCanvas("cP2", "GaussP2", 800, 600);
  histP2->GetXaxis()->SetTitle("Canali");
  histP2->GetYaxis()->SetTitle("Conteggi");
  histP2->SetTitle("Piedistallo 2");
  histP2->GetXaxis()->SetRangeUser(xminP2, xmaxP2);
  histP2->Draw();
  histP2->Fit(gausP2, "RQ"); 
  gStyle->SetOptFit(1111);   

  canP2->Update();
  canP2->SaveAs("../plots/landgauss/Piedistallo2.pdf");

  //Piedistallo3----------------------------------

  TH1D* histP3_before_rebinning = (TH1D*)input_file2->Get("adc3");
  TH1D* histP3 = (TH1D*)histP3_before_rebinning->Rebin(2, "histP3");

  int xminP3 = 0;
  int xmaxP3 = 60;

  TF1* gausP3 = new TF1("gausP3", "gaus(0)", xminP3, xmaxP3);

  TCanvas *canP3 = new TCanvas("cP3", "GaussP3", 800, 600);
  histP3->GetXaxis()->SetTitle("Canali");
  histP3->GetYaxis()->SetTitle("Conteggi");
  histP3->SetTitle("Piedistallo 3");
  histP3->GetXaxis()->SetRangeUser(xminP3, xmaxP3);
  histP3->Draw();
  histP3->Fit(gausP3, "RQ"); 
  gStyle->SetOptFit(1111);   

  canP3->Update();
  canP3->SaveAs("../plots/landgauss/Piedistallo3.pdf");

  //Piedistallo4----------------------------------

  TH1D* histP4_before_rebinning = (TH1D*)input_file3->Get("adc4");
  TH1D* histP4 = (TH1D*)histP4_before_rebinning->Rebin(1, "histP4");

  int xminP4 = 0;
  int xmaxP4 = 60;

  TF1* gausP4 = new TF1("gausP4", "gaus(0)", xminP4, xmaxP4);

  TCanvas *canP4 = new TCanvas("cP4", "GaussP4", 800, 600);
  histP4->GetXaxis()->SetTitle("Canali");
  histP4->GetYaxis()->SetTitle("Conteggi");
  histP4->SetTitle("Piedistallo 4");
  histP4->GetXaxis()->SetRangeUser(xminP4, xmaxP4);
  histP4->Draw();
  histP4->Fit(gausP4, "RQ"); 
  gStyle->SetOptFit(1111);   

  canP4->Update();
  canP4->SaveAs("../plots/landgauss/Piedistallo4.pdf");

  //Piedistallo5----------------------------------

  TH1D* histP5_before_rebinning = (TH1D*)input_file3->Get("adc5");
  TH1D* histP5 = (TH1D*)histP5_before_rebinning->Rebin(1, "histP5");

  int xminP5 = 5;
  int xmaxP5 = 200;

  TF1* gausP5 = new TF1("gausP5", "gaus(0)", xminP5, xmaxP5);

  TCanvas *canP5 = new TCanvas("cP5", "GaussP5", 800, 600);
  histP5->GetXaxis()->SetTitle("Canali");
  histP5->GetYaxis()->SetTitle("Conteggi");
  histP5->SetTitle("Piedistallo 5");
  histP5->GetXaxis()->SetRangeUser(xminP5, xmaxP5);
  histP5->Draw();
  histP5->Fit(gausP5, "RQ"); 
  gStyle->SetOptFit(1111);   

  canP5->Update();
  canP5->SaveAs("../plots/landgauss/Piedistallo5.pdf");

  //Piedistallo6----------------------------------

  TH1D* histP6_before_rebinning = (TH1D*)input_file3->Get("adc6");
  TH1D* histP6 = (TH1D*)histP6_before_rebinning->Rebin(1, "histP6");

  int xminP6 = 10; 
  int xmaxP6 = 40;

  TF1* gausP6 = new TF1("gausP6", "gaus(0)", xminP6, xmaxP6);

  TCanvas *canP6 = new TCanvas("cP6", "GaussP6", 800, 600);
  histP6->GetXaxis()->SetTitle("Canali");
  histP6->GetYaxis()->SetTitle("Conteggi");
  histP6->SetTitle("Piedistallo 6");
  histP6->GetXaxis()->SetRangeUser(xminP6, xmaxP6);
  histP6->Draw();
  histP6->Fit(gausP6, "RQ"); 
  gStyle->SetOptFit(1111);   

  canP6->Update(); 
  canP6->SaveAs("../plots/landgauss/Piedistallo6.pdf");

  //-------------------------------------------------------------------------------------final_output

  output_file->cd();
  hist1->Write("ADC1_singola");
  hist2->Write("ADC2_singola");
  hist3->Write("ADC3_singola");
  hist4->Write("ADC4_singola");
  hist5->Write("ADC5_singola");
  hist6->Write("ADC6_singola");
  histP1->Write("Piedistallo1");
  histP2->Write("Piedistallo2");
  histP3->Write("Piedistallo3");
  histP4->Write("Piedistallo4");
  histP5->Write("Piedistallo5");
  histP6->Write("Piedistallo6");
  output_file->Close();  


  double mu[6], sigma_mu[6], piedistallo[6], sigma_pied[6];

  mu[0] = langauss1->GetParameter(1); sigma_mu[0] = langauss1->GetParError(1);
  mu[1] = langauss2->GetParameter(1); sigma_mu[1] = langauss2->GetParError(1);
  mu[2] = langauss3->GetParameter(1); sigma_mu[2] = langauss3->GetParError(1);
  mu[3] = langauss4->GetParameter(1); sigma_mu[3] = langauss4->GetParError(1);
  mu[4] = langauss5->GetParameter(1); sigma_mu[4] = langauss5->GetParError(1);
  mu[5] = langauss6->GetParameter(1); sigma_mu[5] = langauss6->GetParError(1);
  piedistallo[0] = gausP1->GetParameter(1); sigma_pied[0] = gausP1->GetParError(1);
  piedistallo[1] = gausP2->GetParameter(1); sigma_pied[1] = gausP2->GetParError(1);
  piedistallo[2] = gausP3->GetParameter(1); sigma_pied[2] = gausP3->GetParError(1);
  piedistallo[3] = gausP4->GetParameter(1); sigma_pied[3] = gausP4->GetParError(1);
  piedistallo[4] = gausP5->GetParameter(1); sigma_pied[4] = gausP5->GetParError(1);
  piedistallo[5] = gausP6->GetParameter(1); sigma_pied[5] = gausP6->GetParError(1);

  // print table
  printf("+----------------+---------------------+----------------------+------------------------------+\n");
  printf("| Scintillatore  |       mu +/- s      |  Piedistallo +/- s   |   mu - Piedistallo +/- s     |\n");
  printf("+----------------+---------------------+----------------------+------------------------------+\n");

  for(int i = 0; i < 6; i++){
      double diff = mu[i] - piedistallo[i];
      double sigma_diff = sqrt(sigma_mu[i]*sigma_mu[i] + sigma_pied[i]*sigma_pied[i]);
      printf("|       %d        | %8.2f +/- %6.2f |  %8.2f +/- %6.2f |    %8.2f +/- %6.2f       |\n",
            i+1, mu[i], sigma_mu[i], piedistallo[i], sigma_pied[i], diff, sigma_diff);
      printf("+----------------+---------------------+----------------------+------------------------------+\n");
  }


}
