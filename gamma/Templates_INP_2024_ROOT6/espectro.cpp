void espectro()
{
  //=================================================
  //                 Version root: 6.24/02
  //=================================================
  //   Laboratorio de Instrumentacion Nuclear
  //           
  //   Macro para leer un archivo de datos y representar un espectro.
  //
  //   * En espectros PCA cambiar previamente "," por " "
  //   * Contar líneas de la cabecera del archivo 
  //   * Comprobar y definir número de canales del MCA
  //   * Cambiar el nombre de los archivos de datos 
  //=================================================
  
  //Algunas opciones de ROOT
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  // Definir sistema
  bool amptek = true;   // true para amptek, false para PCA y ICSW_Spectech
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
      ngroup = 1;

 string filename,filename_fondo("");

  // Definir fichero de datos del espectro
     filename = "bario1.mca";

  // Definir fichero del fondo
  //  filename_fondo = "?????.???";  // comentar si no hay fondo

  // Tiempo del espectro y del fondo (en segundos), solo necesario cuando se realiza substraccion de fondo
  float tiempo,tiempo_fondo(0);
  if (filename_fondo!="")  tiempo = 66325.69;        // Tiempo del espectro, solo necesario si hay alguna medida de fondo
  if (filename_fondo!="")  tiempo_fondo = 88164.39;  // solo necesario si hay medida de fondo

  // Verifica ngroup  
  if ( (nchan % ngroup) != 0)
    {
      cout << "El numero de canales no es divisible por el numero de agrupacion de canales. STOP" << endl;
      return;
    }

  // Declarar histograma
  TH1F* hist=new TH1F("Histo1","Espectro global",nchan/ngroup,0,nchan-1);

 // Abrir archivos de datos
  FILE *f= fopen(filename.c_str(),"r");
  FILE *f1(0);
  if (filename_fondo!="") f1=fopen(filename_fondo.c_str(),"r");
  
  // Declarar variables
  char line[200], line1[200], line2[200], name[80];
  float x=0, y=0, x1=0, y1=0;
  int row=0;

  // Definir canvas
  TCanvas* c1 = new TCanvas ("c1","c1",640,480);
  c1->cd(1);
  c1->SetTicks();
  
  // Extraer valores y rellenar histograma
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
	  // hist->Fill(x,y);
         hist->AddBinContent(hist->FindBin(x),y);
	}
      row++;
    }
  fclose(f);  // Cerrar fichero
  
  // Dibujar histograma completo
  hist->GetXaxis()->SetTitle("Canal");
  hist->GetYaxis()->SetTitleOffset(1.5); 
  hist->GetYaxis()->SetTitle("# cuentas");
  gStyle->SetOptStat(0);  
  hist->Draw();
  //c1->Print("espectro.pdf");  
  //c1->Print("espectro.png");  
}
