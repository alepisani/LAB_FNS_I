void ajuste1pico()
{
  //=================================================
  //                 Version root: 6.24/02
  //=================================================
  //   Laboratorio de Instrumentación Nuclear y de Partículas
  //
  //   Macro para ajustar un pico gaussiano con un fondo lineal o exponencial
  //   * En espectros PCA cambiar previamente "," por " "
  //   * Ejecutar previamente espectro.cpp para ver el comportamiento de los datos
  //   * Contar líneas de la cabecera del archivo 
  //   * Comprobar y definir número de canales del MCA
  //   * Cambiar el nombre de los archivos de datos 
  //=================================================
  
  //Algunas opciones de ROOT
  gROOT->Reset();
  //gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111);

  // Definir sistema
  bool amptek = true;   // true para amptek, false para PCA y ICSW_Spectech
     int ichan=0; // Sólo se usa si "amptek = true" para generar los valores de x 

  // Definir entrada
  //   * En espectros PCA cambiar previamente "," por " "

  int nlines,nchan,ngroup,ngroup_peak;
  // Contar líneas de la cabecera del archivo 
      nlines = 12;
  // Definir número de canales del archivo de datos
      nchan = 8192;
  // Definir agrupación de canales. Comprobar que "nchan" es divisible por el numero de agrupación de canales.
  // ...para el espectro
      ngroup = 1;
  // ...para el ajuste del pico
      ngroup_peak = 4;

  string filename, filename_fondo("");

  // Definir fichero de datos del espectro
     filename = "Espectro_1pico_amptek.mca";

  // Definir fichero del fondo
  // filename_fondo = "Espectro_1pico_fondo_amptek.mca";  // comentar si no hay fondo

  // Tiempo del espectro y del fondo (en segundos), solo necesario cuando se realiza substraccion de fondo
  float tiempo,tiempo_fondo(0);
  if (filename_fondo!="")  tiempo = 66325.69;        // Tiempo del espectro, solo necesario si hay alguna medida de fondo
  if (filename_fondo!="")  tiempo_fondo = 88164.39;  // solo necesario si hay medida de fondo
          
  // Definicion de los intervalos
  //
  // Definir Intervalo del histograma en la zona del pico
  int hist_ini,hist_fin;
     hist_ini= 1500;
     hist_fin= 3500;

  // Definir intervalo para ajustar el fondo (lineal o exponencial)
  int bkg_ini,bkg_fin;
     bkg_ini = 3000;
     bkg_fin = 4000;

  // Definir tipo de fondo
  bool bkg_lineal = true;  // true para lineal, false para exponencial
 
  // Definir intervalo de la gaussiana
  int g1_ini, g1_fin;
     g1_ini= 2200;
     g1_fin= 2700;

  // Definir intervalo del ajuste global
  int total_ini,total_fin;
     total_ini= 1900;
     total_fin= 3000;

  // Verifica ngroup  
  if ( (nchan % ngroup) != 0 || (nchan % ngroup) != 0 )
    {
      cout << "El numero de canales no es divisible por el numero de agrupacion de canales. STOP" << endl;
      return;
    }
    
  // Declarar histograma
  TH1F* hist=new TH1F("Histo1","Espectro global",nchan/ngroup,0,nchan);
  TH1F* peak=new TH1F("Histo2","Ajuste del pico",nchan/ngroup_peak,0,nchan);
    
  // Abrir archivos de datos
  FILE *f= fopen(filename.c_str(),"r");
  FILE *f1(0);
  if (filename_fondo!="") f1=fopen(filename_fondo.c_str(),"r");
  
  // Declarar variables
  char line[200], line1[200], line2[200], name[80];
  float x=0, y=0, x1=0, y1=0;
  int row=0;

  // Define Canvas
  TCanvas* c1 = new TCanvas ("c1","c1",640,480);
  c1->cd(1);
  c1->SetTicks();
  
  // Abrir fichero ascii y extraer valores
  while (fgets(line,200,f)) 
    {
      //printf("%d: %s",row,line); 
      if (f1) fgets(line1,200,f1);
      if(row>=nlines) // Saltar cabecera
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

	  if (f1) 
           {
                if (amptek)
                    sscanf(&line1[0],"%f",&y1); 
                else 
                    sscanf(&line1[0],"%f %f ",&x1,&y1);
	  // Restar fondo 
                    y = y - (tiempo/tiempo_fondo)*y1;
           }

	  // Llenar histogramas
	  hist->AddBinContent(hist->FindBin(x),y);
	  peak->AddBinContent(peak->FindBin(x),y);
	}
      row++;
    }
  fclose(f);  // Cerrar fichero
  
  // Dibujar histograma completo (ampliar para ver picos)
  hist->GetXaxis()->SetTitle("Canal");
  hist->GetYaxis()->SetTitleOffset(1.5);   
  hist->GetYaxis()->SetTitle("# cuentas");
  hist->Draw();
  
  // Crear histograma y Canvas para la región de interés
  Double_t par[5];
  TCanvas* c2 = new TCanvas ("C2","analisis picos",640,480);
  c2->cd(1);
  c2->SetTicks();
  
  //Declarar funciones: una recta o una exponencial y una gaussiana
 TF1 *bkg;
  if (bkg_lineal)
    bkg = new TF1("bkg","pol1",bkg_ini,bkg_fin);    // fondo lineal
  else
    bkg = new TF1("bkg","expo",bkg_ini,bkg_fin);    // fondo exponencial    
  TF1 *g1    = new TF1("g1","gaus",g1_ini,g1_fin);
  
   // Funcion total como suma de las anteriores
  TF1 *total = new TF1("total","bkg+gaus(2)",total_ini,total_fin);
  total->SetParNames("Constante","Pendiente",
		     "Amplitud","Centroide","Sigma");
  total->SetLineColor(2);
  
  // Primera aproximacion: ajustar el pico y el fondo
  // por separado y extraer los valores de los parametros
  peak->Fit(g1,"R0");
  peak->Fit(bkg,"R0+");
  
  bkg->GetParameters(&par[0]);
  g1->GetParameters(&par[2]);

  // Asignar parametros previos a la funcion total y ajustar
  total->SetParameters(par);
  peak->Fit(total,"R");
  total->GetParameters(par);
  
  // Opciones finales y dibujar histograma
  peak->GetXaxis()->SetRange(hist_ini/ngroup_peak,hist_fin/ngroup_peak);
  peak->GetXaxis()->SetTitle("Canal");
  peak->GetYaxis()->SetTitleOffset(1.5);   
  peak->GetYaxis()->SetTitle("# cuentas");
  //hist->SetFillColor(4);
  //hist->SetMaximum(125);
  // Make the plot:
  peak->Draw();
  
  // Dibuja gaussianas de los pico separadamente
  //g1->SetParameters(&par[2]);
  //g1->SetLineStyle(2);
  //g1->Draw("same");
  
  // Tabla con resultados del ajuste:
  printf("\nParametros Ajuste:\n");
  printf("\nFondo 1\n");  
  printf("Constante: %4.3f +/- %4.3f\n",total->GetParameter(0),total->GetParError(0));
  printf("Pendiente:       %4.3f +/- %4.3f\n", total->GetParameter(1),total->GetParError(1));
  printf("\nGaussiana 1\n");
  printf("Cte. Normalizacion: %4.2f +/- %4.2f\n",total->GetParameter(1+1),total->GetParError(1+1));
  printf("Centroide: %4.2f +/- %4.2f\n",total->GetParameter(1+2),total->GetParError(1+2));
  printf("Sigma    : %4.2f +/- %4.2f\n",total->GetParameter(1+3),total->GetParError(1+3));
  
  //  Cálculos
  printf("\nParametros Gaussiana 1\n");
  
  Double_t Amp_1=0, mu_1=0, sig_1=0, e_Amp_1=0, e_mu_1=0, e_sig_1=0;
  Double_t FWHM_1=0, e_FWHM_1=0, Resol_1=0, e_Resol_1=0, Area_1=0, e_Area_1=0;
  Double_t pi=atan2(0.,-1.);
  
  // Parámetros y errores de la Gaussiana 1
  Amp_1 = total->GetParameter(1+1);
  mu_1 = total->GetParameter(1+2);
  sig_1 = total->GetParameter(1+3);
  e_Amp_1 = total->GetParError(1+1);	
  e_mu_1 = total->GetParError(1+2);
  e_sig_1 = total->GetParError(1+3);
  
  printf("Amplitud: %4.2f +/- %4.2f\n", Amp_1, e_Amp_1);
  printf("Centroide: %4.2f +/- %4.2f\n", mu_1, e_mu_1);
  printf("Sigma    : %4.2f +/- %4.2f\n", sig_1, e_sig_1);
  
  FWHM_1 = 2.35*sig_1;
  e_FWHM_1 = 2.35*e_sig_1;
  Resol_1=FWHM_1/mu_1;
  e_Resol_1=Resol_1*sqrt(pow((e_FWHM_1/FWHM_1),2)+pow((e_mu_1/mu_1),2));
  
  printf("FWHM: %4.2f +/- %4.2f\n", FWHM_1, e_FWHM_1);
  printf("Resolucion: %7.5f +/- %7.5f\n", Resol_1, e_Resol_1);
  
  Area_1= sqrt(2*pi)*Amp_1*sig_1/ngroup_peak;
  e_Area_1= Area_1*sqrt(pow((e_Amp_1/Amp_1),2)+pow((e_sig_1/sig_1),2));
  printf("Area_1: %7.0f +/- %7.0f\n", Area_1, e_Area_1);
  
  // Chi-2
  printf("\nChi-2\n");
  printf("chi2 / ndf: %f / %d\n",peak->GetFunction("total")->GetChisquare(),peak->GetFunction("total")->GetNDF());

  // dump canvas to pdf
  // c1->Print("ajuste1pico_espectro.pdf");  
  // c1->Print("ajuste1pico_espectro.png");  
     c2->Print("ajuste1pico.pdf");  
  // c2->Print("ajuste1pico.png");  
}
