void poli_fit()
{
  //=================================================
  // macro per fit polinomiale di primo e secondo grado
  //
  //=================================================
  
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetOptFit(1111);

  bool useall = true;  //se false esclude il primo punto

  vector<double> x, y, ex, ey;
  int np;

  if(useall){

// Numero di punti: np
  np = 6;

  // Punti (x,y)
  y  = {122, 511, 662, 1173, 1275, 1333}; //energie in keV dei gamma
  x = {181, 695, 890, 1541, 1667, 1741}; //canali ADC

  // Errori
  ey = {0.,0.,0.,0.,0.,0.};
  ex = {1, 1, 1, 1, 1, 1};

  } else {
  //Numero di punti: np
  np = 5;

  // Punti (x,y)
  y  = {511, 662, 1173, 1275, 1333}; //energie in keV dei gamma
  x  = {695, 890, 1541, 1667, 1741}; //canali ADC

  // Errori
  ey = {0.,0.,0.,0.,0.};
  ex = {1, 1, 1, 1, 1};
  }
  
// ===============================
// Definire intervallo in x
  Double_t xmin= 100;
  Double_t xmax= 1800;
// =============================== 
 
  //grafici
  TGraphErrors *graph1 = new TGraphErrors(np, x.data(), y.data(), ex.data(), ey.data());
  TGraphErrors *graph2 = new TGraphErrors(np, x.data(), y.data(), ex.data(), ey.data());
  
  // Grafici in un nuovo canvas
  TCanvas* c1 = new TCanvas ("Canvas","Fit",700,1400);
  c1->Divide(1,2);
  c1->cd(1);
  c1->SetTicks();

  graph1->GetYaxis()->SetTitle("Energia [keV]");
  graph1->GetXaxis()->SetTitle("Canali ADC");
  graph1->SetTitle("Fit lineare");
  graph1->GetYaxis()->SetTitleOffset(1.2);
  graph1->SetMarkerStyle(20);
  graph1->SetMarkerSize(0.7);
  graph1->SetMarkerColor(1);
 
  graph2->GetYaxis()->SetTitle("Energia [keV]");
  graph2->GetXaxis()->SetTitle("Canali ADC");
  graph2->SetTitle("Fit parabolico");
  graph2->GetYaxis()->SetTitleOffset(1.2);
  graph2->SetMarkerStyle(20);
  graph2->SetMarkerSize(0.7);
  graph2->SetMarkerColor(1);
  
  
  
  // RETTA
  TF1 *l1;
  l1 = new TF1("l1","pol1",xmin,xmax);
  l1->SetLineColor(2);
  graph1->Fit("l1","RW");
  graph1->Draw("AP");

  c1->cd(2);
  
  // PARABOLA
  TF1 *l2;
  l2= new TF1("l2","pol2",xmin,xmax);
  l2->SetLineColor(4);
  graph2->Fit("l2","RW");
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


  double p0 = l2->GetParameter(0);
  double p1 = l2->GetParameter(1);
  double p2 = l2->GetParameter(2);

  cout << "p0 = " << p0 << endl;
  cout << "p1 = " << p1 << endl;
  cout << "p2 = " << p2 << endl;


  // dump canvas to pdf

  if (useall) c1->Print("../../plots/calib_energia/calib_ene.pdf");
  else c1->Print("../../plots/calib_energia/calib_ene_senza1.pdf");     
  
}
