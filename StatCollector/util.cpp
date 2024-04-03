//
// Created by linus on 01.04.24.
//

#include <fstream>
#include <algorithm>
string* sortAlphabetically(string* data,int size){
    std::sort(&data[0],&data[size]);
    return data;
}

void init_problem_csv(ofstream& outputFile){
    outputFile << "problem" << ",";
    outputFile << "config" << ",";
    outputFile << "domain" << ",";
    outputFile << "solved" << ",";
    outputFile << "ramError" << ",";
    outputFile << "timeError" << ",";
    outputFile << "assertionError" << ",";
    outputFile << "length" << ",";
    outputFile << "cost" << ",";
    outputFile << "searchnodes" << ",";
    outputFile << "time" << ",";
    outputFile << endl;

}

void initAverageStats_csv(ofstream& outputFile,bool orderedByDomain){
    if(orderedByDomain){
        outputFile << "domain" << ",";
    }
    outputFile << "config" << ",";
    outputFile << "amntSolved" << ",";
    outputFile << "amntRamError" << ",";
    outputFile << "amntTimeError" << ",";
    outputFile << "amntAssertionError" << ",";
    outputFile << "avgLength" << ",";
    outputFile << "avgCost" << ",";
    outputFile << "avgSearchnodes"<< ",";
    outputFile << "avgTime" << ",";
    outputFile << "sampleSize" <<",";
    outputFile << endl;
}
