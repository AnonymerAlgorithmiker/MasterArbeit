//
// Created by linus on 02.04.24.
//
#include <fstream>
#include <vector>
#include "statCollector.cpp"

using namespace std;

//read pandastats that are produced by pandStat.outputSingleStat_csv
vector<pandaStat> readProblem_csv(string problemFile){
    int countProblem=0;
    vector<pandaStat> problemStats;
    vector<bool> solved;
    vector<bool>ramErrors;
    vector<bool> timeErrors;
    vector<bool> assertionErrors;
    vector<int> lengths;
    vector<int> costs;
    vector<int> nodes;
    vector<float> times;
    vector<string> names;
    vector<string> configs;
    vector<string> domains;


    ifstream problemStream(problemFile);
    if(!problemStream.is_open()) throw runtime_error("Cant open File: " +problemFile);

    string line;
    string colname[11];
    // Read the column names
    if(problemStream.good())
    {
        // Extract the first line in the file
        getline(problemStream, line);

        // Create a stringstream from line
        stringstream ss(line);

        string currColname;
        int colNo=0;
        // Extract each column name
        while(getline(ss, currColname, ',')){
            // Initialize and add <colname, int vector> pairs to result
            colname[colNo]=currColname;
            colNo++;
        }
    }

    // Read data, line by line
    while(std::getline(problemStream, line))
    {
        // Create a stringstream of the current line
        stringstream ss(line);

        // Keep track of the current column index

        // Extract each integer
        for(int colIdx=0;colIdx<11;colIdx++){
            bool nextBool;
            int nextInt;
            float nextFloat;
            char nextString[256];

            switch(colIdx){
                case 0:
                    ss.getline(nextString,256,',');
                    names.push_back(string(nextString));
                    break;
                case 1:
                    ss.getline(nextString,256,',');
                    configs.push_back(string(nextString));
                    break;
                case 2:
                    ss.getline(nextString,256,',');
                    domains.push_back(string(nextString));
                    break;
                case 3:
                    ss >> nextBool;
                    solved.push_back(nextBool);
                    break;
                case 4:
                    ss >> nextBool;
                    ramErrors.push_back(nextBool);
                    break;
                case 5:
                    ss >> nextBool;
                    timeErrors.push_back(nextBool);
                    break;
                case 6:
                    ss >> nextBool;
                    assertionErrors.push_back(nextBool);
                    break;
                case 7:
                    ss >> nextInt;
                    lengths.push_back(nextInt);
                    break;
                case 8:
                    ss >> nextInt;
                    costs.push_back(nextInt);
                    break;
                case 9:
                    ss >> nextInt;
                    nodes.push_back(nextInt);
                    break;
                case 10:
                    ss >> nextFloat;
                    times.push_back(nextFloat);
                    break;
            }
            // If the next token is a comma, ignore it and move on
            if(ss.peek() == ',') ss.ignore();
        }

        countProblem++;
    }
    for(int i=0;i<countProblem;i++){
        problemStats.push_back(pandaStat(solved[i],ramErrors[i],timeErrors[i],assertionErrors[i],lengths[i],costs[i],nodes[i],
                                  times[i],domains[i],names[i],configs[i]));
    }
    // Close file
    problemStream.close();
    return problemStats;
}

// todo: finish dis
vector<vector<pandaStat>> orderSetByDomain(vector<pandaStat> unorderedSet){
    int amntConfigs = 0;
    vector<string> domains;
    int domainNo;
    for(int i = 0; i< unorderedSet.size();i++){
        domainNo= -1;
        pandaStat currStat = unorderedSet[i];
        string currDomain = currStat.domain;
        for(int n=0;n<domains.size();n++){
            if(domains[n] == currDomain){
               domainNo=n;
            }
        }
        if(domainNo == -1){
            domains.push_back(currDomain);
        }
    }
}

//prints multiple pandaStats into a csv file
void printStats(vector<pandaStat> stats, string file){
    ofstream outputStream(file);
    for(int i=0;i<stats.size();i++){
        stats[i].outputSingleStat_csv(outputStream);
    }
}

//compares multiple sets of stats
//returns only the problems that are solved in all sets of stats

vector<vector<pandaStat>> compareSolvedStats_orderedByConfig(vector<vector<pandaStat>> setOfStats){
    //contains every problem that occurs in setOfStats
    //the bool value indicates if the problems gets solved by every config
    vector<pair<string,bool>> problemList;
    for(vector<pandaStat> stats:setOfStats){
        for(pandaStat stat : stats){
            bool contains = false;
            int i = 0;
            for(pair<string,bool> problem : problemList){
                if(get<0>(problem)==stat.problem){
                    contains = true;
                    if(!stat.solved){
                        problemList[i].second=false;
                    }
                }
                i++;
            }
            if(!contains){
                problemList.push_back({stat.problem,stat.solved});
            }
        }
    }
    vector<vector<pandaStat>> result;
    for(vector<pandaStat> stats : setOfStats){
        vector<pandaStat> newStats;
        for(pandaStat stat : stats){
            for(pair<string,int> problem : problemList){
                if(stat.problem == get<0>(problem) && get<1>(problem)){
                    newStats.push_back(stat);
                }
            }
        }
        result.push_back(newStats);
    }

    return result;
}

//takes multiple csv files(with possibly different config) that contain stats of problems
//prints a csv file containing only problems that are solved by every input config
void compareAndPrintStats(vector<string> inputFiles,string outputFile,string avg_outputFile) {
    vector<vector<pandaStat>> setOfStats;
    for (string file: inputFiles) {
        setOfStats.push_back(readProblem_csv(file));
    }
    vector<vector<pandaStat>> intersecStats = compareSolvedStats_orderedByConfig(setOfStats);
    ofstream outputStream(outputFile);
    ofstream avg_outputStream(avg_outputFile);
    initAverageStats_csv(avg_outputStream, false);
    for (vector<pandaStat> currConfig: intersecStats) {
        //the set of stats(with the same config) are also need as array for calculate average method
        pandaStat currConfigStats[currConfig.size()];
        for (int i = 0; i < currConfig.size(); i++) {
            currConfig[i].outputSingleStat_csv(outputStream);
            currConfigStats[i] = currConfig[i];
        }
        pandaAverageStat newAverages = calculateAverages(currConfigStats, currConfig.size(), currConfig[0].domain,
                                                         currConfig[0].config);
        newAverages.outputAverageStats_csv(avg_outputStream, false);
    }
}

void compareAndPrintStats_obd(vector<string> inputFiles,string outputFile,string avg_outputFile){
    vector<vector<pandaStat>> setOfStats;
    for(string file : inputFiles){
        setOfStats.push_back(readProblem_csv(file));
    }
    vector<vector<pandaStat>> intersecStats= compareSolvedStats_orderedByConfig(setOfStats);
    ofstream outputStream(outputFile);
    ofstream avg_outputStream(avg_outputFile);
    initAverageStats_csv(avg_outputStream,true);
    for(vector<pandaStat> currConfig: intersecStats){
        //the set of stats(with the same config) are also need as array for calculate average method
        pandaStat currConfigStats[currConfig.size()];
        for(int i=0;i<currConfig.size();i++){
            currConfig[i].outputSingleStat_csv(outputStream);
            currConfigStats[i]=currConfig[i];
        }
        pandaAverageStat newAverages = calculateAverages(currConfigStats,currConfig.size(),currConfig[0].domain,currConfig[0].config);
        newAverages.outputAverageStats_csv(avg_outputStream, true);
    }
}


