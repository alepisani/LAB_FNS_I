#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "letturaFile.cpp"

void efficienza7(){
        
//-----------Spettro TDC-----------------------------------------------------
vector<vector<int>> data = datReader("../data/doppie_68/TDC_doppia68_5000.dat", 0);
TH1D* TDC7 = new TH1D("TDC7", "Spettro TDC rivelatore 7", 200, 0., 4000);

for (int i=0; i<5000; i++) {
        TDC7->Fill(data[7][i]);

    }

TCanvas* c1 = new TCanvas("c1", "Spettro TDC rivelatore 7", 800, 600);
    TDC7->Draw();
    gStyle->SetOptFit(1111);
    TDC7->GetXaxis()->SetTitle("Canali TDC");
    TDC7->GetYaxis()->SetTitle("Conteggi");
    TDC7->SaveAs("../plots/TDC7.pdf");

// numero misure prese
    const int nmisure_a = 6;

    // ---------------------- Array dei dati --------------------------- //
    
    float soglia[] = {16, 21, 26, 31, 36, 40};
    float efficienza[] = {0.969, 0.975, 0.964, 0.96, 0.93, 0.913};
    // errori
    float s_soglia[] = {1,1,1,1,1,1};
    float s_efficienza[] = {0.006197, 0.005891,0.008068,0.008912,0.004937,0.005481};
    // ----------------------------------------------------------------- //

    //grafico diel guadagno in funzione di f

    TCanvas *eff = new TCanvas("eff","g(f)",200,10,600,400);
    // Mi assicuro che la tela sia bianca (0 corrisponde al bianco, per altri colori vedi https://root.cern.ch/doc/master/classTColor.html)
    eff->SetFillColor(0);
    eff->cd();
    TGraphErrors *eff_vs_thr = new TGraphErrors(nmisure_a,soglia,efficienza,s_soglia,s_efficienza);

  eff_vs_thr->SetMarkerSize(1.5);
  eff_vs_thr->SetMarkerStyle(20);
  eff_vs_thr->SetMarkerColor(kBlue);

  // Titolo del grafico
  eff_vs_thr->SetTitle("Efficienza rivelatore 7");
  // Titoli degli assi
  eff_vs_thr->GetXaxis()->SetTitle("Soglia [mV]");
  eff_vs_thr->GetYaxis()->SetTitle("Efficienza");

  eff_vs_thr->Draw("AP");
  eff_vs_thr->SaveAs("../plots/eff7_vs_thr.pdf");

  //efficienza 8
  /*
  const int nmisure = 11;
  double threshold[] = {30, 40.3, 43.7, 46.7, 50.7, 55.1, 58.5, 63.3, 68.3, 72.5, 75};
  double ev_fondoscala[] = {705, 683, 724, 682, 723, 726, 706, 742, 760, 748, 762};
  double frazione_eventi_visti[nmisure];
  
  for(int i = 0; i < nmisure; i++){
    frazione_eventi_visti[i] = 1 - ev_fondoscala[i]/1000.;
    //cout << frazione_eventi_visti[i] << endl;
  }


  TCanvas *eff8 = new TCanvas("eff8", "eff8", 200, 10, 600, 400);
  TGraphErrors *eff8_vs_thr = new TGraphErrors(nmisure, threshold, frazione_eventi_visti, 0, 0);

  eff8_vs_thr->GetXaxis()->SetTitle("threshold [mV]");
  eff8_vs_thr->GetYaxis()->SetTitle("frazione eventi visti");
  eff8_vs_thr->SetMarkerSize(3.);
  eff8_vs_thr->Draw();
  eff8_vs_thr->SaveAs("../plots/eff8_vs_thr.png");
*/



}