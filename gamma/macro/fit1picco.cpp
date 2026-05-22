void fit1picco()
{
  //=================================================
  // macro per fare un fit gaussiano su fondo lineare/esponenziale

  gROOT->Reset();
  //gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1111);


  bool amptek = true;   // true: amptek, false: PCA, ICSW_Spectech
     int ichan=0; // se amptek=true, genera i valori di x

  // Input

  int nlines,nchan,ngroup,ngroup_peak;
  // Intestazione
      nlines = 12;
  // Numero di canali
      nchan = 2048;
  // Binning. nchan deve essere divisibile per ngroup
  // ...spettro
      ngroup = 1;
  // ...fit del picco
      ngroup_peak = 1;

  string fname, filename;

  // File di input dati
    fname = "Spettro_energia_ravvicinato_delay_ampl_0deg_Cs"; //cambiare questo, senza .mca
     filename = "../data/" + fname + ".mca";
          
  // INTERVALLI 
  //
  // Definire intervallo nella zona del picco
  int hist_ini,hist_fin;
     hist_ini= 600;
     hist_fin= 1200;

  // Def Intervallo per il fit del fondo
  int bkg_ini,bkg_fin;
     bkg_ini = 620;
     bkg_fin = 800;

  // Definire tipo de fondo
  bool bkg_lineal = false;  // true per lineare, false per esponenziale
 
  // Definire intervallo della gaussiana
  int g1_ini, g1_fin;
     g1_ini= 800;
     g1_fin= 1000;

  // Definire intervallo fit totale
  int total_ini,total_fin;
     total_ini= 750;
     total_fin= 1050;

  // Verifica ngroup  
  if ( (nchan % ngroup) != 0 || (nchan % ngroup) != 0 )
    {
      cout << "Numero di canali non divisibile per il numero di raggruppazioni." << endl;
      return;
    }
    
  // dichiarare histo
  TH1F* hist=new TH1F("Histo1","Spettro globale",nchan/ngroup,0,nchan);
  TH1F* peak=new TH1F("Histo2","Fit del picco",nchan/ngroup_peak,0,nchan);
    
  // Aprire i file dati
  FILE *f= fopen(filename.c_str(),"r");
  
  // Dichiarazione delle variabili di lettura
  char line[200], line1[200], line2[200], name[80];
  float x=0, y=0, x1=0, y1=0;
  int row=0;

  // Definizione Canvas
  TCanvas* c1 = new TCanvas ("c1","c1",640,480);
  c1->cd(1);
  c1->SetTicks();
  
  // Apertura file e estrazione valori
  while (fgets(line,200,f)) 
    {
      //printf("%d: %s",row,line); 
      if(row>=nlines) // Saltare intestazione
	{
          if (amptek)
           {
             sscanf(&line[0],"%f",&y); 
             ichan++;  
             x = ichan;
           }
          else 
             sscanf(&line[0],"%f %f ",&x,&y);	 
 
	   y1=0;

	  // Riempimento istogrammi
	  hist->AddBinContent(hist->FindBin(x),y);
	  peak->AddBinContent(peak->FindBin(x),y);
	}
      row++;
    }
  fclose(f);  // Chiusura file
  
  // disgeno hist dello spettro
  hist->GetXaxis()->SetTitle("Canali ADC");
  hist->GetYaxis()->SetTitleOffset(1.5);   
  hist->GetYaxis()->SetTitle("# conteggi");
  hist->Draw();
  
  // Creazione istogramma e canvas della regione di interesse
  Double_t par[5];
  TCanvas* c2 = new TCanvas ("C2","analisi picco",640,480);
  c2->cd(1);
  c2->SetTicks();
  
  //Dichiarazione funzioni: retta/exp e gaussiana
 TF1 *bkg;
  if (bkg_lineal)
    bkg = new TF1("bkg","pol1",bkg_ini,bkg_fin);    // fondo lineare
  else
    bkg = new TF1("bkg","expo",bkg_ini,bkg_fin);    // fondo esponenziale   
    bkg->SetParameters(16., 1.); 
  TF1 *g1    = new TF1("g1","gaus",g1_ini,g1_fin);
  
   // Funzione totale come somma delle anteriori
  TF1 *total = new TF1("total","bkg+gaus(2)",total_ini,total_fin);
  total->SetParNames("Costante","Pendenza",
		     "Ampiezza","Centroide","Sigma");
  total->SetLineColor(2);
  
  // Prima approssimazione: fit del fondo e del picco separatamente
  // ed estrazione dei parametri
  peak->Fit(g1,"R0");
  peak->Fit(bkg,"R0+");
  
  bkg->GetParameters(&par[0]);
  g1->GetParameters(&par[2]);

  // Fit della funzione totale coi parametri trovati prima
  total->SetParameters(par);
  peak->Fit(total,"R");
  total->GetParameters(par);
  
  // Disegno del canvas
  peak->GetXaxis()->SetRange(hist_ini/ngroup_peak,hist_fin/ngroup_peak);
  peak->GetXaxis()->SetTitle("Canali ADC");
  peak->GetYaxis()->SetTitleOffset(1.5);   
  peak->GetYaxis()->SetTitle("# conteggi");
  //hist->SetFillColor(4);
  //hist->SetMaximum(125);
  // Make the plot:
  peak->Draw();
  
  // Draw della gaussiana separatamente
  g1->SetParameters(&par[2]);
  g1->SetLineStyle(2);
  g1->Draw("same");
  
  // Risultati del fit:
  printf("\nParametri fit:\n");
  printf("\nFondo 1\n");  
  printf("Costante: %4.3f +/- %4.3f\n",total->GetParameter(0),total->GetParError(0));
  printf("Coeff. angolare:       %4.3f +/- %4.3f\n", total->GetParameter(1),total->GetParError(1));
  printf("\nGaussiana 1\n");
  printf("Cte. Normalizazione: %4.2f +/- %4.2f\n",total->GetParameter(1+1),total->GetParError(1+1));
  printf("Centroide: %4.2f +/- %4.2f\n",total->GetParameter(1+2),total->GetParError(1+2));
  printf("Sigma    : %4.2f +/- %4.2f\n",total->GetParameter(1+3),total->GetParError(1+3));
  
  //  Calcoli
  printf("\nParametri Gaussiana 1\n");
  
  Double_t Amp_1=0, mu_1=0, sig_1=0, e_Amp_1=0, e_mu_1=0, e_sig_1=0;
  Double_t FWHM_1=0, e_FWHM_1=0, Resol_1=0, e_Resol_1=0, Area_1=0, e_Area_1=0;
  Double_t pi=atan2(0.,-1.);
  
  // Parametri ed errori gaussiana 1
  Amp_1 = total->GetParameter(1+1);
  mu_1 = total->GetParameter(1+2);
  sig_1 = total->GetParameter(1+3);
  e_Amp_1 = total->GetParError(1+1);	
  e_mu_1 = total->GetParError(1+2);
  e_sig_1 = total->GetParError(1+3);
  
  printf("Ampiezza: %4.2f +/- %4.2f\n", Amp_1, e_Amp_1);
  printf("Centroide: %4.2f +/- %4.2f\n", mu_1, e_mu_1);
  printf("Sigma    : %4.2f +/- %4.2f\n", sig_1, e_sig_1);
  
  FWHM_1 = 2.35*sig_1;
  e_FWHM_1 = 2.35*e_sig_1;
  Resol_1=FWHM_1/mu_1;
  e_Resol_1=Resol_1*sqrt(pow((e_FWHM_1/FWHM_1),2)+pow((e_mu_1/mu_1),2));
  
  printf("FWHM: %4.2f +/- %4.2f\n", FWHM_1, e_FWHM_1);
  printf("Risoluzione: %7.5f +/- %7.5f\n", Resol_1, e_Resol_1);
  
  Area_1= sqrt(2*pi)*Amp_1*sig_1/ngroup_peak;
  e_Area_1= Area_1*sqrt(pow((e_Amp_1/Amp_1),2)+pow((e_sig_1/sig_1),2));
  printf("Area_1: %7.0f +/- %7.0f\n", Area_1, e_Area_1);
  
  // Chi-2
  printf("\nChi-2\n");
  printf("chi2 / ndf: %f / %d\n",peak->GetFunction("total")->GetChisquare(),peak->GetFunction("total")->GetNDF());

  // salvare i pdf
  c1->Print(("../plots/spettro_" + fname + ".pdf").c_str());  
  c2->Print(("../plots/fit_" + fname + ".pdf").c_str());  
}
