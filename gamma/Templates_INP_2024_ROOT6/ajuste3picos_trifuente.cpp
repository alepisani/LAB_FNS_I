void ajuste3picos_trifuente()
{
  //=================================================
  //                 Version root: 6.24/02
  //=================================================
  //   Laboratorio de Instrumentación Nuclear y de Partículas
  //
  //   Macro para ajustar tres picos gaussianos con un fondo lineal o exponencial
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
      ngroup_peak = 1;

  string filename, filename_fondo("");

  // Definir fichero de datos del espectro
     filename = "trifuente.mca";

  // Definir fichero del fondo
  //   filename_fondo = "????.???";  // comentar si no hay fondo

  // Tiempo del espectro y del fondo (en segundos), solo necesario cuando se realiza substraccion de fondo
  float tiempo,tiempo_fondo(0);
  if (filename_fondo!="")  tiempo = 1;        // Tiempo del espectro, solo necesario si hay alguna medida de fondo
  if (filename_fondo!="")  tiempo_fondo = 1;  // solo necesario si hay medida de fondo
          
  // Definicion de los intervalos
  //
 // Definir Intervalo del histograma en la zona del pico
  int hist_ini,hist_fin;
     hist_ini= 7540;
     hist_fin= 7900;

  // Definir intervalo para ajustar el fondo (lineal o exponencial)
  int bkg_ini,bkg_fin;
     bkg_ini = 7550;
     bkg_fin = 7620;

  // Definir tipo de fondo
  bool bkg_lineal = true;  // true para lineal, false para exponencial
 
  // Definir intervalo de la gaussiana 1
  int g1_ini, g1_fin;
     g1_ini= 7640;
     g1_fin= 7700;

  // Definir intervalo de la gaussiana 2
  int g2_ini, g2_fin;
     g2_ini= 7725;
     g2_fin= 7760;

  // Definir intervalo de la gaussiana 3
  int g3_ini, g3_fin;
     g3_ini= 7790;
     g3_fin= 7830;

  // Definir intervalo del ajuste global
  int total_ini,total_fin;
     total_ini= 7560;
     total_fin= 7900;

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
  Double_t par[15];
  TCanvas* c2 = new TCanvas ("C2","analisis picos",640,480);
  c2->cd(1);
  c2->SetTicks();
  
 //Declarar funciones: una recta o una exponencial y tres gaussianas
  TF1 *bkg;
  if (bkg_lineal)
    bkg   = new TF1("bkg","pol1",bkg_ini,bkg_fin);    // fondo lineal
  else
    bkg   = new TF1("bkg","expo",bkg_ini,bkg_fin);    // fondo exponencial
  TF1 *g1    = new TF1("g1","gaus",g1_ini,g1_fin);
  TF1 *g2    = new TF1("g2","gaus",g2_ini,g2_fin);
  TF1 *g3    = new TF1("g3","gaus",g3_ini,g3_fin);
  
// Funcion total como suma de las anteriores
  TF1 *total = bkg_lineal? new TF1("total","pol1+gaus(2)+gaus(5)+gaus(8)",total_ini,total_fin) :  // con fondo lineal
                           new TF1("total","expo+gaus(2)+gaus(5)+gaus(8)",total_ini,total_fin) ;  // con fondo exponencial
  total->SetParNames("Constante","Pendiente",
		     "Amplitud 1","Centroide 1","Sigma 1",
                     "Amplitud 2","Centroide 2","Sigma 2",
                     "Amplitud 3","Centroide 3","Sigma 3");
  total->SetLineColor(2);
  
  // Primera aproximacion: ajustar los 3 picos y el fondo
  // por separado y extraer los valores de los parametros
  peak->Fit(g1,"R0");
  peak->Fit(g2,"R0+");
  peak->Fit(g3,"R0+");
  peak->Fit(bkg,"R0+");
  
  bkg->GetParameters(&par[0]);
  g1->GetParameters(&par[2]);
  g2->GetParameters(&par[5]);
  g3->GetParameters(&par[8]);
  
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

  /*  
  // Dibuja gaussianas de los picos separadamente
  g1->SetParameters(&par[2]);
  g2->SetParameters(&par[5]);
  g3->SetParameters(&par[8]);

  g1->SetLineStyle(2);
  g2->SetLineStyle(2);
  g3->SetLineStyle(2);
  
  g1->Draw("same");
  g2->Draw("same");
  g3->Draw("same");
 */

  // Tabla con resultados del ajuste:
  printf("\nParametros Ajuste:\n");
  printf("\nFondo\n");
  
  // Fondo lineal
  printf("\nFondo 1\n");  
  printf("Constante: %4.3f +/- %4.3f\n",total->GetParameter(0),total->GetParError(0));
  printf("Pendiente:       %4.3f +/- %4.3f\n",total->GetParameter(1),total->GetParError(1));
  
  // Gaussiana 1
  printf("\nGaussiana 1\n");
  printf("Amplitud: %4.2f +/- %4.2f\n",total->GetParameter(1+1),total->GetParError(1+1));
  printf("Centroide: %4.2f +/- %4.2f\n",total->GetParameter(1+2),total->GetParError(1+2));
  printf("Sigma    : %4.2f +/- %4.2f\n",total->GetParameter(1+3),total->GetParError(1+3));
  
  // Gaussiana 2
  printf("\nGaussiana 2\n");
  printf("Cte. Normalizacion: %4.2f +/- %4.2f\n",total->GetParameter(4+1),total->GetParError(4+1));
  printf("Centroide: %4.2f +/- %4.2f\n",total->GetParameter(4+2),total->GetParError(4+2));
  printf("Sigma    : %4.2f +/- %4.2f\n",total->GetParameter(4+3),total->GetParError(4+3));

 // Gaussiana 3
  printf("\nGaussiana 3\n");
  printf("Cte. Normalizacion: %4.2f +/- %4.2f\n",total->GetParameter(7+1),total->GetParError(7+1));
  printf("Centroide: %4.2f +/- %4.2f\n",total->GetParameter(7+2),total->GetParError(7+2));
  printf("Sigma    : %4.2f +/- %4.2f\n",total->GetParameter(7+3),total->GetParError(7+3));

  // Chi-2
  printf("\nChi-2\n");
  printf("chi2 / ndf: %f / %d\n",peak->GetFunction("total")->GetChisquare(),peak->GetFunction("total")->GetNDF());


  // dump canvas to pdf
  // c1->Print("ajuste3picos_espectro.pdf");  
  // c1->Print("ajuste3picos_espectro.png");  
  c2->Print("ajuste3picos_trifuente.pdf");  
  // c2->Print("ajuste3picos_trifuente.png");  

}
