void landau_gauss(){

    //open input file
    TFile hfile("../data/singole/ADC3_singola_HV1980_THR40_100000.root");

    //read histo from imput file: !!!! CHANGE !!!!!!
    TH1D* hist = (TH1D*)hfile->Get("adc3");

    //define function landau + gauss
    TF1D* langauss = new TF1D("landgauss", "landau(0) + gaus(3)", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());

    




}