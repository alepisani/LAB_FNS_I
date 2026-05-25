/**
 * same macro as spettro.cpp but it take all the file.mca in the data directory
 * and save the histograms of the spectra in .pdf files in the plots directory
*/

#include "./ene_calib/poli_fit.cpp"

double p0 = -12.9527;
double p1 = 0.741858;
double p2 = 1.81343e-05;

void spettro2()
{
  //=================================================
  // Macro per leggere spettri gamma - versione multi-file
  //=================================================

  gROOT->Reset();
  gROOT->SetStyle("Plain");

  // Definire sistema
  bool amptek = true;
  int nlines = 12;
  int nchan  = 2048;
  int ngroup = 1;

  if ((nchan % ngroup) != 0) {
    cout << "Numero di canali non divisibile per il numero di raggruppazioni." << endl;
    return;
  }

  // Raccogliere tutti i file .mca nella cartella ../data/
  TSystemDirectory dataDir("dataDir", "../data/");
  TList* fileList = dataDir.GetListOfFiles();

  if (!fileList) {
    cout << "Cartella ../data/ non trovata o vuota." << endl;
    return;
  }

  TSystemFile* sysFile;
  TIter next(fileList);
  int fileCount = 0;

  while ((sysFile = (TSystemFile*)next())) {
    TString sysFileName = sysFile->GetName();

    // Filtrare solo i file .mca
    if (sysFile->IsDirectory() || !sysFileName.EndsWith(".mca"))
      continue;

    // Costruire il path completo
    TString fullPath = TString("../data/") + sysFileName;

    // Estrarre il nome senza estensione (per i plot)
    TString fname = sysFileName;
    fname.ReplaceAll(".mca", "");

    cout << ">>> Elaborazione file: " << fullPath << endl;

    // Aprire il file
    FILE* f = fopen(fullPath.Data(), "r");
    if (!f) {
      cout << "Impossibile aprire: " << fullPath << endl;
      continue;
    }

    // Creare istogramma con nome univoco per ogni file
    TString histName  = TString("Histo_") + fname;
    TString histTitle = TString("Spettro globale - ") + fname;
    TH1F* hist = new TH1F(histName, histTitle, nchan / ngroup, 0, nchan - 1);

    // Variabili di lettura
    char  line[200];
    float x = 0, y = 0;
    int   row = 0, ichan = 0;

    while (fgets(line, 200, f)) {
      if (row >= nlines) {
        if (amptek) {
          sscanf(&line[0], "%f", &y);
          ichan++;
          x = ichan;
        } else {
          sscanf(&line[0], "%f %f", &x, &y);
        }
        double x_after_calib = x*x*p2 + x*p1 + p0;
        hist->AddBinContent(hist->FindBin(x_after_calib), y);
      }
      row++;
    }
    fclose(f);

    // Canvas dedicata per ogni file
    TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
    c1->SetTicks();

    hist->GetXaxis()->SetTitle("Enegia [keV]");
    hist->GetYaxis()->SetTitleOffset(1.5);
    hist->GetYaxis()->SetTitle("# conteggi");
    gStyle->SetOptStat(0);
    hist->Draw();

    TString outPdf = TString("../plots/spettri_energia/spettro_raw_") + fname + ".pdf";
    c1->Print(outPdf.Data());

    cout << "    Salvato: " << outPdf << endl;

    // Pulizia
    delete c1;
    delete hist;

    fileCount++;
  }

  cout << "\n=== Elaborati " << fileCount << " file .mca ===" << endl;
}
