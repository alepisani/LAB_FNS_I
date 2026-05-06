#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "letturaFile.cpp"
#include <vector>
#include <cmath>

void patternunit(){
    vector<vector<int>> data = datReader("../data/quintupla_lunga/Pattern_quintupla_test.dat", 0);

    vector<int> molteplicita;
    vector<int> pattern;

    TH1D* PatternUnit = new TH1D("h1", "Pattern Unit", 8, -0.5, 7.5);

    //per usare datReader sul file della pattern unit che ha solo 2 colonne bisogna commentare "output.erase(output.begin());" se no rimane un'unica colonna

    for (int i=0; i<data[0].size(); i++){
        molteplicita.push_back(data[0][i]);
        pattern.push_back(data[1][i]);

        PatternUnit->Fill(data[1][i]);
    }

    TCanvas* c1 = new TCanvas("c1", "Pattern Unit", 800, 600);
    PatternUnit->Draw();


}