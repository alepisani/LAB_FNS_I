void Co57_fit2picchi()
{
 //=================================================
  // macro per fare due fit gaussiani su fondo lineare/esponenziale
  
  gROOT->Reset();
  //gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111);


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
    fname = "Spettro_energia_ravvicinato_delay_ampl_0deg_Co57"; //cambiare questo, senza .mca
    filename = "../../data/" + fname + ".mca";

  // INTERVALLI 
  //
  // Definire intervallo nella zona del picco
  int hist_ini,hist_fin;
     hist_ini= 100;
     hist_fin= 700;

  // Def Intervallo per il fit del fondo
  int bkg_ini,bkg_fin;
     bkg_ini = 120;
     bkg_fin = 500;

  // Definire tipo de fondo
  bool bkg_lineal = false;  // true per lineare, false per esponenziale
 
  // Definire intervallo della gaussiana 1
  int g1_ini, g1_fin;
     g1_ini= 150;
     g1_fin= 210;

  // Definire intervallo della gaussiana 2
  int g2_ini, g2_fin;
     g2_ini= 270;
     g2_fin= 300;

  // Definire intervallo del fit totale
  int total_ini,total_fin;
     total_ini= 150;
     total_fin= 400;

  // Verifica ngroup  
  if ( (nchan % ngroup) != 0 || (nchan % ngroup) != 0 )
    {
      cout << "Numero di canali non divisibile per il numero di raggruppazioni." << endl;
      return;
    }
    
  // dichiarare histo
  TH1F* hist=new TH1F("Histo1","Spettro del ^{60}Co",nchan/ngroup,0,nchan);
  TH1F* peak=new TH1F("Histo2","Fit dei picchi da 1173 e 1333 keV del ^{60}Co",nchan/ngroup_peak,0,nchan);
    
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
  
 //DDichiarazione funzioni: retta/esponenziale e due gaussiane
  TF1 *bkg;
  if (bkg_lineal)
    bkg   = new TF1("bkg","pol1",bkg_ini,bkg_fin);    // fondo lineare
  else
    bkg   = new TF1("bkg","expo",bkg_ini,bkg_fin);    // fondo esponenziale
  TF1 *g1    = new TF1("g1","gaus",g1_ini,g1_fin);
  TF1 *g2    = new TF1("g2","gaus",g2_ini,g2_fin);
  
// Funzione totale come somma delle altre tre
  TF1 *total = bkg_lineal? new TF1("total","pol1+gaus(2)+gaus(5)",total_ini,total_fin) :  // con fondo lineal
                           new TF1("total","expo+gaus(2)+gaus(5)",total_ini,total_fin) ;  // con fondo exponencial
  total->SetParNames("Intercetta","Pendenza",
		     "Ampiezza 1","Centroide 1","Sigma 1","Ampiezza 2","Centroide 2","Sigma 2");
  total->SetLineColor(2);

  g1->SetParameter(1, 280);
  
 // Prima approssimazione: fit del fondo e dei picchi separatamente
  // ed estrazione dei parametri
  peak->Fit(g1,"R0");
  peak->Fit(g2,"R0+");
  peak->Fit(bkg,"R0+");
  
  bkg->GetParameters(&par[0]);
  g1->GetParameters(&par[2]);
  g2->GetParameters(&par[5]);
  

  // Fit della funzione totale coi parametri trovati prima
  total->SetParameters(par);
  peak->Fit(total,"R");
  total->GetParameters(par);

  // Disegno canvas
  peak->GetXaxis()->SetRange(hist_ini/ngroup_peak,hist_fin/ngroup_peak);
  peak->GetXaxis()->SetTitle("Canal");
  peak->GetYaxis()->SetTitleOffset(1.5);   
  peak->GetYaxis()->SetTitle("# cuentas");
  
  //hist->SetFillColor(4);
  //hist->SetMaximum(125);
  // Make the plot:
  peak->Draw();

  /*  
  // Picchi gaussiani separati
  g1->SetParameters(&par[2]);
  g2->SetParameters(&par[5]);
  
  g1->SetLineStyle(2);
  g2->SetLineStyle(2);
  
  g1->Draw("same");
  g2->Draw("same");
 */

  // Risultati del fit
  printf("\nParametri del fit:\n");
  printf("\nFondo\n");
  
  // Fondo lineal
  printf("\nFondo 1\n");  
  printf("Intercetta: %4.3f +/- %4.3f\n",total->GetParameter(0),total->GetParError(0));
  printf("Pendenza:       %4.3f +/- %4.3f\n",total->GetParameter(1),total->GetParError(1));
  
  // Gaussiana 1
  printf("\nGaussiana 1\n");
  printf("Ampiezza: %4.2f +/- %4.2f\n",total->GetParameter(1+1),total->GetParError(1+1));
  printf("Centroide: %4.2f +/- %4.2f\n",total->GetParameter(1+2),total->GetParError(1+2));
  printf("Sigma    : %4.2f +/- %4.2f\n",total->GetParameter(1+3),total->GetParError(1+3));
  
  // Gaussiana 2
  printf("\nGaussiana 2\n");
  printf("Ampiezza: %4.2f +/- %4.2f\n",total->GetParameter(4+1),total->GetParError(4+1));
  printf("Centroide: %4.2f +/- %4.2f\n",total->GetParameter(4+2),total->GetParError(4+2));
  printf("Sigma    : %4.2f +/- %4.2f\n",total->GetParameter(4+3),total->GetParError(4+3));

  // Chi-2
  printf("\nChi-2\n");
  printf("chi2 / ndf: %f / %d\n",peak->GetFunction("total")->GetChisquare(),peak->GetFunction("total")->GetNDF());


  // dump canvas to pdf
  c1->Print(("../../plots/calib_energia/spettro_" + fname + ".pdf").c_str());   
  c2->Print(("../../plots/calib_energia/fit_" + fname + ".pdf").c_str());   

}
