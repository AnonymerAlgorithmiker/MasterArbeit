//
// Created by linus on 03.02.24.
//
#include <fstream>
#include "pandaStat.cpp"
#include "pandaAverageStat.cpp"

bool isFloat( string myString ) {
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail();
}

//analysing engineresults
pandaStat collectSingleStat(string result,string domain,string problem,string config){
    bool solved = false;
    bool ramError = false;
    bool timeError = false;
    bool assertionError = false;
    int length = -1;
    int cost = -1;
    int searchnodes = -1;
    float time = -1;


    istringstream iss(result);
    for (std::string line; std::getline(iss, line); )
    {
        if(line.find("killed by the OOM killer")!=string::npos){
            ramError= true;
        }
        if(line.find("timeout") != string::npos){
            timeError=true;
        }
        if(line.find("-Success")!=string::npos){
            ramError=false;
            timeError=false;
        }
        if (line.find("Status: ") != std::string::npos) {
            if(line.find("Solved") != string::npos){
                solved=true;
            }
        }
        if(solved && line.find("Found solution of length")!=string::npos){
            stringstream linestr(line);
            string word;
            while (linestr >> word){
            }
            length = stoi(word);
        }
        if(solved && line.find("Total costs of actions:")!=string::npos){
            stringstream linestr(line);
            string word;
            while (linestr >> word){
            }
            cost = stoi(word);
        }
        if(line.find("Search time")!=string::npos){
            stringstream linestr(line);
            string word;
            while(linestr >> word){
               if(isFloat(word)){
                   time = stof(word);
               }
            }
        }
        if(line.find("- Generated ")!=string::npos && line.find("search nodes")!=string::npos){
            stringstream linestr(line);
            string word;
            linestr >> word;
            linestr >> word;
            linestr >> word;
            searchnodes = stoi(word);
        }
        if(line.find("Exceeded Max Action condition")!=string::npos){
            assertionError=true;
            cout << "Problem: " << problem << endl;
            cout << "Assertion Error" << endl;
        }

    }
    pandaStat outputStat=pandaStat(solved, ramError, timeError, assertionError, length, cost, searchnodes, time);
    outputStat.addContext(domain,problem,config);
    return outputStat;
}

//Given multiple outputs of Pandaengine this method will return an Array version of the stats in collectSingleStats
pandaStat* collectMultipleStats(string* results, int sampleSize, string* problemList, string domain, string config){

    pandaStat singleStats;
    pandaStat* multipleStats = new pandaStat[sampleSize];
    for(int i=0;i<sampleSize;i++){
        singleStats=collectSingleStat(results[i],domain,problemList[i],config);
        multipleStats[i]=singleStats;
    }
    delete[] results;
    return multipleStats;
}

//merges two sets of stats together and deleting the individual sets
pandaStat* addStats(pandaStat* stats1, pandaStat* stats2, int sampleSize1, int sampleSize2){
    int resultSampleSize=sampleSize1+sampleSize2;
    pandaStat* resultStats = new pandaStat[resultSampleSize];

    for(int i=0;i<resultSampleSize;i++){
        if(i<sampleSize1){
            resultStats[i]=stats1[i];
        }
        if(i>=sampleSize1){
            resultStats[i]=stats2[i-sampleSize1];
        }
    }
    delete[] stats1;
    delete[] stats2;
    return resultStats;
}


pandaAverageStat calculateAverages(pandaStat* stats, int amntProblems, string domain, string config){
    int amntSolved=0;
    int amntRamError=0;
    int amntTimeError=0;
    int amntAssertionError=0;
    int sumLength=0;
    int sumCost=0;
    int sumSearchnodes=0;
    float sumTime=0;
    int i = 0;
    for(i=0;i<amntProblems;i++){
        pandaStat currStat = stats[i];
        if(currStat.solved){
            amntSolved++;
            sumLength=sumLength+currStat.length;
            sumCost=sumCost+currStat.cost;
            sumTime=sumTime+currStat.time;
            sumSearchnodes=sumSearchnodes+currStat.searchnodes;
        }
        if(currStat.ramError){
            amntRamError++;
        }
        if(currStat.assertionError){
            amntAssertionError++;
        }
        if(currStat.timeError){
            amntTimeError++;
        }
    }
    float avgLength=sumLength/i;
    float avgCost=sumCost/i;
    float avgTime=sumTime/i;
    float avgSearchNodes=sumSearchnodes/i;

    pandaAverageStat averageStat = pandaAverageStat(amntSolved,amntRamError,amntTimeError,amntAssertionError,avgLength,avgCost,avgSearchNodes,avgTime,amntProblems);
    averageStat.addContext(domain,config);

    return averageStat;
}