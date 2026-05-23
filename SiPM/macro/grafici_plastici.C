#include <iostream>
#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TAxis.h>
#include <TLegend.h>
#include <vector>
#include "txt_reader.cpp"

void grafici_plastici() {

    // parte dedicata ai grafici degli staircase

    vector<vector<double>> dati = txt_reader("../data/txt/Gruppo6_Ch0_708_40dB_Cosm_staircase.txt");

    TH1D *hist = new TH1D("hist", "Staircase plot", 100, 0, 205);

    for (int i=0; i<195; i++) {
        hist->Fill(dati[0][i], dati[1][i]);
    }

    TCanvas *c1 = new TCanvas("c1", "Staircase Plot", 800, 600);
    hist->Draw();
    hist->GetXaxis()->SetTitle ("Soglia [mV]"); 
    hist->GetYaxis()->SetTitle ("Frequenza [kHz]");
    c1->SetLogy();
    //c1->SaveAs("../plots/Staircase_DC_Ch0.png");


    //grafici dello scaler, singole e coincidenze

    // la prima riga di ogni asse y è lo scaler 1, la seconda lo scaler 2, la terza le coincidenze
    // ---- CURVA 1 ---- solo dark count
    const int n1 = 12;
    //double y1[n1] = {611.178, 670.881, 649.682, 109.4085, 90.561, 54.6598, 14.303, 11.10056, 4.52493, 1.801305, 1.319085, 0.4403};   //scaler1
    //double y1[n1] = {523.061, 546.937, 152.001, 75.4665, 61.6585, 12.3994, 9.358773333, 5.29293, 1.494005, 1.1206375, 0.416765, 0.1894}; //scaler2
    double y1[n1] = {37332, 42052, 11331, 939.5, 644, 82.06666667, 15.58666667, 6.49, 0.9, 0.285, 0.0725, 0.0067}; //coincidenze
    double x1[n1] = {5, 15, 25, 35, 45, 55, 65, 75, 85, 95, 105, 115};
    TGraph *g1 = new TGraph(n1, x1, y1);
    g1->SetLineColor(kRed);       // Linea rossa
    g1->SetLineWidth(2);
    g1->SetMarkerColor(kRed);     // Punti rossi
    g1->SetMarkerStyle(20);       // Cerchio pieno

    // ---- CURVA 2---- dark count + cosmici
    const int n2 = 13;
    //double y2[n2] = {609.93, 670.80, 671.01, 120.66, 99.12, 74.65, 17.46, 13.59, 8.45, 2.47, 1.86, 1.04, 0.31};  //scaler1
    //double y2[n2] = {626.42, 605.78, 250.92, 92.89, 87.30, 22.33, 14.43, 10.07, 2.56, 1.36, 0.92, 0.34, 0.19}; //scaler2
    double y2[n2] = {45151.00, 47019.00, 19304.00, 1267.00, 1018.50, 196.13, 33.55, 18.65, 3.48, 1.47, 0.78, 0.51, 0.41}; //coincidenze
    double x2[n2] = {5, 15, 25, 35, 45, 55, 65, 75, 85, 95, 105, 115, 125};
    TGraph *g2 = new TGraph(n2, x2, y2);
    g2->SetLineColor(kBlue);      // Linea blu
    g2->SetLineWidth(2);
    g2->SetMarkerColor(kBlue);    // Punti blu
    g2->SetMarkerStyle(21);       // Quadrato pieno

    // ----- CURVA 3 ---- dark count + cosmici + stronzio90

    const int n3 = 13;
    //double y3[n3] = {608.209, 670.92, 666.494, 116.67, 95.774, 64.17575, 15.974775, 12.3292, 6.4251, 2.119015, 1.56225, 0.62767, 0.292325};  //scaler1
    //double y3[n3] = {542.81, 567.857, 232.268, 84.857, 72.3255, 17.054125, 11.2815, 7.4505, 1.8866, 1.37975, 0.562745, 0.233315, 0.149865};  //scaler2
    double y3[n3] = {39357, 44556, 18187, 1321, 1054, 226.375, 83.925, 48.38333333, 19.78, 10.945, 5.31, 2.63, 1.31}; //coincidenze
    double x3[n3] = {5, 15, 25, 35, 45, 55, 65, 75, 85, 95, 105, 115, 125};
    TGraph *g3 = new TGraph(n3, x3, y3);
    g3->SetLineColor(kGreen);      // Linea verdi
    g3->SetLineWidth(2);
    g3->SetMarkerColor(kGreen);    // Punti verdi
    g3->SetMarkerStyle(22);       // triangolo pieno

    // ---- IL CONTENITORE: TMultiGraph ----
    TMultiGraph *mg = new TMultiGraph();
    
    // Aggiungi i singoli grafici al multigraph
    // "LP" significa che per quel grafico vuoi disegnare sia le Linee (L) che i Punti (P)
    mg->Add(g1, "L");  //"LP" per avere linee e punti
    mg->Add(g2, "L");
    mg->Add(g3, "L");

    // Imposta il titolo globale del multigraph (TitoloGenerale;AsseX;AsseY)
    mg->SetTitle("Confronto Curve; Soglia [mV]; Conteggi");

    // Crea una legenda posizionata in alto a destra
    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg->SetHeader("Legenda", "C"); // "C" centra il testo del titolo
    leg->AddEntry(g1, "Dark Count", "l");
    leg->AddEntry(g2, "DC + cosmici", "l");
    leg->AddEntry(g3, "DC + cosmici + Sr90", "l");

    // ---- DISEGNO ----
    TCanvas *c2 = new TCanvas("c2", "Canvas", 800, 600);
    c2->SetLogy();
    
    // Disegni il TMultiGraph con l'opzione "A" per fargli generare gli assi automatici
    // che contengano TUTTI i punti di ENTRAMBI i grafici.
    mg->Draw("A"); 
    leg->Draw();




}