#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TKey.h"
#include <vector>
#include <string>
#include <iostream>

/**
 * Script per visualizzare tutti gli istogrammi dai file .root nella cartella landgauss_path
 * Crea un canvas diviso in 8 sottopanelli con conteggi normalizzati e asse Y fisso
 */

void display_histograms(){
    
    std::string data_path = "../data/singole/landgauss_path/";
    
    std::vector<std::string> root_files = {
        "ADC4_singola_HV2050_THR85_1000.root",
        "ADC4_singola_HV2070_THR95_5000.root",
        "ADC4_singola_HV2070_THR97_5000.root",
        "ADC4_singola_HV2070_THR98_5000.root",
        "ADC4_singola_HV2070_THR100_5000.root",
        "ADC4_singola_HV2070_THR106_5000.root",
        "ADC4_singola_HV2070_THR107_5000.root",
        "ADC4_singola_HV2080_THR105_1000.root"
    };
    
    // --- Prima passata: carica tutti gli istogrammi e trova il massimo globale ---
    std::vector<TH1D*> histograms;
    double global_max = 0.0;

    for(int i = 0; i < (int)root_files.size(); i++){
        std::string full_path = data_path + root_files[i];
        
        TFile *file = TFile::Open(full_path.c_str());
        if(!file || file->IsZombie()){
            std::cerr << "Errore nell'apertura del file: " << full_path << std::endl;
            histograms.push_back(nullptr);
            continue;
        }
        
        TH1D *hist_before_rebinning = (TH1D*)file->Get("adc4");
        if(!hist_before_rebinning){
            std::cerr << "Istogramma 'adc4' non trovato in: " << full_path << std::endl;
            file->Close();
            histograms.push_back(nullptr);
            continue;
        }
        
        TH1D *hist = (TH1D*)hist_before_rebinning->Rebin(2, ("hist_" + std::to_string(i)).c_str());
        hist->SetDirectory(0);
        file->Close();

        // Normalizza al numero di entries
        double nEntries = hist->GetEntries();
        if(nEntries > 0) hist->Scale(1.0 / nEntries);

        // Considera solo il range di interesse per trovare il massimo
        hist->GetXaxis()->SetRangeUser(25, 200);
        double local_max = hist->GetMaximum();
        if(local_max > global_max) global_max = local_max;
        // Resetta il range (verrà reimpostato al momento del disegno)
        hist->GetXaxis()->SetRange(0, 0);

        histograms.push_back(hist);
    }

    // Aggiungi un margine del 10% sopra il massimo
    double y_max = global_max * 1.01;

    // --- Seconda passata: disegna con asse Y fisso ---
    TCanvas *canvas = new TCanvas("c_histograms", "Histograms from landgauss_path", 600, 1600);
    canvas->Divide(2, 4);
    
    for(int i = 0; i < (int)root_files.size(); i++){
        if(!histograms[i]) continue;

        canvas->cd(i + 1);
        histograms[i]->GetXaxis()->SetTitle("Canali");
        histograms[i]->GetYaxis()->SetTitle("Conteggi / Entries");
        histograms[i]->GetXaxis()->SetRangeUser(25, 200);
        histograms[i]->GetYaxis()->SetRangeUser(0, y_max);  // Asse Y fisso uguale per tutti
        histograms[i]->SetTitle(root_files[i].c_str());
        histograms[i]->Draw("HIST");
    }
    
    canvas->Update();
    canvas->SaveAs("../plots/landgauss/all_histograms_comparison.pdf");
    
    std::cout << "Visualizzazione completata! Y max globale = " << y_max << std::endl;
}