#include "TH1.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "letturaFile.cpp"
#include "TGraph.h"
#include "TF1.h"
#include "TCanvas.h"
#include <string>
#include <vector>
#include <numeric>

void accettanza(){
    
/*
Macro per misurare il fattore di accettanza geometrica di tre scintillatori sovrapposti
e valutare la potenza di cos(theta) della direziione di provenienza dei RC
- leggere file montecarlo.txt
- misurare triple/doppie montecarlo con TEfficiency
- leggere tutti i .dat
- riempire TGraphAsymmErrors per tutti i valori di soglia
- valutare n per la prima soglia (40.3 mV) 
- valutare n considerando l'efficienza dell'8
*/

double eff7 = 0.96; // da integrare con il valore misurato

bool errori_CP = false;  //errore calcolato con Clopper-Pearson, se false calcolato manualmente con la binomiale

// ------------- DA CAMBIARE -----------------------
bool calcolo_mediato = true;   //true per calcolare n dalla media delle efficienze nel plateau
int evento_di_interesse=2;

vector<double> soglie = {30.0, 40.3, 43.7, 46.7, 50.7, 55.1, 58.5, 63.3, 68.2, 72.5, 75.0};
int npoints = soglie.size();

vector<vector<int>> data;   //vettore per lettura file .dat del TDC

//inizializziamo le variabili di conteggio del loop
//sono double perché dovremo calcolare il rapporto
double triple;
double doppie;
double points_counter=0.; //per contare i punti su cui calcoliamo l'eff media

//livello di confidenza per la statistica
double cl = 0.6827; //1 sigma

gStyle->SetOptFit(1111);
//gStyle->SetMarkerSize(1);
//gStyle->SetMarkerStyle(20);

//_____LETTURA DATI MONTECARLO__________
vector<vector<double>> dataMC = doubleReader("original_macros/montecarlosim.txt", 0, false);
int nMC = dataMC[1].size();
double nevents = 50000; // del MC

// dichiarazione grafico per i punti (errori non ottenibili dal MC non avendo il numero assoluto di entries)
TGraphErrors* gMC = new TGraphErrors(nMC);
// riempimento grafico
for(int i=0; i<nMC; i++){
    double n_i = dataMC[0][i];        
    if(dataMC[1][i]!=0){        
        double G2 = dataMC[1][i]/100;
        double G3 = dataMC[2][i]/100;
        double frac = G3/G2; //
        double e_frac = frac*sqrt((1-frac)/nevents);   //spiegazione errore in relazione
        //cout << dataMC[0][i] << " " << dataMC[1][i] << " " << dataMC[2][i] << endl;
        gMC->SetPoint(i, n_i, frac);
        gMC->SetPointError(i, 0, e_frac);   
    }
    

}

TF1* MC_fit = new TF1("MC_fit", "[0]+[1]*x+[2]*x*x", -0.1, 10); //fit quadratico
//TF1* MC_fit = new TF1("MC_fit", "[0]+[1]*x", 0, 10);
gMC->Fit(MC_fit);

TCanvas* cMC = new TCanvas("cMC", "MC", 1200, 800);
gMC->SetMarkerStyle(33);
gMC->SetMarkerColor(8);
gMC->SetMarkerSize(1.2);
gMC->SetTitle("Accettanza relativa in funzione di n (da simulazione MC)");
gMC->GetXaxis()->SetTitle("n");
gMC->GetYaxis()->SetTitle("G_{3}/G_{2}");
gMC->Draw("AP");
cMC->Print("../plots/accettanza/MC_accettanza.pdf");



//inizializziamo un TGraphAsymmErrors per graficare triple/doppie vs soglia
TGraphAsymmErrors* gEff8 = new TGraphAsymmErrors(npoints);
double eff_media, e_eff, n, n_inf, n_sup;
double somma_sq_err=0;
vector<double> efficienze;

//loop sui valori di soglia
for(int i=0; i<npoints; i++){
    data = datReader(std::string(TString::Format("../data/doppie_67/TDC_doppia67_thr%.0f_1000.dat", soglie[i]*10).Data()), 0);
    if(data.empty() || data[0].empty()) {
        cerr << "Errore lettura file!" << endl;
        continue;
    }
    doppie=0;
    triple=0;
    for(int ev=0; ev<(int)data[0].size(); ev++){
        doppie++;
        if(data[8][ev] < 3000) triple++;
    }

    if(doppie<1) continue;
    
    //riempiamo TGraphAsymmErrors con errori binomiali (Clopper-Pearson o manuali)
    double eff = triple/doppie;
    double el, eh;
    if(errori_CP){
        el = eff - TEfficiency::ClopperPearson(doppie, triple, cl, false); //errore inferiore
        eh = TEfficiency::ClopperPearson(doppie, triple, cl, true) - eff; //errore inferiore
    } else{
        el = sqrt(eff*(1-eff)/doppie);
        if((eff + el) > 1.) eh = 1 - eff;
        else eh = el;                       // se lontano da 1, errori simmetrici
    }

    if(i==evento_di_interesse && !calcolo_mediato){       //calcolo standard
        // calcolo di n per la soglia preimpostata
        n = MC_fit->GetX(eff/eff7);
        n_inf = MC_fit->GetX((eff-el)/eff7);
        n_sup = MC_fit->GetX((eff+eh)/eff7);

        cout << "n = " << n << " - " << n-n_inf << " + " << n_sup - n << endl;

    } else if (i<7 && calcolo_mediato){
        efficienze.push_back(triple/doppie);
    }

    //output di controllo
    //printf("soglia: %.1f mV, triple: %.f, doppie: %.f, percentuale: %.1f\n", soglie[i], triple, doppie, eff*100);

    gEff8->SetPoint(i, soglie[i], eff);
    gEff8->SetPointError(i, 0.1, 0.1, el, eh);

    data.clear();

}

//calcolo di n con l'eff media
if (calcolo_mediato){
    eff_media = std::accumulate(efficienze.begin(), efficienze.end(), 0.0)/efficienze.size(); 
    n = MC_fit->GetX(eff_media/eff7);

    //calcolo errore
    for (auto elem : efficienze) somma_sq_err += (elem-eff_media)*(elem-eff_media);
    e_eff = sqrt(somma_sq_err)/efficienze.size();

    n_inf = MC_fit->GetX((eff_media-e_eff)/eff7);
    n_sup = MC_fit->GetX((eff_media+e_eff)/eff7);



    cout << "efficienza = " << eff_media << ", n = " << n << " - " << n-n_inf << " + " << n_sup - n << endl;
}


TCanvas* c2 = new TCanvas("c2", "frazione eventi persi", 1200, 800);
gEff8->SetTitle("Frazione di eventi rivelati dallo scintillatore 8");
gEff8->GetXaxis()->SetTitle("soglia [mV]");
gEff8->GetYaxis()->SetTitle("G_{3}/G_{2}");
gEff8->SetMarkerStyle(20);
gEff8->SetMarkerSize(2);
gEff8->SetMarkerColor(8);
gEff8->SetLineWidth(2);
gEff8->SetMinimum(0.15);
gEff8->SetMaximum(0.4);
gEff8->Draw("AP");
c2->Print("../plots/accettanza/fraz8_vs_thr.pdf");



/*dubbi
- fit del MC senza errori?
- ok calcolare n con la media delle efficienze nel plateau?

*/


}