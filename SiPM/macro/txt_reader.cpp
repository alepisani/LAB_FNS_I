/**
 * this macro contains the function we need to take the data from the .txt file 
*/

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
using namespace std;

vector<vector<double>> txt_reader(const string& filename) {

    vector<vector<double>> output(2); // colonna 0: x, colonna 1: y

    ifstream input_file(filename);
    if (!input_file.is_open())
        throw runtime_error("Impossibile aprire il file: " + filename);

    string line;
    while (getline(input_file, line)) {
        if (line.empty()) continue;

        stringstream line_stream(line);
        double v1, v2;

        // Prova a leggere esattamente due valori double
        // Se ci riesce, è una riga dati; altrimenti è intestazione e si salta
        if (!(line_stream >> v1 >> v2)) continue;

        // Verifica che non ci siano altri token sulla riga (intestazioni numeriche spurie)
        //string extra;
        //if (line_stream >> extra) continue;

        output[0].push_back(v1);
        output[1].push_back(v2);
    }

    if (output[0].empty())
        throw runtime_error("Nessun dato trovato in: " + filename);

    return output;
}
