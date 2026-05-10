#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "letturaFile.cpp"
#include <vector>
#include <cmath>

void patternunit(){
    vector<vector<int>> data = datReader("../data/quintupla_lunga/Pattern_quintupla_test.dat", 0, false);

    vector<int> molteplicita;
    vector<int> pattern;

    TH1D* PatternUnit = new TH1D("h1", "Pattern Unit", 8, -0.5, 7.5);

    //per usare datReader sul file della pattern unit che ha solo 2 colonne bisogna commentare "output.erase(output.begin());" se no rimane un'unica colonna

    for (int i=0; i<data[0].size(); i++){
        molteplicita.push_back(data[0][i]);
        pattern.push_back(data[1][i]);

        PatternUnit->Fill(data[1][i]);
    }

    // 1. Accesso all'asse X
    TAxis *xaxis = PatternUnit->GetXaxis();

    // 2. Assegnazione delle label
    // Supponendo che tu voglia rinominare gli 8 bin (da 1 a 8)
    const char *labels[8] = {"0_{10} = 000_{2}", "1_{10} = 001_{2}", "2_{10} = 010_{2}", 
                            "3_{10} = 011_{2}", "4_{10} = 100_{2}", "5_{10} = 101_{2}", "6_{10} = 110_{2}", "7_{10} = 111_{2}"};

    for (int i = 1; i <= 8; i++) {
        xaxis->SetBinLabel(i, labels[i-1]);
    }

    // 3. Ottimizzazione estetica (Opzionale)
    // Se le label sono lunghe, puoi ruotarle per evitarne la sovrapposizione
    // "v" = verticale, "h" = orizzontale, "u" = up (45°), "d" = down (45°)
    xaxis->LabelsOption("h"); 

    // Aumenta la dimensione se necessario
    xaxis->SetLabelSize(0.04);

    TCanvas* c1 = new TCanvas("c1", "Pattern Unit", 800, 600);
    PatternUnit->Draw();
    c1->Update();
    c1->SaveAs("../plots/Pattern_unit.pdf");   



}