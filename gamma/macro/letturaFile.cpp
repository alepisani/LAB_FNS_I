#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// macro to read .dat files to 2d vectors of int

 
vector<vector<int>> datReader(string filename, int header = 0, bool erase1col=true) {

   vector<vector<int>> output;
   ifstream input_file(filename);
   
   // Controlla apertura file
   if (!input_file.is_open())
   throw runtime_error("Impossibile aprire il file: " + filename);
   
   string line;
   for (int i = 0; i < header; i++)
   getline(input_file, line);
   
   bool firstLine = true;
   int column_counter = 0;
   
   while (getline(input_file, line)) {
     if (line.empty()) continue; // Salta righe vuote
     
     stringstream line_stream(line);
     double value;
     vector<int> row_values;
     
     // Leggi direttamente int (gestisce spazi multipli/tab automaticamente)
     while (line_stream >> value) {
       row_values.push_back(value);
      }
      
      if (row_values.empty()) continue;
      
      if (firstLine) {
        column_counter = row_values.size();
        for (int i = 0; i < column_counter; i++)
        output.push_back(vector<int>());
        firstLine = false;
      }
      
      if ((int)row_values.size() != column_counter)
      throw invalid_argument("not_equal_columns");
      
      for (int i = 0; i < column_counter; i++)
      output[i].push_back(row_values[i]);
    }
    
    if(erase1col) output.erase(output.begin());
    return output;
  }

  vector<vector<double>> doubleReader(string filename, int header = 0, bool erase1col = true) {

   vector<vector<double>> output;
   ifstream input_file(filename);
   
   // Controlla apertura file
   if (!input_file.is_open())
   throw runtime_error("Impossibile aprire il file: " + filename);
   
   string line;
   for (int i = 0; i < header; i++)
   getline(input_file, line);
   
   bool firstLine = true;
   int column_counter = 0;
   
   while (getline(input_file, line)) {
     if (line.empty()) continue; // Salta righe vuote
     
     stringstream line_stream(line);
     double value;
     vector<double> row_values;
     
     // Leggi direttamente int (gestisce spazi multipli/tab automaticamente)
     while (line_stream >> value) {
       row_values.push_back(value);
      }
      
      if (row_values.empty()) continue;
      
      if (firstLine) {
        column_counter = row_values.size();
        for (int i = 0; i < column_counter; i++)
        output.push_back(vector<double>());
        firstLine = false;
      }
      
      if ((int)row_values.size() != column_counter)
      throw invalid_argument("not_equal_columns");
      
      for (int i = 0; i < column_counter; i++)
      output[i].push_back(row_values[i]);
    }
    
    if(erase1col) output.erase(output.begin());
    return output;
  }
  
  
void letturaFile(){

  vector<vector<int>> dati = datReader("../data/TDC3_singola_HV1980_THR38_1000.dat", 0);

  for(int row = 0; row < 20; row++){
    
    for(int cols = 0; cols < 16; cols++){

      cout << dati[cols][row] << ", ";     

    }

    cout << endl;

  }


}

