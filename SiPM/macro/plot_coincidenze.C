#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <TGraph.h>
#include <iostream>
#include "txt_reader.cpp"

void plot_coincidenze() {

    // multigraph con i dati ottenuti dalle coincidenze 
    //creo prima i tre grafici, poi li metto in un multigraph

    //--------- solo dark count ------------

    const int n=12;
    double x1[n]= {5, 15, 25, 35, 45, 55, 65, 75, 85, 95, 105, 115};
    //double y1[n]={611.178, 670.881, 649.682, 109.4085, 90.561,  54.6598, 14.303, 11.10056, 4.52493, 1.801305, 1.319085, 0.4403};  //scaler ch1
    //double y1[n]= {523.061, 546.937, 152.001, 75.4665, 61.6585, 12.3994, 9.358773333, 5.29293, 1.494005, 1.1206375, 0.416765, 0.1894}; //scaler ch2
    double y1[n]={37332, 42052, 11331, 939.5, 644, 82.06666667, 15.58666667, 6.49, 0.9, 0.285, 0.0725, 0.0067}; //scaler ch3
    TGraph* g1 = new TGraph(n, x1, y1);
    g1->SetLineColor(kRed);
    g1->SetLineWidth(2);
    

    //------------- DC e cosmici  ---------------

    const int m=13;
    double x2[m]= {5, 15, 25, 35, 45, 55, 65, 75, 85, 95, 105, 115, 125};
    //double y2[m]={609.93, 670.80, 671.01, 120.66, 99.12, 74.65, 17.46, 13.59, 8.45, 2.47, 1.86, 1.04, 0.31};  //scaler ch1
    //double y2[m]= {626.42, 605.78, 250.92, 92.89, 87.30,22.33, 14.43, 10.07, 2.56,  1.36, 0.92, 0.34, 0.19}; //scaler ch2
    double y2[m]={45151.00, 47019.00, 19304.00, 1267.00, 1018.50, 196.13, 33.55, 18.65, 3.48, 1.47, 0.78, 0.51, 0.41}; //scaler ch3
    TGraph* g2 = new TGraph(m, x2, y2);
    g2->SetLineColor(kBlue);
    g2->SetLineWidth(2);

    //---------------- dc, cosmici e sorgente --------------
    const int N=13;
    double x3[N]= {5, 15, 25, 35, 45, 55, 65, 75, 85, 95, 105, 115, 125};
    //double y3[N]={608.209, 670.92, 666.494, 116.67, 95.774, 64.17575, 15.974775, 12.3292, 6.4251, 2.119015, 1.56225, 0.62767, 0.292325};  //scaler ch1
    //double y3[N]= {542.81, 567.857, 232.268, 84.857, 72.3255, 17.054125,  11.2815, 7.4505, 1.8866, 1.37975, 0.562745, 0.233315, 0.149865}; //scaler ch2
    double y3[N]={39357, 44556, 18187, 1321, 1054, 226.375, 83.925, 48.38333333, 19.78, 10.945, 5.31, 2.63,1.31}; //scaler ch3
    TGraph* g3 = new TGraph(N, x3, y3);
    g3->SetLineColor(kGreen);
    g3->SetLineWidth(2);

    // ---- IL CONTENITORE: TMultiGraph ----
    TMultiGraph *mg = new TMultiGraph();
    
    // Aggiungi i singoli grafici al multigraph
    // "LP" significa che per quel grafico vuoi disegnare sia le Linee (L) che i Punti (P)
    mg->Add(g1, "L");
    mg->Add(g2, "L");
    mg->Add(g3, "L");

    // Imposta il titolo globale del multigraph (TitoloGenerale;AsseX;AsseY)
    //mg->SetTitle("Scaler ch2; soglie [mV]; Rate[kHz]"); //singole
    mg->SetTitle("Scaler ch3 - coincidenze; Soglia [mV]; Rate[Hz]"); //coincidenze

    TLegend *leg = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg->SetTextSize(0.03);
    leg->SetHeader("Legenda", "C"); // "C" centra il testo del titolo
    leg->AddEntry(g1, "Solo Dark Count", "l");
    leg->AddEntry(g2, "DC e Cosmici", "l");
    leg->AddEntry(g3, "DC, Cosmici e Sorgente", "l");

    // ---- DISEGNO ----
    TCanvas *c1 = new TCanvas("c1", "Canvas", 800, 600);
    c1->SetLogy();
    
    // Disegni il TMultiGraph con l'opzione "A" per fargli generare gli assi automatici
    // che contengano TUTTI i punti di ENTRAMBI i grafici.
    mg->Draw("A");

    // 2. ABILITA LA GRIGLIA (chiamata su gPad dopo il Draw)
    gPad->SetGrid(); // Attiva sia griglia X che Y (oppure usa SetGridx() solo per le X)

    // 3. AGGIUNGI LA LINEA VERTICALE
    // Sostituisci '32.0' con il valore di soglia in mV che ti interessa
    double x_valore = 100.0; 
    TLine *line = new TLine(x_valore, 0, x_valore, 100000);
    
    // Personalizzazione estetica della linea
    line->SetLineColor(kBlack);      // Colore rosso
    line->SetLineStyle(2);         // Tratteggiata
    line->SetLineWidth(2);         // Spessore maggiorato
    line->Draw();

    leg->Draw();
    c1->SaveAs("../plots/scalerch3.png");

}