/**
* cerchiamo di capire il significato di avere tanti conteggi a 0 nella misura di PU
* quindi proviamo la seguente analisi
* per ogni valore di molteplicità a 0 nella PU prendiamo i valori di quell'evento 
* in ADC e cerchiamo di capire quante particelle sono passate.
* Come?
* prendi il valore di adc (per 6-7-8) togli piedistallo e lo dividiamo per il picco della Landau.
* if x < 0.5 nessuna particella; if 0.5 < x < 1.5; if 1.5 < x < 2.5 and so on. 
