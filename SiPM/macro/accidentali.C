#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <TGraph.h>
#include <iostream>
#include "txt_reader.cpp"

void accidentali(){

    //-------------- coincidenze misurate --------------
    const int n=12;
    double x1[n]= {5, 15, 25, 35, 45, 55, 65, 75, 85, 95, 105, 115};
    double y1[n]={37332, 42052, 11331, 939.5, 644, 82.06666667, 15.58666667, 6.49, 0.9, 0.285, 0.0725, 0.0067}; //scaler ch3
    TGraph* g1 = new TGraph(n, x1, y1);
    g1->SetLineColor(kRed);
    g1->SetLineWidth(2);
    

    //------------coincidenze accidentali ---------------

    const int m=12;
    double x2[m]= {5, 15, 25, 35, 45, 55, 65, 75, 85, 95, 105, 115};
    double y2[m]={38362.0051, 44031.55698, 11850.27764, 990.8011878, 670.0626502, 81.32984689, 16.0630242, 7.050538445, 0.8112321654, 0.2422331918, 0.0659698152, 0.01000383957}; //scaler ch3
    TGraph* g2 = new TGraph(m, x2, y2);
    g2->SetLineColor(kBlue);
    g2->SetLineWidth(2);

    // ---- IL CONTENITORE: TMultiGraph ----
    TMultiGraph *mg = new TMultiGraph();
    
    // Aggiungi i singoli grafici al multigraph
    // "LP" significa che per quel grafico vuoi disegnare sia le Linee (L) che i Punti (P)
    mg->Add(g1, "L");
    mg->Add(g2, "L");

    // Imposta il titolo globale del multigraph (TitoloGenerale;AsseX;AsseY)
    //mg->SetTitle("Confronto Curve; soglie [mV]; Rate[kHz]"); //singole
    mg->SetTitle("Confronto Curve; soglie [mV]; Rate[Hz]"); //coincidenze

    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg->SetTextSize(0.03);
    leg->SetHeader("Legenda", "C"); // "C" centra il testo del titolo
    leg->AddEntry(g1, "Coincidenze misurate", "l");
    leg->AddEntry(g2, "Coincidenze accidentali", "l");


    // ---- DISEGNO ----
    TCanvas *c1 = new TCanvas("c1", "Canvas", 800, 600);
    c1->SetLogy();
    
    // Disegni il TMultiGraph con l'opzione "A" per fargli generare gli assi automatici
    // che contengano TUTTI i punti di ENTRAMBI i grafici.
    mg->Draw("A");
    leg->Draw();
    //c1->SaveAs("../plots/coincidenzeaccidentali.png");


}