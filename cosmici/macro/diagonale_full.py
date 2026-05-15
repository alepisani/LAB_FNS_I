import numpy as np
import pandas as pd
from ROOT import gStyle, TProfile, TCanvas

#intestazione df
Tcols = ["time", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8"]
Tcols_keep = range(1,10)

Acols = ["s1", "s2", "s3", "s4", "s5"]
Acols_keep = range(2,7)

lim = 3000  #limite del TDC

TDC = pd.read_csv("../data/diagonale/TDC_diagonale42_3150.dat",
                  delimiter=" ",
                  header=None,
                  usecols=Tcols_keep,
                  names=Tcols)

ADC = pd.read_csv("../data/diagonale/ADC_diagonale42_3150.dat",
                  delimiter=" ", 
                  header=None,
                  usecols=Acols_keep,
                  names=Acols)

# valori di ADC per singola particella, primi 5 rivelatori
singpart = pd.Series([75.5, 145.3, 87.1, 61.4, 40.2],
                     index=Acols,
                     name="singpart")
piedistallo = pd.Series([33.6, 31.4, 25.9, 30.8, 22.6],
                        index=Acols,
                        name="singpart")

    
ADC_part = (ADC-piedistallo)/singpart
ADC_part = abs(ADC_part.round(decimals=0))

mask_ADC = TDC[Acols]<lim

ADC_part["tot_particelle"] = ADC_part.where(mask_ADC, 0).sum(axis=1)

# maschera per il TDC
#condizioni
c1 = TDC["s1"]<lim
c2 = TDC["s3"]<lim
c3 = TDC["s5"]<lim
c4 = (TDC["s6"]<lim) & (TDC["s7"]<lim) & (TDC["s8"]<lim)

mask = [
        (~c1 & ~c2 & ~c3), #24
        (c1 & ~c2 & ~c3), #124
        (~c1 & c2 & ~c3), #234
        (~c1 & ~c2 & c3), #245
        (c1 & c2 & ~c3), #1234
        (c1 & ~c2 & c3), #1245
        (~c1 & c2 & c3), #2345
        (c1 & c2 & c3 & ~c4), #12345
        (c1 & c2 & c3 & c4) #1245+678
        ]

combinations = np.arange(1,10, 1)

TDC["pattern"] = np.select(mask, combinations)


prof = TProfile("prof","Coincidenze sulla diagonale 2-4", 9, 0.5, 9.5)
for pattern, mult in zip(TDC["pattern"], ADC_part["tot_particelle"]):
    prof.Fill(pattern, mult)

prof.GetXaxis().SetTitle("pattern")
ax = prof.GetXaxis()
ax.SetBinLabel(1, "24")
ax.SetBinLabel(2, "124")
ax.SetBinLabel(3, "234")
ax.SetBinLabel(4, "245")
ax.SetBinLabel(5, "1234")
ax.SetBinLabel(6, "1245")
ax.SetBinLabel(7, "2345")
ax.SetBinLabel(8, "12345")
ax.SetBinLabel(9, "12345+678")

prof.GetYaxis().SetTitle("# medio di particelle rivelate")
    
C1 = TCanvas("C1", "c1", 800, 600)
gStyle.SetOptStat(0)
prof.Draw("E")
C1.Print("../plots/diagonale.pdf")





