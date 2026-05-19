void poli_fit()
{
  //=================================================
  //                 Version root: 6.24/02
  //=================================================
  //   Laboratorio de Instrumentacion Nuclear y de Prtículas
  //           
  //   Macro para realizar ajustes polinomicos a una gráfica con errores.
  //   Preparado para recta y parábola.
  //
  //   Cambiar la dimensión de las variables (np) según el número de puntos.
  //   Cambiar los valores xmin y xmax para definir el intervalo del eje X. 
  //
  //=================================================
  
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetOptFit(111);

// Definir número de puntos: np
   const Int_t np = 6;

  // Puntos a ajustar (x,y)
  Double_t x[np]  = {-1.1,0.1,0.6,0.9,1.7,2.0};
  Double_t y[np]  = {-0.9,0.9,2.8,4.6,6.6,10.7};

  // Errores
  Double_t ex[np] = {0.,0.,0.,0.,0.,0.};
  Double_t ey[np] = {0.6,0.3,0.6,0.9,1.0,0.9};
  
// ===============================
// Define intervalo de la escala en x
  Double_t xmin=-1.2;
  Double_t xmax= 2.1;
// =============================== 
 
  // Decalrar grafica
  TGraphErrors *graph1 = new TGraphErrors(np, x, y, ex, ey);
  TGraphErrors *graph2 = new TGraphErrors(np, x, y, ex, ey);
  
  // Pintar grafica en un nuevo canvas
  TCanvas* c1 = new TCanvas ("Canvas","Ajustes",640,640);
  c1->Divide(1,2);
  c1->cd(1);
  c1->SetTicks();

  graph1->GetXaxis()->SetTitle("Valores X");
  graph1->GetYaxis()->SetTitle("Valores Y");
  graph1->GetYaxis()->SetTitleOffset(1.2);
  graph1->SetMarkerStyle(20);
  graph1->SetMarkerSize(0.7);
  graph1->SetMarkerColor(1);
 
  graph2->GetXaxis()->SetTitle("Valores X");
  graph2->GetYaxis()->SetTitle("Valores Y");
  graph2->GetYaxis()->SetTitleOffset(1.2);
  graph2->SetMarkerStyle(20);
  graph2->SetMarkerSize(0.7);
  graph2->SetMarkerColor(1);
  
  
  
  // Declarar funcion recta
  TF1 *l1;
  l1 = new TF1("l1","pol1",xmin,xmax);
  l1->SetLineColor(2);
  graph1->Fit("l1","R");
  graph1->Draw("AP");

  c1->cd(2);
  
  // Declarar funcion parabola
  TF1 *l2;
  l2= new TF1("l2","pol2",xmin,xmax);
  l2->SetLineColor(4);
  graph2->Fit("l2","R");
  graph2->Draw("AP");
  
  // Pintar parametros y chi2
  printf("\nAJUSTE LINEAL: y=a+bx\n");
  printf("a= %4.2f +/- %4.2f\n",
	 l1->GetParameter(0),l1->GetParError(0));
  printf("b= %4.2f +/- %4.2f\n",
	 l1->GetParameter(1),l1->GetParError(1));
  printf("chi2 / ndf: %f / %d\n",graph1->GetFunction("l1")->GetChisquare(),
	 graph1->GetFunction("l1")->GetNDF());

  printf("\nAJUSTE PARABOLICO: y=a+bx+cx^2\n");
  printf("a= %4.2f +/- %4.2f\n",
	 l2->GetParameter(0),l2->GetParError(0));
  printf("b= %4.2f +/- %4.2f\n",
	 l2->GetParameter(1),l2->GetParError(1));
  printf("c= %4.2f +/- %4.2f\n",
	  l2->GetParameter(2),l2->GetParError(2));
  printf("chi2 / ndf: %f / %d\n",graph2->GetFunction("l2")->GetChisquare(),
	 graph2->GetFunction("l2")->GetNDF());

  // dump canvas to pdf

  c1->Print("poli_fit.pdf");  
  // c1->Print("poli_fit.png");  
  
}
