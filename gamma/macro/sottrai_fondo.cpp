#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "TSystem.h"
#include "TMath.h"

using namespace std;

// Funzione robusta per leggere i conteggi e il live time
vector<double> LeggiConteggiMCA(const string& filepath, double& live_time) {
    vector<double> conteggi;
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Impossibile aprire fisicamente il file: " << filepath << endl;
        return conteggi;
    }

    string line;
    bool in_data = false;
    live_time = -1.0;

    while (getline(file, line)) {
        // Rimuove eventuali caratteri di ritorno a capo strani (\r) tipici di Windows se letti su Mac
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.find("LIVE_TIME -") != string::npos) {
            size_t pos = line.find("-");
            live_time = stod(line.substr(pos + 2));
        }
        
        // Entra nella sezione dati
        if (line.find("<<DATA>>") != string::npos) {
            in_data = true;
            continue;
        }
        
        // Esce dalla sezione dati solo quando incontra esattamente <<END>>
        if (in_data && line.find("<<END>>") != string::npos) {
            in_data = false;
            break; // Abbiamo finito di leggere i canali
        }
        
        if (in_data) {
            try {
                conteggi.push_back(stod(line));
            } catch (...) {
                // Salta righe vuote o spurie all'interno di <<DATA>>
                continue;
            }
        }
    }
    file.close();
    return conteggi;
}

void sottrai_fondo() {
    // I tuoi percorsi assoluti su Mac
    string file_fondo = "/Users/sissy/Desktop/LAB_FNS_I/gamma/data/coinc/TAC_Na_fondo_lungo4h_-35deg.mca"; 
    string dir_input  = "/Users/sissy/Desktop/LAB_FNS_I/gamma/data/coinc";
    string dir_output = "/Users/sissy/Desktop/LAB_FNS_I/gamma/data/coinc/spettri_puliti"; 

    // Crea la cartella di output se non esiste
    gSystem->MakeDirectory(dir_output.c_str());

    // Leggi il fondo
    double live_time_fondo = 0;
    vector<double> conteggi_fondo = LeggiConteggiMCA(file_fondo, live_time_fondo);

    if (conteggi_fondo.empty() || live_time_fondo <= 0) {
        cerr << "\n[ERRORE CRITICO]: Impossibile leggere il file di fondo!" << endl;
        cerr << "Verifica se il file esiste o se mancano i tag LIVE_TIME - o <<DATA>> al suo interno." << endl;
        return;
    }

    cout << "--- FILE DI FONDO CARICATO CON SUCCESSO ---" << endl;
    cout << "Path: " << file_fondo << endl;
    cout << "Live Time Fondo: " << live_time_fondo << " s" << endl;
    cout << "Canali letti: " << conteggi_fondo.size() << "\n" << endl;

    // Apri la directory per elaborare i 24 file
    void* dir = gSystem->OpenDirectory(dir_input.c_str());
    if (!dir) {
        cerr << "Errore: la cartella di input non esiste: " << dir_input << endl;
        return;
    }

    const char* entry;
    int file_elaborati = 0;

    while ((entry = gSystem->GetDirEntry(dir))) {
        string filename = entry;

        // Filtra: deve essere un file .mca, NON deve essere il file di fondo stesso, e NON deve essere un file nascosto del Mac
        if (filename.find(".mca") != string::npos && 
            filename != "TAC_Na_fondo_lungo4h_-35deg.mca" && 
            filename.front() != '.') {
            
            string full_input_path = dir_input + "/" + filename;
            double live_time_segnale = 0;
            vector<double> conteggi_segnale = LeggiConteggiMCA(full_input_path, live_time_segnale);

            if (conteggi_segnale.empty() || live_time_segnale <= 0) {
                cerr << "Salto il file (incompleto o illecibile): " << filename << endl;
                continue;
            }
            
            if (conteggi_segnale.size() != conteggi_fondo.size()) {
                cerr << "Errore di mismatch: Il file " << filename << " ha " << conteggi_segnale.size() 
                     << " canali, mentre il fondo ne ha " << conteggi_fondo.size() << "!" << endl;
                continue;
            }

            // Prepariamo la scrittura dello spettro pulito
            ifstream file_in(full_input_path);
            ofstream file_out(dir_output + "/pulito_" + filename);
            
            if (!file_in.is_open() || !file_out.is_open()) {
                cerr << "Errore nell'apertura dei file per la scrittura di: " << filename << endl;
                continue;
            }

            string line;
            // 1. Copia la testata originale identica fino a <<DATA>>
            while (getline(file_in, line)) {
                file_out << line << "\n";
                if (line.find("<<DATA>>") != string::npos) break;
            }

            // 2. Sottrazione del fondo riscalato
            double fattore_scala = live_time_segnale / live_time_fondo;
            for (size_t i = 0; i < conteggi_segnale.size(); ++i) {
                double fondo_atteso = conteggi_fondo[i] * fattore_scala;
                double conteggi_netti = conteggi_segnale[i] - fondo_atteso;
                
                if (conteggi_netti < 0) conteggi_netti = 0.0; 

                // Arrotonda all'intero più vicino
                file_out << TMath::Nint(conteggi_netti) << "\n";
            }

            // 3. Copia il resto del file hardware (configurazioni e footer)
            bool dopo_data = false;
            file_in.seekg(0); 
            while (getline(file_in, line)) {
                if (line.find("<<END>>") != string::npos) dopo_data = true;
                if (dopo_data) {
                    file_out << line << "\n";
                }
            }

            file_in.close();
            file_out.close();
            cout << "-> Generato spettro pulito [" << file_elaborati+1 << "]: pulito_" << filename << endl;
            file_elaborati++;
        }
    }
    gSystem->FreeDirectory(dir);
    cout << "\n[COMPLETATO]: Sottrazione completata! Elaborati " << file_elaborati << " file." << endl;
    cout << "Trovi i risultati in: " << dir_output << endl;
}