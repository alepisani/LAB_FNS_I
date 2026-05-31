#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath> 
#include "letturaFile.cpp"
#include "funzionebrutta.cpp"
using namespace std;

void coinc(){
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111);
    vector<int> angoli = {-32, -30, -28, -26, -24, -22, -20, -18, -14, -10, -5, 0, +5, +10, +14, +18, +20, +22, +24, +26, +28, +30, +32};
    vector<vector<double>> fondo = doubleReader("/Users/sissy/Desktop/LAB_FNS_I/gamma/data/coinc (non usatela)/TAC_Na_fondo_lungo4h_-35deg.mca", 12, false);
    double tempo_fondo = 14409.261000;

    ofstream file_output("/Users/sissy/Desktop/LAB_FNS_I/gamma/data/coinc (non usatela)/risultati_rate.txt");
    if (!file_output.is_open()) {
        cout << "Errore: impossibile creare il file risultati_rate.txt!" << endl;
        return;
    }
    // Intestazione con solo Angolo e Rate Pulito
    file_output << "# Angolo(deg)\tRate_Pulito(Hz)" << endl;    

    // Vettori che conterranno i punti del grafico finale
    vector<double> x_angoli;    // Asse X (gli angoli)
    vector<double> y_rate;      // Asse Y (i rate puliti)
    vector<double> ex_angoli;   // Errore sulla X
    vector<double> ey_rate;     // Errore sulla Y

for (int i = 0; i < angoli.size(); i++) {
        int ang = angoli[i];
        
        // 1. Costruisci correttamente il percorso del file unendo le stringhe con il +
        string percorso_file = "/Users/sissy/Desktop/LAB_FNS_I/gamma/data/coinc (non usatela)/TAC_" + to_string(ang) + "deg_Na_coin.mca";
        
        // 2. Leggi lo spettro con la tua funzione
        vector<vector<double>> coinc = doubleReader(percorso_file, 12, false);
        
        // 3. Ora apriamo lo stesso file per cercare il LIVE_TIME nella testata
        ifstream file_mca(percorso_file);
        if (!file_mca.is_open()) {
            cout << "Errore: impossibile aprire il file per l'angolo " << ang << endl;
            continue;
        }
        
        string riga;
        double live_time = -1.0;
        
        // Leggiamo il file riga per riga alla ricerca del live time
        while (getline(file_mca, riga)) {
            if (riga.find("LIVE_TIME -") != string::npos) {
                size_t pos = riga.find("-");
                live_time = stod(riga.substr(pos + 2)); // Converte la parte dopo il " - " in un numero
                break; // Trovato! Usciamo dal ciclo while
            }
        }
        file_mca.close(); // Ricordati sempre di chiudere il file di testo
        
        // 4. Stampa di controllo per vedere se tutto funziona
        cout << "Angolo: " << ang << "° | Live Time trovato: " << live_time << " secondi" << endl;
        
        // Da qui in poi, per questo angolo, hai sia la matrice 'coinc' con i conteggi 
        // sia la variabile 'live_time' con il tempo reale della misura!

        double fattore_scala= live_time/tempo_fondo;
        TH1D* dati = new TH1D(Form("dati_%d", ang), "Spettro originale", coinc[0].size(), 0, coinc[0].size());
        TH1D* dati_fondo = new TH1D(Form("dati_fondo_%d", ang), "fondo", fondo[0].size(), 0, fondo[0].size());
        TH1D* dati_puliti = new TH1D(Form("dati_puliti_%d", ang), "Spettro pulito", coinc[0].size(), 0, coinc[0].size());

        double count_spettro=0.;
        double count_fondo=0.;
        for (int k = 0; k < coinc[0].size(); k++){
        count_spettro+=coinc[0][k];
        count_fondo+=fondo[0][k];
        dati->SetBinContent(k+1, coinc[0][k]);
        dati_fondo->SetBinContent(k+1, fondo[0][k]*fattore_scala);
        double puliti_canale = coinc[0][k] - fondo[0][k] * fattore_scala;
            if (puliti_canale < 0) puliti_canale = 0;
            dati_puliti->SetBinContent(k+1, puliti_canale);
        
        }

             
    double rate_spettro=count_spettro/live_time;
    double rate_fondo=count_fondo/tempo_fondo;
    double rate_spettropulito=(count_spettro-count_fondo*fattore_scala)/live_time;

    cout<<"Rate spettro: "<< rate_spettro<<endl;
    cout<<"Rate fondo: "<< rate_fondo<<endl;
    cout<<"Rate spettro pulito: "<< rate_spettropulito<<endl;

    file_output << ang << "\t" << rate_spettro << "\t" <<rate_spettropulito << endl;

    //cout << "Angolo: " << ang << "° | Live Time: " << live_time << " s | Rate Pulito: " << rate_spettropulito << " Hz" << endl;


    // Calcolo dell'errore statistico (propagazione delle fluttuazioni di Poisson)
    double errore_netti = sqrt(count_spettro - count_fondo * fattore_scala);
    double errore_rate = errore_netti / live_time;

    // Salva i dati correnti nei contenitori esterni
    x_angoli.push_back(ang);              // Inserisce l'angolo corrente
    y_rate.push_back(rate_spettropulito);  // Inserisce il rate pulito corrente
    ex_angoli.push_back(0);             // Incertezza del goniometro (es. 0.5 gradi)
    ey_rate.push_back(errore_rate);        // Inserisce l'errore sul rate

    /*
    TCanvas* c1 = new TCanvas(Form("c_%d", ang), Form("Spettri Angolo %d", ang), 800, 600);
    
    dati->SetLineColor(kBlue);
    dati->Rebin(2);
    dati->SetXTitle("MCA");
    dati->SetYTitle("Conteggi");
    dati->Draw("HIST");
    dati_puliti->SetLineColor(kGreen);
    dati_puliti->Rebin(2);
    dati_puliti->Draw("HIST SAME");  
    dati_fondo->SetLineColor(kRed);
    dati_fondo->Rebin(2);
    dati_fondo->Draw("HIST SAME");

    TLegend *spettri = new TLegend(0.7,0.7,0.9,0.9);
    spettri->AddEntry(dati, "spettro", "l");
    spettri->AddEntry(dati_fondo, "fondo", "l");
    spettri->AddEntry(dati_puliti, "spettro pulito", "l");
    spettri->SetBorderSize(0);   // toglie bordo
    spettri->SetFillStyle(0);    // trasparente
    spettri->SetTextSize(0.04);  
    spettri->Draw("SAME");

    c1->SaveAs(Form("spettro_angolo_%d.png", ang));
    */

    }

    double R = 9.5;
    double r = 5.547/2;
    double x_0 = 0;
  
    
    // Creiamo un nuovo Canvas dedicato solo al grafico finale
    TCanvas* c_grafico = new TCanvas("c_grafico", "Grafico Finale", 800, 600);
    //c_grafico->SetGrid(); // Mette una comoda griglia sullo sfondo
    TF1* mia_funzione = new TF1("mia_funzione", funzionebrutta, -35, 35, 4);
    mia_funzione->SetParameter(0, 45); //Normalizzazione
    mia_funzione->SetParameter(1, R);
    mia_funzione->SetParameter(2, r); 
    mia_funzione->SetParameter(3, x_0);
    mia_funzione->SetParName(0, "Costante"); // Cambia il nome di par[0]
    mia_funzione->SetParName(1, "R");        // Cambia il nome di par[1]
    mia_funzione->SetParName(2, "r");        // Cambia il nome di par[2]
    mia_funzione->SetParName(3, "#theta_0");
    //mia_funzione->FixParameter(1, R);
    //mia_funzione->FixParameter(2, r);

    // Creiamo il TGraphErrors passando i nostri vettori pieni di dati
    // Il simbolo '&' serve a dire a ROOT dove iniziano i dati in memoria
    TGraphErrors* ge = new TGraphErrors(x_angoli.size(), &x_angoli[0], &y_rate[0], &ex_angoli[0], &ey_rate[0]);

    // Abbelliamo il grafico con i titoli degli assi
    ge->SetTitle("Rate di Coincidenza in funzione dell'Angolo");
    ge->GetXaxis()->SetTitle("Angolo #theta (gradi)");
    ge->GetYaxis()->SetTitle("Rate (Hz)");

    // Scegliamo lo stile dei punti (20 è il cerchietto pieno) e il colore
    ge->SetMarkerStyle(20);
    ge->SetMarkerSize(1.2);
    ge->SetMarkerColor(kBlue+1);
    ge->SetLineColor(kRed+1); // Colore delle barrette d'errore

   

    // Disegniamo il grafico: A = disegna gli Assi, P = disegna i Punti con l'errore
    ge->Draw("AP");
     //Facciamo il fit
    ge->Fit("mia_funzione", "R");
    //Disegniamo la funzione
    mia_funzione->SetLineColor(kRed);
    mia_funzione->SetLineWidth(3);
    //mia_funzione->Draw();

    // Salviamo il grafico finale come immagine sul PC
    c_grafico->SaveAs("grafico_rate_vs_angolo.png");


    
    

}