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

/**
 * Read a dat and save data to vector of vectors of doubles.
 *
 * @param filename The name of the file to load.
 * @param header The number of header lines to skip.
 * @return a vector of vectors of doubles. Each internal vector represents a
 * column.
 *
 * @throw If in a line the number of columns is different from the first line.
 */

/*
vector<vector<double>> datReaderToDouble(string filename, int header = 0) {
  vector<vector<double>> output;
  // Open the file
  ifstream input_file(filename);
  string line;

  // Skip the header lines
  for (int i = 0; i < header; i++)
    getline(input_file, line);

  bool firstLine = true;
  int column_counter = 0;
  while (getline(input_file, line)) {
    stringstream line_stream(line);
    string cell;

    // Initialize the vector with the columns
    if (firstLine) {
      while (getline(line_stream, cell, ' ')) {
        output.push_back(vector<double>());
        output[column_counter].push_back(stod(cell));
        column_counter++;
      }
      firstLine = false;
      continue;
    }

    int columnIndex = 0;
    while (getline(line_stream, cell, ' ')) {
      // Check column overflow
      if (columnIndex >= column_counter)
        throw invalid_argument("not_equal_columns");
      output[columnIndex].push_back(stod(cell));
      columnIndex++;
    }
    // Check column underflow
    if (columnIndex != column_counter)
      throw invalid_argument("not_equal_columns");
  }

  
  return output;
  }
  */
 
 vector<vector<double>> datReaderToDouble(string filename, int header = 0) {
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
     
     // Leggi direttamente doubles (gestisce spazi multipli/tab automaticamente)
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
    
    output.erase(output.begin());
    return output;
  }
  
  
void letturaFile(){

  vector<vector<double>> dati = datReaderToDouble("../data/TDC3_singola_HV1980_THR38_1000.dat", 0);

  for(int row = 0; row < 20; row++){
    
    for(int cols = 0; cols < 16; cols++){

      cout << dati[cols][row] << ", ";     

    }

    cout << endl;

  }


}

