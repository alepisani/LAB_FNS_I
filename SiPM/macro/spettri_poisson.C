#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// macro per l'analisi degli spettri di poisson di un SiPM con un LED a tre diverse
// intensità, 2.4, 2.7 e 3.7 unità arbitrarie (ua)

void spettri_poisson(){

    // lettura file
    vector<vector<int>> data_24 = datReader("708V_30dB_24ua_histo.txt", header=8, erase1col=false);
    vector<vector<int>> data_27 = datReader("708V_30dB_27ua_histo.txt", header=8, erase1col=false);
    vector<vector<int>> data_30 = datReader("708V_30dB_30ua_pp_histo.txt", header=8, erase1col=false);

    int dpp = {340, }





}