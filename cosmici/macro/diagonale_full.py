import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy import stats
import os
import time

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

#ADC.loc["somma_part"] = 





