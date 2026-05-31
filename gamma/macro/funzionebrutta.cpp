#include <iostream>
#include "TMath.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"


Double_t funzionebrutta(Double_t *x, Double_t *par) {
    // x[0] è la variabile indipendente (l'angolo)
    // par[0] è la costante di scala (l'altezza)
    // par[1] è R (fissato a 9.5)
    // par[2] è r (fissato a 5.547/2)
    // par[3] è il discostamento da zero
    Double_t x_0 = par [3];
    Double_t x_ass = TMath::Abs(x[0]-x_0);
    Double_t ang_rad = (x_ass / 2.0) * (M_PI / 180.0); // Conversione x/2 in radianti
    Double_t R = par[1];
    Double_t r = par[2];

    
    // 1. Isoliamo l'argomento dell'arcocoseno per fare il controllo
    Double_t arg_acos = (R / r) * sin(ang_rad);
    
    // 2. CONTROLLO DI SICUREZZA:
    // Se l'argomento esce dal dominio matematico di acos [-1, 1],
    // significa che geometricamente i rivelatori non si sovrappongono più.
    if (arg_acos > 1.0 || arg_acos < -1.0) {
        return 0.0; 
    }
    
    // Calcoliamo i due pezzi della formula in modo pulito
    Double_t pezzo1 = (2.0 / M_PI) * acos(arg_acos);
    Double_t pezzo2 = (2.0 * R) / (M_PI * r * r) * sin(ang_rad) * sqrt(r * r - R * R * sin(ang_rad) * sin(ang_rad));
    
    // Moltiplichiamo tutto per il parametro di scala par[0]
    return par[0] * (pezzo1 - pezzo2);
}