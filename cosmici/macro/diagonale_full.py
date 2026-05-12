import numpy as np
import pandas as pd
from scipy import stats
from ROOT import gStyle, TProfile, TCanvas

#intestazione df
Tcols = ["time", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8"]
Tcols_keep = range(1,10)

Acols = ["s1", "s2", "s3", "s4", "s5"]
Acols_keep = range(2,7)

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
ADC_part["tot_particelle"] = ADC_part.sum(axis=1)

# maschera per il TDC
#condizioni
lim = 3000
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
        (c1 & c2 & c3), #12345
        (c1 & c2 & c3 & c4) #1245+678
        ]

combinations = range(1,10)

TDC["pattern"] = np.select(mask, combinations)


bins_npart = np.arange(1.5,41.6, 4)
bins_npart = np.append(bins_npart, int(ADC_part["tot_particelle"].max()))



prof = TProfile("prof","prof", 9, 0.5, 9.5)
for pattern, mult in zip(TDC["pattern"], ADC_part["tot_particelle"]):
    prof.Fill(pattern, mult)
c1 = TCanvas("c1", "c1", 800, 600)
gStyle.SetOptStat(0)
prof.Draw("E")
c1.Print("../plots/diagonale.pdf")





