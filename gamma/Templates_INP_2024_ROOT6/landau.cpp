void landau()
{
  //=================================================
  //                 Version root: 6.24/02
  //================================================= 
  //   Laboratorio de Instrumentacion Nuclear y de Partículas
  //           
  //   Macro para ajustar una curva de Landau
  //   Se necesita utils.cpp
  //   
  //   ==> Adaptado para un MCA Amptek
  //   * Ejecutar previamente espectro.C para ver el comportamiento de los datos
  //   * Contar líneas de la cabecera del archivo 
  //   * Comprobar y definir número de canales del MCA
  //   * Cambiar el nombre del archivo de datos 
  //=================================================
  //
  //   ==> xmin: canal inicial
  //       xmax: canal repecto del cual se invierte el histograma
  //       ngroup y ngroup_peak: agrupa canales para el ajuste
  //
  //=================================================
  
  //Algunas opciones de ROOT
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1111);
 
  //Load utilities
  gROOT->LoadMacro("utils.cpp");

 // Adaptado para un MCA Amptek 

  int nlines,nchan,ngroup,ngroup_peak;
  // Contar líneas de la cabecera del archivo 
      nlines = 12;
  // Definir número de canales del archivo de datos
      nchan = 2048;
  // Definir agrupación de canales. Comprobar que "nchan" es divisible por el numero de agrupación de canales.
  // ...para el espectro
      ngroup = 1;
  // ...para el ajuste del pico
      ngroup_peak = 5;
 

  string filename;

  // Definir fichero de datos del espectro
     filename = "Americio.mca";


  // Declarar histograma del espectro completo
  TH1F* hist=new TH1F("En Loss","Energy Spectrum",nchan/ngroup,0,nchan);

  
  // Declarar límites del histograma para el ajuste Landau
  // xmax: canal repecto del cual se invierte el histograma para ajustar la curva de Landau
  float xmin=0;
  float xmax=1800;

 // Definir intervalo del pico para un ajuste preliminar gaussiano 
  int g1_ini, g1_fin;
     g1_ini= 1200;
     g1_fin= 1800;

 // Definir intervalo para el ajuste final de Landau
  int land_ini, land_fin;
     land_ini= 0;
     land_fin= 1400;


  // Declarar histograma para el ajuste 
  TH1F* hist_g=new TH1F("En Loss Inv","Energy Loss (Inverted Spectrum)",(xmax-xmin)/ngroup_peak,xmin,xmax);


    
  // Abrir archivos de datos
  FILE *f = fopen(filename.c_str(), "r");

  // Declarar variables
  char line[200], name[80];
  float x=0,y=0,z=0;
  int row=0;

// Define channel para Amptek
  int ichan=0;
  Double_t par[6];


  while (fgets(line,200,f)) {

  if(row>=nlines)  
  {  // Saltar cabecera

    sscanf(&line[0],"%f ", &y);
     x=ichan;

    //if (row%10==0)    printf("%f %f \n",x,y);

    // Llenar histograma
    if(x>50){
      // hist->Fill(x,y);
      // hist_g->Fill(xmax-x,y);

		hist->AddBinContent(hist->FindBin(x), y);
		hist_g->AddBinContent(hist_g->FindBin(xmax-x), y);
    }
//      printf("%f %f %f \n",ichan,x,y);

      ichan++;
  }  
  row++;
  }
  fclose(f); // Cerrar fichero

// Declarar canvas para los plots

  TCanvas* c1 = new TCanvas ("Orig","Espectro original",640,480);
  c1->cd(1);
  c1->SetTicks(); //Mostrar marcas en ambos ejes

  // Opciones del histograma del espectro completo

  gStyle->SetOptStat("ne");
  hist->GetXaxis()->SetTitle("Canal");
  hist->GetYaxis()->SetTitleOffset(1.5);   
  hist->GetYaxis()->SetTitle("# cuentas");
  hist->Draw();

  TCanvas* c2 = new TCanvas ("Invert","Espectro invertido con ajuste Landau",640,480);
  c2->cd(2);
  c2->SetTicks(); //Mostrar marcas en ambos ejes

// Opciones del histograma del espectro invertido
  
  gStyle->SetOptStat("ne");
  
  hist_g->GetXaxis()->SetRange(0,xmax);
  hist_g->GetXaxis()->SetTitle("Canal");
  hist_g->GetYaxis()->SetTitle("Numero de cuentas");
  hist_g->GetYaxis()->SetTitleOffset(1.25);
 


  Double_t gauspar[3];

  //Declarar funciones gaussianas
  TF1 *g1    = new TF1("g1","gaus",g1_ini,g1_fin);
  g1->SetParNames("Amp","Mean","Sigma");
  hist_g->Fit(g1,"R0+");


  g1->GetParameters(&gauspar[0]);

   
  TF1 *fitfunc =new TF1("fu","langaufun",0,1000,4);

  printf("Par: %f  %f  %f  %f \n",hist_g->GetRMS()*0.15,
	 hist_g->GetMean()*0.8, 
	 hist_g->Integral()*hist_g->GetBinWidth(0),
	 hist_g->GetRMS()*0.2); 
  
  fitfunc->SetParameters(hist_g->GetRMS()*0.15,
			 hist_g->GetMean()*0.8, 
			 hist_g->Integral()*hist_g->GetBinWidth(0),
			 hist_g->GetRMS()*0.2); 
  fitfunc->SetRange(land_ini,land_fin);
  fitfunc->SetParNames("Width L","MOP","Area","Width G");
  fitfunc->SetLineColor(2);
  hist_g->Fit(fitfunc,"R");

// Opciones del histograma del espectro invertido con ajuste Landau
  
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111);
  
  hist_g->Draw();

// Chi-2

  printf("\nChi-2\n");
  printf("chi2 / ndf: %f / %d\n",hist_g->GetFunction("fu")->GetChisquare(),
	 hist_g->GetFunction("fu")->GetNDF());

  // dump canvas to pdf
  c1->Print("Espectro alfa original.pdf");  
  // c1->Print("Espectro alfa original.png");  
  c2->Print("Espectro alfa invertido y ajuste Landau.pdf");
  // c2->Print("Espectro alfa invertido y ajuste Landau.png");  

}
