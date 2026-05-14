#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "letturaFile.cpp"
#include "landau_gauss.C"
#include <vector>
#include <cmath>

void patternunit(){
    vector<vector<int>> data = datReader("../data/quintupla_lunga/Pattern_quintupla_test.dat", 0, false);

    vector<int> multiplicity;
    vector<int> pattern;

    TH1D* PatternUnit = new TH1D("h1", "Pattern Unit", 8, -0.5, 7.5);

    //per usare datReader sul file della pattern unit che ha solo 2 colonne bisogna commentare "output.erase(output.begin());" se no rimane un'unica colonna

    for (int i=0; i<data[0].size(); i++){
        multiplicity.push_back(data[0][i]);
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
    c1->SaveAs("../plots/patternunit/Pattern_unit.pdf");   

//-----------------------------------------------------------------------------------------------------------

/**
* cerchiamo di capire il significato di avere tanti conteggi a 0 nella misura di PU
* quindi proviamo la seguente analisi
* per ogni valore di molteplicità a 0 nella PU prendiamo i valori di quell'evento 
* in ADC e cerchiamo di capire quante particelle sono passate.
* Come?
* prendi il valore di adc (per 6-7-8) togli piedistallo e lo dividiamo per il picco della Landau.
* if x < 0.5 nessuna particella; if 0.5 < x < 1.5; if 1.5 < x < 2.5 and so on. 
* magari prova a plottare un istogramma con il numero di particelle
*/

//prendo i dati dei picchi della landau generati da landau_gauss.C
ifstream params_file("../data/landau_params.txt");
double mu1, mu2, mu3, mu4, mu5, mu6;
params_file >> mu1 >> mu2 >> mu3 >> mu4 >> mu5 >> mu6;

//prendiamo i dati dalla quintupla
int nEvents_quintupla = 5000;
vector<vector<double>> data_ADC_quintupla = doubleReader("../data/quintupla_lunga/ADC_quintupla_5000.dat", 0);

//creiamo histos per capire quante particelle incidono
TH1D* Npart_ADC1_quintupla = new TH1D("hadc1", "Npart_ADC1_quintipla", 10, -0.5, 9.5);
TH1D* Npart_ADC2_quintupla = new TH1D("hadc2", "Npart_ADC2_quintipla", 10, -0.5, 9.5);
TH1D* Npart_ADC3_quintupla = new TH1D("hadc3", "Npart_ADC3_quintipla", 10, -0.5, 9.5);
TH1D* Npart_ADC4_quintupla = new TH1D("hadc4", "Npart_ADC4_quintipla", 10, -0.5, 9.5);
TH1D* Npart_ADC5_quintupla = new TH1D("hadc5", "Npart_ADC5_quintipla", 10, -0.5, 9.5);
TH1D* Npart_ADC6_quintupla = new TH1D("hadc6", "Npart_ADC6_quintipla", 10, -0.5, 9.5);


vector<double> adc1; adc1.reserve(nEvents_quintupla);
vector<double> adc2; adc2.reserve(nEvents_quintupla);
vector<double> adc3; adc3.reserve(nEvents_quintupla);
vector<double> adc4; adc4.reserve(nEvents_quintupla);
vector<double> adc5; adc5.reserve(nEvents_quintupla);
vector<double> adc6; adc6.reserve(nEvents_quintupla);
vector<double> adc7; adc7.reserve(nEvents_quintupla);
vector<double> adc8; adc8.reserve(nEvents_quintupla);

//riempiamo i vector con i dati degli adc per ogni evento
for (int i = 0; i < data_ADC_quintupla[0].size(); i++){

    adc1.push_back(data_ADC_quintupla[1][i]);
    adc2.push_back(data_ADC_quintupla[2][i]);
    adc3.push_back(data_ADC_quintupla[3][i]);
    adc4.push_back(data_ADC_quintupla[4][i]);
    adc5.push_back(data_ADC_quintupla[5][i]);
    adc6.push_back(data_ADC_quintupla[6][i]);
    adc7.push_back(data_ADC_quintupla[7][i]);
    adc8.push_back(data_ADC_quintupla[8][i]);

    //fill histo ADC1, ADC2, ADC3, ADC4, ADC5, ADC6
    //if(!pattern[i]){
    if(pattern[i] == 7){

        double Npart_ADC1 = data_ADC_quintupla[1][i] / mu1; 
        if(Npart_ADC1 <= 0.5) Npart_ADC1_quintupla->Fill(Npart_ADC1);
        if(Npart_ADC1 > 0.5 && Npart_ADC1 <= 1.5) Npart_ADC1_quintupla->Fill(Npart_ADC1);
        if(Npart_ADC1 > 1.5 && Npart_ADC1 <= 2.5) Npart_ADC1_quintupla->Fill(Npart_ADC1);
        if(Npart_ADC1 > 2.5 && Npart_ADC1 <= 3.5) Npart_ADC1_quintupla->Fill(Npart_ADC1);
        if(Npart_ADC1 > 3.5 && Npart_ADC1 <= 4.5) Npart_ADC1_quintupla->Fill(Npart_ADC1);
        if(Npart_ADC1 > 4.5 && Npart_ADC1 <= 5.5) Npart_ADC1_quintupla->Fill(Npart_ADC1);
        if(Npart_ADC1 > 5.5 && Npart_ADC1 <= 6.5) Npart_ADC1_quintupla->Fill(Npart_ADC1);
        if(Npart_ADC1 > 6.5 && Npart_ADC1 <= 7.5) Npart_ADC1_quintupla->Fill(Npart_ADC1);
        if(Npart_ADC1 > 7.5 && Npart_ADC1 <= 8.5) Npart_ADC1_quintupla->Fill(Npart_ADC1);
        if(Npart_ADC1 > 8.5) Npart_ADC1_quintupla->Fill(Npart_ADC1);

        double Npart_ADC2 = data_ADC_quintupla[2][i] / mu2; 
        if(Npart_ADC2 <= 0.5) Npart_ADC2_quintupla->Fill(Npart_ADC2);
        if(Npart_ADC2 > 0.5 && Npart_ADC2 <= 1.5) Npart_ADC2_quintupla->Fill(Npart_ADC2);
        if(Npart_ADC2 > 1.5 && Npart_ADC2 <= 2.5) Npart_ADC2_quintupla->Fill(Npart_ADC2);
        if(Npart_ADC2 > 2.5 && Npart_ADC2 <= 3.5) Npart_ADC2_quintupla->Fill(Npart_ADC2);
        if(Npart_ADC2 > 3.5 && Npart_ADC2 <= 4.5) Npart_ADC2_quintupla->Fill(Npart_ADC2);
        if(Npart_ADC2 > 4.5 && Npart_ADC2 <= 5.5) Npart_ADC2_quintupla->Fill(Npart_ADC2);
        if(Npart_ADC2 > 5.5 && Npart_ADC2 <= 6.5) Npart_ADC2_quintupla->Fill(Npart_ADC2);
        if(Npart_ADC2 > 6.5 && Npart_ADC2 <= 7.5) Npart_ADC2_quintupla->Fill(Npart_ADC2);
        if(Npart_ADC2 > 7.5 && Npart_ADC2 <= 8.5) Npart_ADC2_quintupla->Fill(Npart_ADC2);
        if(Npart_ADC2 > 8.5) Npart_ADC2_quintupla->Fill(Npart_ADC2);

        double Npart_ADC3 = data_ADC_quintupla[3][i] / mu3; 
        if(Npart_ADC3 <= 0.5) Npart_ADC3_quintupla->Fill(Npart_ADC3);
        if(Npart_ADC3 > 0.5 && Npart_ADC3 <= 1.5) Npart_ADC3_quintupla->Fill(Npart_ADC3);
        if(Npart_ADC3 > 1.5 && Npart_ADC3 <= 2.5) Npart_ADC3_quintupla->Fill(Npart_ADC3);
        if(Npart_ADC3 > 2.5 && Npart_ADC3 <= 3.5) Npart_ADC3_quintupla->Fill(Npart_ADC3);
        if(Npart_ADC3 > 3.5 && Npart_ADC3 <= 4.5) Npart_ADC3_quintupla->Fill(Npart_ADC3);
        if(Npart_ADC3 > 4.5 && Npart_ADC3 <= 5.5) Npart_ADC3_quintupla->Fill(Npart_ADC3);
        if(Npart_ADC3 > 5.5 && Npart_ADC3 <= 6.5) Npart_ADC3_quintupla->Fill(Npart_ADC3);
        if(Npart_ADC3 > 6.5 && Npart_ADC3 <= 7.5) Npart_ADC3_quintupla->Fill(Npart_ADC3);
        if(Npart_ADC3 > 7.5 && Npart_ADC3 <= 8.5) Npart_ADC3_quintupla->Fill(Npart_ADC3);
        if(Npart_ADC3 > 8.5) Npart_ADC3_quintupla->Fill(Npart_ADC3);

        double Npart_ADC4 = data_ADC_quintupla[4][i] / mu4; 
        if(Npart_ADC4 <= 0.5) Npart_ADC4_quintupla->Fill(Npart_ADC4);
        if(Npart_ADC4 > 0.5 && Npart_ADC4 <= 1.5) Npart_ADC4_quintupla->Fill(Npart_ADC4);
        if(Npart_ADC4 > 1.5 && Npart_ADC4 <= 2.5) Npart_ADC4_quintupla->Fill(Npart_ADC4);
        if(Npart_ADC4 > 2.5 && Npart_ADC4 <= 3.5) Npart_ADC4_quintupla->Fill(Npart_ADC4);
        if(Npart_ADC4 > 3.5 && Npart_ADC4 <= 4.5) Npart_ADC4_quintupla->Fill(Npart_ADC4);
        if(Npart_ADC4 > 4.5 && Npart_ADC4 <= 5.5) Npart_ADC4_quintupla->Fill(Npart_ADC4);
        if(Npart_ADC4 > 5.5 && Npart_ADC4 <= 6.5) Npart_ADC4_quintupla->Fill(Npart_ADC4);
        if(Npart_ADC4 > 6.5 && Npart_ADC4 <= 7.5) Npart_ADC4_quintupla->Fill(Npart_ADC4);
        if(Npart_ADC4 > 7.5 && Npart_ADC4 <= 8.5) Npart_ADC4_quintupla->Fill(Npart_ADC4);
        if(Npart_ADC4 > 8.5) Npart_ADC4_quintupla->Fill(Npart_ADC4);

        double Npart_ADC5 = data_ADC_quintupla[5][i] / mu5; 
        if(Npart_ADC5 <= 0.5) Npart_ADC5_quintupla->Fill(Npart_ADC5);
        if(Npart_ADC5 > 0.5 && Npart_ADC5 <= 1.5) Npart_ADC5_quintupla->Fill(Npart_ADC5);
        if(Npart_ADC5 > 1.5 && Npart_ADC5 <= 2.5) Npart_ADC5_quintupla->Fill(Npart_ADC5);
        if(Npart_ADC5 > 2.5 && Npart_ADC5 <= 3.5) Npart_ADC5_quintupla->Fill(Npart_ADC5);
        if(Npart_ADC5 > 3.5 && Npart_ADC5 <= 4.5) Npart_ADC5_quintupla->Fill(Npart_ADC5);
        if(Npart_ADC5 > 4.5 && Npart_ADC5 <= 5.5) Npart_ADC5_quintupla->Fill(Npart_ADC5);
        if(Npart_ADC5 > 5.5 && Npart_ADC5 <= 6.5) Npart_ADC5_quintupla->Fill(Npart_ADC5);
        if(Npart_ADC5 > 6.5 && Npart_ADC5 <= 7.5) Npart_ADC5_quintupla->Fill(Npart_ADC5);
        if(Npart_ADC5 > 7.5 && Npart_ADC5 <= 8.5) Npart_ADC5_quintupla->Fill(Npart_ADC5);
        if(Npart_ADC5 > 8.5) Npart_ADC5_quintupla->Fill(Npart_ADC5);

        double Npart_ADC6 = data_ADC_quintupla[6][i] / mu6; 
        if(Npart_ADC6 <= 0.5) Npart_ADC6_quintupla->Fill(Npart_ADC6);
        if(Npart_ADC6 > 0.5 && Npart_ADC6 <= 1.5) Npart_ADC6_quintupla->Fill(Npart_ADC6);
        if(Npart_ADC6 > 1.5 && Npart_ADC6 <= 2.5) Npart_ADC6_quintupla->Fill(Npart_ADC6);
        if(Npart_ADC6 > 2.5 && Npart_ADC6 <= 3.5) Npart_ADC6_quintupla->Fill(Npart_ADC6);
        if(Npart_ADC6 > 3.5 && Npart_ADC6 <= 4.5) Npart_ADC6_quintupla->Fill(Npart_ADC6);
        if(Npart_ADC6 > 4.5 && Npart_ADC6 <= 5.5) Npart_ADC6_quintupla->Fill(Npart_ADC6);
        if(Npart_ADC6 > 5.5 && Npart_ADC6 <= 6.5) Npart_ADC6_quintupla->Fill(Npart_ADC6);
        if(Npart_ADC6 > 6.5 && Npart_ADC6 <= 7.5) Npart_ADC6_quintupla->Fill(Npart_ADC6);
        if(Npart_ADC6 > 7.5 && Npart_ADC6 <= 8.5) Npart_ADC6_quintupla->Fill(Npart_ADC6);
        if(Npart_ADC6 > 8.5) Npart_ADC6_quintupla->Fill(Npart_ADC6);
    }

}


//cout << mu1 << " " << mu2 << " " << mu3 << " " << mu4 << " " << mu5 << " " << mu6 << " " << endl;

TCanvas* cADC1 = new TCanvas("cADC1", "partADC1", 800, 600);
Npart_ADC1_quintupla->SetTitle("# particelle ADC1, 111_{2} = 7_{10}");
Npart_ADC1_quintupla->GetXaxis()->SetTitle("N_{particelle}");
Npart_ADC1_quintupla->GetYaxis()->SetTitle("Conteggi");
Npart_ADC1_quintupla->Draw();
cADC1->Update();
cADC1->SaveAs("../plots/patternunit/Npart_ADC1.pdf");

TCanvas* cADC2 = new TCanvas("cADC2", "partADC2", 800, 600);
Npart_ADC2_quintupla->SetTitle("# particelle ADC2, 111_{2} = 7_{10}");
Npart_ADC2_quintupla->GetXaxis()->SetTitle("N_{particelle}");
Npart_ADC2_quintupla->GetYaxis()->SetTitle("Conteggi");
Npart_ADC2_quintupla->Draw();
cADC2->Update();
cADC2->SaveAs("../plots/patternunit/Npart_ADC2.pdf");

TCanvas* cADC3 = new TCanvas("cADC3", "partADC3", 800, 600);
Npart_ADC3_quintupla->SetTitle("# particelle ADC3, 111_{2} = 7_{10}");
Npart_ADC3_quintupla->GetXaxis()->SetTitle("N_{particelle}");
Npart_ADC3_quintupla->GetYaxis()->SetTitle("Conteggi");
Npart_ADC3_quintupla->Draw();
cADC3->Update();
cADC3->SaveAs("../plots/patternunit/Npart_ADC3.pdf");

TCanvas* cADC4 = new TCanvas("cADC4", "partADC4", 800, 600);
Npart_ADC4_quintupla->SetTitle("# particelle ADC4, 111_{2} = 7_{10}");
Npart_ADC4_quintupla->GetXaxis()->SetTitle("N_{particelle}");
Npart_ADC4_quintupla->GetYaxis()->SetTitle("Conteggi");
Npart_ADC4_quintupla->Draw();
cADC4->Update();
cADC4->SaveAs("../plots/patternunit/Npart_ADC4.pdf");

TCanvas* cADC5 = new TCanvas("cADC5", "partADC5", 800, 600);
Npart_ADC5_quintupla->SetTitle("# particelle ADC5, 111_{2} = 7_{10}");
Npart_ADC5_quintupla->GetXaxis()->SetTitle("N_{particelle}");
Npart_ADC5_quintupla->GetYaxis()->SetTitle("Conteggi");
Npart_ADC5_quintupla->Draw();
cADC5->Update();
cADC5->SaveAs("../plots/patternunit/Npart_ADC5.pdf");

TCanvas* cADC6 = new TCanvas("cADC6", "partADC6", 800, 600);
Npart_ADC6_quintupla->SetTitle("# particelle ADC6, 111_{2} = 7_{10}");
Npart_ADC6_quintupla->GetXaxis()->SetTitle("N_{particelle}");
Npart_ADC6_quintupla->GetYaxis()->SetTitle("Conteggi");
Npart_ADC6_quintupla->Draw();
cADC6->Update();
cADC6->SaveAs("../plots/patternunit/Npart_ADC6.pdf");

}