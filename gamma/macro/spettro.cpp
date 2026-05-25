void spettro()
{
  //=================================================
 // Macro per leggere spettri gamma

  //=================================================
  
  
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  // Definir sistema
  bool amptek = true;   // true: amptek, false: PCA, ICSW_Spectech
  int ichan=0; // se amptek=true, genera i valori di x

  int nlines,nchan,ngroup,ngroup_peak;
  // Intestazione
      nlines = 12;
  // Numero di canali
      nchan = 2048;
  // Binning. nchan deve essere divisibile per ngroup
  // ...spettro
      ngroup = 1;


 string fname, filename,filename_fondo("");

  // Nome file dati
    fname = "Spettro_energia_ravvicinato_delay_ampl_0deg_Cs";
    filename = "../data/" + fname + ".mca";



  // Verifica ngroup  
  if ( (nchan % ngroup) != 0)
    {
      cout << "Numero di canali non divisibile per il numero di raggruppazioni." << endl;
      return;
    }

  // Dichiarare istogramma
  TH1F* hist=new TH1F("Histo1","Spettro globale",nchan/ngroup,0,nchan-1);

 // Apertura file dati
  FILE *f= fopen(filename.c_str(),"r");

  
  // Dichiarare variabili
  char line[200], line1[200], line2[200], name[80];
  float x=0, y=0, x1=0, y1=0;
  int row=0;

  // Dichiarazione Canvas
  TCanvas* c1 = new TCanvas ("c1","c1",800,600);
  c1->cd(1);
  c1->SetTicks();
  
  // Estrarre valori
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

	  // Llenar histogramas
	  // hist->Fill(x,y);
    hist->AddBinContent(hist->FindBin(x),y);
	}
      row++;
    }
  fclose(f);  // Cerrar fichero
  
  // Dibujar histograma completo
  hist->GetXaxis()->SetTitle("Canali ADC");
  hist->GetYaxis()->SetTitleOffset(1.5); 
  hist->GetYaxis()->SetTitle("# conteggi");
  gStyle->SetOptStat(0);  
  hist->Draw();
  c1->Print(("../plots/spettro_raw_" + fname + ".pdf").c_str()); 
}
