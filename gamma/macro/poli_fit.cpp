void poli_fit()
{
  //=================================================
  // macro per fit polinomiale di primo e secondo grado
  //
  //=================================================
  
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetOptFit(1111);

// Numero di punti: np
   const Int_t np = 6;

  // Puntos a ajustar (x,y)
  Double_t x[np]  = {};
  Double_t y[np]  = {};

  // Errores
  Double_t ex[np] = {0.,0.,0.,0.,0.,0.};
  Double_t ey[np] = {};
  
// ===============================
// Define intervalo de la escala en x
  Double_t xmin=-1.2;
  Double_t xmax= 2.1;
// =============================== 
 
  // Decalrar grafica
  TGraphErrors *graph1 = new TGraphErrors(np, x, y, ex, ey);
  TGraphErrors *graph2 = new TGraphErrors(np, x, y, ex, ey);
  
  // Pintar grafica en un nuevo canvas
  TCanvas* c1 = new TCanvas ("Canvas","Fit",640,640);
  c1->Divide(1,2);
  c1->cd(1);
  c1->SetTicks();

  graph1->GetXaxis()->SetTitle("X");
  graph1->GetYaxis()->SetTitle("Y");
  graph1->GetYaxis()->SetTitleOffset(1.2);
  graph1->SetMarkerStyle(20);
  graph1->SetMarkerSize(0.7);
  graph1->SetMarkerColor(1);
 
  graph2->GetXaxis()->SetTitle("X");
  graph2->GetYaxis()->SetTitle("Y");
  graph2->GetYaxis()->SetTitleOffset(1.2);
  graph2->SetMarkerStyle(20);
  graph2->SetMarkerSize(0.7);
  graph2->SetMarkerColor(1);
  
  
  
  // RETTA
  TF1 *l1;
  l1 = new TF1("l1","pol1",xmin,xmax);
  l1->SetLineColor(2);
  graph1->Fit("l1","R");
  graph1->Draw("AP");

  c1->cd(2);
  
  // PARABOLA
  TF1 *l2;
  l2= new TF1("l2","pol2",xmin,xmax);
  l2->SetLineColor(4);
  graph2->Fit("l2","R");
  graph2->Draw("AP");
  
  // Parametri e chi2
  printf("\nFIT LINEARE: y=q+mx\n");
  printf("q= %4.2f +/- %4.2f\n",
	 l1->GetParameter(0),l1->GetParError(0));
  printf("m= %4.2f +/- %4.2f\n",
	 l1->GetParameter(1),l1->GetParError(1));
  printf("chi2 / ndf: %f / %d\n",graph1->GetFunction("l1")->GetChisquare(),
	 graph1->GetFunction("l1")->GetNDF());

  printf("\nFIT PARABOLICO: y=a+bx+cx^2\n");
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
  
}
