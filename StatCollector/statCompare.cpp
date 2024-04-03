//
// Created by linus on 02.04.24.
//
#include <fstream>

using namespace std;

pair<pandaStat*,int> readProblem_csv(string problemFile){
     int amntProblem;
     pandaStat* problemStats;
     ifstream problemStream(problemFile);


     if(!problemStream.is_open()) throw runtime_error("Cant open File: " +problemFile);

     vector<pair<string ,vector<int>>>
    string line, colname;
    int val;
    // Read the column names
    if(problemStream.good())
    {
        // Extract the first line in the file
        getline(problemStream, line);

        // Create a stringstream from line
        std::stringstream ss(line);

        // Extract each column name
        while(std::getline(ss, colname, ',')){

            // Initialize and add <colname, int vector> pairs to result
            result.push_back({colname, std::vector<int> {}});
        }
    }

    // Read data, line by line
    while(std::getline(problemStream, line))
    {
        // Create a stringstream of the current line
        std::stringstream ss(line);

        // Keep track of the current column index
        int colIdx = 0;

        // Extract each integer
        while(ss >> val){

            // Add the current integer to the 'colIdx' column's values vector
            result.at(colIdx).second.push_back(val);

            // If the next token is a comma, ignore it and move on
            if(ss.peek() == ',') ss.ignore();

            // Increment the column index
            colIdx++;
        }
    }

    // Close file
    problemStream.close();
    return {problemStats,amntProblem};
}