void ajustexp_cte()
{
  //=================================================
  //                 Version root: 6.24/02
  //================================================= 
  //   Laboratorio de Instrumentacion Nuclear y de Partículas
  //
  //   Macro para ajustar la salida del TAC a una exponencial + un fondo constante
  //   El fondo representa la distribución uniforme de 
  //   "Start - Stop" no correlacionados 
  //   tau_muon = -(1/Par_exp) en canales^(-1) 
  //   Es necesaria la calibración del TAC "canales - tiempo"
  //
  //   Preparado para sumar al menos 4 espectros
  //=================================================
  
  //Algunas opciones de ROOT
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetOptFit(111);
  gStyle->SetOptStat(0);

 // Definir sistema
  bool amptek = true;   // true para amptek, false para PCA, ICSW_Spectech y Phywe
     int ichan=0; // Sólo se usa si "amptek = true" para generar los valores de x 

  // Definir entrada
  //   * En espectros PCA cambiar previamente "," por " "

  int nlines,nchan,ngroup;
  // Contar líneas de la cabecera del archivo 
      nlines = 12;
  // Definir número de canales del archivo de datos
      nchan = 4096;
  // Definir agrupación de canales. Comprobar que "nchan" es divisible por el numero de agrupación de canales.
  // ...para el espectro
      ngroup = 4;

string filename_1, filename_2(""), filename_3(""), filename_4("");
 
  // Definir ficheros de datos. Preparado para sumar 4 archivos de identica estructura
     filename_1 = "live_data_amptek.mca";
     filename_2 = "live_data_amptek_copia.mca";  // comentar si no se utiliza
   //filename_3 = "----.---";  // comentar si no se utiliza
   //filename_4 = "----.---";  // comentar si no se utiliza


// Definicion de los intervalos
//
// Intervalo del histograma
     int hist_ini= 200;
     int hist_fin= 4000;

// Intervalo para ajuste aproximado del fondo lineal
     int l0_ini=3500;
     int l0_fin=4000;
  
// Intervalo del ajuste aproximado de la exponencial

     int expo_ini=200;              
     int expo_fin=3000;

// Intervalo del ajuste global: exponencial + fondo

     int total_ini=200;
     int total_fin=4000;

  // Declarar histograma
  
  TH1F* hist=new TH1F("Histo","Espectro total: f(x)= p0 + exp(p1+p2*x)",nchan/ngroup,0,nchan);

  //Abrir archivos de datos
  FILE *f1= fopen(filename_1.c_str(),"r");

  FILE *f2(0),*f3(0),*f4(0);
  if (filename_2!="") f2=fopen(filename_2.c_str(),"r");
  if (filename_3!="") f3=fopen(filename_3.c_str(),"r");
  if (filename_4!="") f2=fopen(filename_4.c_str(),"r");

  // Declarar variables
   char line1[200],line2[200],line3[200],line4[200], name[80];
   float x=0, x1=0, x2=0, x3=0, x4=0;
   float y=0, y1=0, y2=0, y3=0, y4=0;
   int row=0;

 // Extraer valores
  while (fgets(line1,200,f1)) 
 {
    //printf("%d: %s",row,line); 
      if (f2) fgets(line2,200,f2);
      if (f3) fgets(line3,200,f3);
      if (f4) fgets(line4,200,f4);

      if(row>=nlines) // Saltar cabecera
	{
          if (amptek)
           {
             sscanf(&line1[0],"%f",&y1); 

              if (f2) sscanf(&line2[0],"%f",&y2);
	      if (f3) sscanf(&line3[0],"%f",&y3);
	      if (f4) sscanf(&line4[0],"%f",&y4);

             ichan++;  
             x = ichan;
           }
          else 
           {
             sscanf(&line1[0],"%f %f ",&x,&y1);
	
             if (f2) sscanf(&line2[0],"%f %f ",&x,&y2);
	     if (f3) sscanf(&line3[0],"%f %f ",&x,&y3);
	     if (f4) sscanf(&line4[0],"%f %f ",&x,&y4);
           }
 
             y = y1+y2+y3+y4;

  // Llenar histogramas
	  // hist->Fill(x,y);
     hist->AddBinContent(hist->FindBin(x),y);
	}
    row++;
  }
  fclose(f1);  // Cerrar fichero

  // AJUSTE

 Double_t par[3];
 
  TCanvas* c1 = new TCanvas ("C1","ajuste exponencial",640,480);
  c1->cd(1);
  c1->SetTicks();


  //Declarar funciones: fondo constante (p0) + exponencial (p1,p2) 
  //              f(x)= p0 + exp(p1+p2*x)

    TF1 *l0    = new TF1("l0","pol0",l0_ini,l0_fin);
    TF1 *e1    = new TF1("e1","expo",expo_ini,expo_fin);

  // Funcion total como suma de las anteriores
  TF1 *total = new TF1("total","l0+expo(1)",total_ini,total_fin);
  total->SetParNames("Fondo","Cte_exp","Par_exp");
  total->SetLineColor(2);

  // Primera aproximacion: ajustar el fondo y la exponencial
  // por separado y extraer los valores de los parametros
      hist->Fit(l0,"R0");
      hist->Fit(e1,"R0+");

      l0->GetParameters(&par[0]);
      e1->GetParameters(&par[1]);

  // Asignar parametros previos a la funcion total y ajustar
      total->SetParameters(par);
      hist->Fit(total,"R");
      total->GetParameters(par);


  // Opciones finales y dibujar histograma
  hist->GetXaxis()->SetRange(hist_ini/ngroup,hist_fin/ngroup);
  hist->GetXaxis()->SetTitle("Canal");
  hist->GetYaxis()->SetTitle("Numero de cuentas");
  hist->GetYaxis()->SetTitleOffset(1);

  hist->Draw();
 
TCanvas* c2 = new TCanvas ("C2","ajuste exponencial",640,480);
  c2->cd(1);
  c2->SetTicks();


  // Escala logaritmica =====================
  gPad->SetLogy(1);

  hist->Draw();

  // Tabla con resultado del ajuste:
  printf("\nParametros Ajuste:\n");
  printf("Fondo: %4.5f +/- %4.5f\n",
	 total->GetParameter(0),total->GetParError(0));
  printf("Cte_exp: %4.7f +/- %4.7f\n",
	 total->GetParameter(1),total->GetParError(1));
  printf("Par_exp: %4.7f +/- %4.7f\n",
	 total->GetParameter(2),total->GetParError(2));

// Chi-2

  printf("\nChi-2\n");
  printf("chi2 / ndf: %f / %d\n",hist->GetFunction("total")->GetChisquare(),
	 hist->GetFunction("total")->GetNDF());

 // dump canvas to pdf
     c1->Print("ajuste exponencial.pdf");  
  // c1->Print("ajuste exponencial.png");  
     c2->Print("ajuste exponencial (escala logaritmica).pdf");  
  // c2->Print("ajuste exponencial (escala logaritmica).png");  
}
