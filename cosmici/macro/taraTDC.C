#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"

void taraTDC(){
    // numero misure prese
    const int nmisure_a = 9;

    // ---------------------- TDC canale 3 --------------------------- //
    
    float ritardi[] = {-61, -45.75, -30.50, -15.25, 0, 15.25, 30.50, 45.75, 61}; //tempi con le I da 0.25 ns aggiunti;
    float canali[] = {142.5, 205.3, 267.1, 327.8, 387.2, 452.2, 517.9, 585.4, 655.0};
    // errori
    float s_ritardi[] = {2, 1.71, 1.41, 1, 1, 1, 1.41, 1.71, 2};
    float s_canali[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    // ----------------------------------------------------------------- //

    

    TCanvas *TDC = new TCanvas("TDC","g(f)",200,10,600,400);
    // Mi assicuro che la tela sia bianca (0 corrisponde al bianco, per altri colori vedi https://root.cern.ch/doc/master/classTColor.html)
    TDC->SetFillColor(0);
    TDC->cd();
    TGraphErrors *ritardi_vs_canali = new TGraphErrors(nmisure_a,ritardi,canali,s_ritardi,s_canali);

  ritardi_vs_canali->SetMarkerSize(1);
  ritardi_vs_canali->SetMarkerStyle(20);
  ritardi_vs_canali->SetMarkerColor(kBlue);

  // Titolo del grafico
  ritardi_vs_canali->SetTitle("Ritardi vs Canali 3");
  // Titoli degli assi
  ritardi_vs_canali->GetXaxis()->SetTitle("Ritardi [ps]");
  ritardi_vs_canali->GetYaxis()->SetTitle("Canali");

  TF1 *fit_lineare = new TF1("fit_lineare", "pol1", -70, 70);
  ritardi_vs_canali->Fit(fit_lineare, "R");
  cout << "Chi^2:" << fit_lineare->GetChisquare() << ", number of DoF: " << fit_lineare->GetNDF() << " (Probability: " << fit_lineare->GetProb() << ")." << endl;


  ritardi_vs_canali->Draw("AP");
  ritardi_vs_canali->SaveAs("../plots/taratura_TDC_3.pdf");


  //------------------- TDC canale 4 --------------------------
   const int nmisure_b = 9;
    float ritardi2[] = {-61, -45.75, -30.50, -15.25, 0, 15.25, 30.50, 45.75, 61}; //tempi con le I da 0.25 ns aggiunti;
    float canali2[] = {137.9, 200.1, 261.2, 321.1, 380.2, 444.3, 510.6, 578.6, 648.4};
    // errori
    float s_ritardi2[] = {2, 1.71, 1.41, 1, 1, 1, 1.41, 1.71, 2};
    float s_canali2[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    // ----------------------------------------------------------------- //

    

    TCanvas *TDC4 = new TCanvas("TDC4","g(f)",200,10,600,400);
    // Mi assicuro che la tela sia bianca (0 corrisponde al bianco, per altri colori vedi https://root.cern.ch/doc/master/classTColor.html)
    TDC4->SetFillColor(0);
    TDC4->cd();
    TGraphErrors *ritardi_vs_canali2 = new TGraphErrors(nmisure_b,ritardi2,canali2,s_ritardi2,s_canali2);

  ritardi_vs_canali2->SetMarkerSize(1);
  ritardi_vs_canali2->SetMarkerStyle(20);
  ritardi_vs_canali2->SetMarkerColor(kBlue);

  // Titolo del grafico
  ritardi_vs_canali2->SetTitle("Ritardi vs Canali 4");
  // Titoli degli assi
  ritardi_vs_canali2->GetXaxis()->SetTitle("Ritardi [ps]");
  ritardi_vs_canali2->GetYaxis()->SetTitle("Canali");


  ritardi_vs_canali2->Fit(fit_lineare, "R");
  cout << "Chi^2:" << fit_lineare->GetChisquare() << ", number of DoF: " << fit_lineare->GetNDF() << " (Probability: " << fit_lineare->GetProb() << ")." << endl;


  ritardi_vs_canali2->Draw("AP");
  ritardi_vs_canali2->SaveAs("../plots/taratura_TDC_4.pdf");
}
