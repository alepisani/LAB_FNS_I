#include "letturaFile.cpp"
#include "TH1.h"
#include "TAxis.h"

void doppie_diagonali(){
    vector<vector<int>> data = datReader("../data/TDC_diagonale42_3150.dat");

    int lim = 3000;

    //istogramma simil pattern unit: coincidenze in presa diagonale doppia 4-2
    TH1I* pu = new TH1I("pu", "pattern unit tetto", 8, 0.5, 8.5);
    // 42, 421, 423, 425, 4213, 4215, 4235, 42135 --> 8 combinazioni
    // 1    2    3   4     5     6     7     8

    for(int row=0; row<data[0].size(); row++){
             if (data[1][row]<lim & data[3][row]<lim & data[5][row]<lim) pu->Fill(1);
        else if (data[1][row]>lim & data[3][row]<lim & data[5][row]<lim) pu->Fill(2);
        else if (data[1][row]<lim & data[3][row]>lim & data[5][row]<lim) pu->Fill(3);
        else if (data[1][row]<lim & data[3][row]<lim & data[5][row]>lim) pu->Fill(4);
        else if (data[1][row]>lim & data[3][row]>lim & data[5][row]<lim) pu->Fill(5);
        else if (data[1][row]>lim & data[3][row]<lim & data[5][row]>lim) pu->Fill(6);
        else if (data[1][row]<lim & data[3][row]>lim & data[5][row]>lim) pu->Fill(7);
        else if (data[1][row]>lim & data[3][row]>lim & data[5][row]>lim) pu->Fill(8);
    }

   // TCanvas* c1 = new TCanvas("c1", "c1");
    TAxis* ax = pu->GetXaxis();
    ax->SetBinLabel(1, "24");
    ax->SetBinLabel(2, "124");
    ax->SetBinLabel(3, "234");
    ax->SetBinLabel(4, "245");
    ax->SetBinLabel(5, "1234");
    ax->SetBinLabel(6, "1245");
    ax->SetBinLabel(7, "2354");
    ax->SetBinLabel(8, "12345");

    pu->Draw();



}