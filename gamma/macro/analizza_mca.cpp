#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "TSystem.h"

using namespace std;

// Struttura per memorizzare i dati di ogni file
struct DatiSpettro {
    string nome_file;
    double angolo;
    double live_time;
    long long slow_count;
};

// Funzione di supporto per ordinare i dati in base all'angolo
bool confrontaAngoli(const DatiSpettro& a, const DatiSpettro& b) {
    return a.angolo < b.angolo;
}

void analizza_mca() {
    string directory_path = "/Users/sissy/Desktop/LAB_FNS_I/gamma/data/TAC"; // Modifica se i file sono in un'altra cartella
    vector<DatiSpettro> lista_dati;
    
    void* dir = gSystem->OpenDirectory(directory_path.c_str());
    if (!dir) {
        cerr << "Errore: Cartella non trovata!" << endl;
        return;
    }

    const char* entry;
    while ((entry = gSystem->GetDirEntry(dir))) {
        string filename = entry;
        
        if (filename.find(".mca") != string::npos) {
            // --- ESTRAZIONE DELL'ANGOLO DAL NOME ---
            // Cerca la posizione di "deg" nel nome del file
            size_t pos_deg = filename.find("deg");
            double angolo = 0.0;
            
            if (pos_deg != string::npos) {
                // Torna indietro per trovare l'inizio del numero (cerca il '_' precedente)
                size_t pos_underscore = filename.rfind('_', pos_deg);
                if (pos_underscore != string::npos) {
                    string str_angolo = filename.substr(pos_underscore + 1, pos_deg - pos_underscore - 1);
                    try {
                        angolo = stod(str_angolo);
                    } catch (...) {
                        cerr << "Attenzione: Impossibile convertire l'angolo nel file " << filename << endl;
                        angolo = 999.0; // Valore di fallback in caso di errore
                    }
                }
            } else {
                cerr << "Attenzione: Stringa 'deg' non trovata nel file " << filename << endl;
                continue; 
            }
            // ----------------------------------------

            string full_path = directory_path + "/" + filename;
            ifstream file(full_path);
            if (!file.is_open()) continue;

            string line;
            double live_time = -1.0;
            long long slow_count = -1;
            bool trovato_live = false;
            bool trovato_slow = false;

            while (getline(file, line)) {
                if (line.find("LIVE_TIME -") != string::npos) {
                    size_t pos = line.find("-");
                    live_time = stod(line.substr(pos + 2));
                    trovato_live = true;
                }
                else if (line.find("Slow Count:") != string::npos) {
                    size_t pos = line.find(":");
                    slow_count = stoll(line.substr(pos + 1));
                    trovato_slow = true;
                }
                if (trovato_live && trovato_slow) break;
            }
            file.close();

            if (trovato_live && trovato_slow) {
                DatiSpettro dati = {filename, angolo, live_time, slow_count};
                lista_dati.push_back(dati);
            }
        }
    }
    gSystem->FreeDirectory(dir);

    // Ordina il vettore in base all'angolo (dal più negativo al più positivo)
    sort(lista_dati.begin(), lista_dati.end(), confrontaAngoli);

    // --- SCRITTURA E STAMPA DEI RISULTATI ---
    ofstream output_file("riassunto_angoli_conteggi.txt");
    
    // Intestazioni formattate
    auto stampa_riga = [](ostream& os, const string& f, double ang, double lt, long long sc) {
        os << left << setw(35) << f 
           << setw(12) << ang 
           << setw(15) << lt 
           << setw(15) << sc << endl;
    };

    cout << "\n" << string(77, '=') << endl;
    stampa_riga(cout, "Nome File", "Angolo (°)", "Live Time (s)", "Slow Count");
    cout << string(77, '=') << endl;

    if (output_file.is_open()) {
        stampa_riga(output_file, "Nome File", "Angolo (°)", "Live Time (s)", "Slow Count");
        output_file << string(77, '-') << endl;
    }

    for (const auto& d : lista_dati) {
        stampa_riga(cout, d.nome_file, d.angolo, d.live_time, d.slow_count);
        if (output_file.is_open()) {
            stampa_riga(output_file, d.nome_file, d.angolo, d.live_time, d.slow_count);
        }
    }
    cout << string(77, '=') << endl;

    if (output_file.is_open()) {
        output_file.close();
        cout << "\nAnalisi completata con successo! Dati ordinati salvati in 'riassunto_angoli_conteggi.txt'\n" << endl;
    }
}