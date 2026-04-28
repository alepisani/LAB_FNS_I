#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"

void efficienza(){
        
// numero misure prese
    const int nmisure_a = 6;

    // ---------------------- Array dei dati --------------------------- //
    
    float soglia[] = {16, 21, 26, 31, 36, 40};
    float efficienza[] = {0.969, 0.975, 0.964, 0.96, 0.93, 0.913};
    // errori
    float s_soglia[] = {};
    float s_efficienza[] = {};
    // ----------------------------------------------------------------- //

    //grafico diel guadagno in funzione di f

    TCanvas *eff = new TCanvas("eff","g(f)",200,10,600,400);
    // Mi assicuro che la tela sia bianca (0 corrisponde al bianco, per altri colori vedi https://root.cern.ch/doc/master/classTColor.html)
    eff->SetFillColor(0);
    eff->cd();
    TGraphErrors *eff_vs_thr = new TGraphErrors(nmisure_a,soglia,efficienza,0,0);

  eff_vs_thr->SetMarkerSize(1.5);
  eff_vs_thr->SetMarkerStyle(29);
  eff_vs_thr->SetMarkerColor(kBlue);

  // Titolo del grafico
  eff_vs_thr->SetTitle("Efficienza del 7");
  // Titoli degli assi
  eff_vs_thr->GetXaxis()->SetTitle("soglia [mV]");
  eff_vs_thr->GetYaxis()->SetTitle("efficienza");



  eff_vs_thr->Draw("AP");
  eff_vs_thr->SaveAs("../plots/eff7_vs_thr.pdf");


}