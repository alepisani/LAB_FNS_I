#include "TH1.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "letturaFile.cpp"
#include <string>
#include <vector>

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

vector<double> soglie = {30.0, 40.3, 43.7, 46.7, 50.7, 55.1, 58.5, 63.3, 68.2, 72.5, 75.0};
int npoints = soglie.size();

vector<vector<int>> data;

//inizializziamo le variabili di conteggio del loop
//sono double perché dovremo calcolare il rapporto
double triple;
double doppie;
double doppie_403 = 0.;
double triple_403 = 0.;

//inizializziamo un TGraphAsymmErrors per graficare triple/doppie vs soglia
TGraphAsymmErrors* gEff8 = new TGraphAsymmErrors(npoints);

//livello di confidenza per la statistica
double cl = 0.6827; //1 sigma

//loop sui valori di soglia
for(int i=0; i<npoints; i++){
    data = datReader(std::string(TString::Format("../data/doppie_67/TDC_doppia67_thr%.0f_1000.dat", soglie[i]*10).Data()), 0);
    doppie=0;
    triple=0;
    for(int ev=0; ev<data[0].size(); ev++){
        doppie++;
        if(data[8][ev] < 3000) triple++;
    }

    if(i==1){
        doppie_403 = doppie;
        triple_403 = triple;
    }
    
    //riempiamo TGraphAsymmErrors con errori binomiali (Clopper-Pearson)
    double eff = triple/doppie;
    double el = eff - TEfficiency::ClopperPearson(doppie, triple, cl, false); //errore inferiore
    double eh = TEfficiency::ClopperPearson(doppie, triple, cl, true) - eff; //errore inferiore

    //output di controllo
    printf("soglia: %.1f mV, triple: %.f, doppie: %.f, percentuale: %.1f\n", soglie[i], triple, doppie, eff*100);

    gEff8->SetPoint(i, soglie[i], eff);
    gEff8->SetPointError(i, -0.1, 0.1, el, eh);

    data.clear();

}

gEff8->Draw();

//_____LETTURA DATI MONTECARLO__________
vector<vector<double>> dataMC = doubleReader("original_macros/montecarlosim.txt");



TGraphAsymmErrors* gMC = new TGraphAsymmErrors(nMC);






}