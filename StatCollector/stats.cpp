//
// Created by linus on 03.02.24.
//
#include <fstream>

bool isFloat( string myString ) {
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail();
}

//analysing engineResult to return six values
//1. has the plan been solved
//2. has an error occured
//3. has an Ilo Cplex Error occured
//4. the length of the solution
//5. the cost of the solution(cost of each action)
//6  the computation time
tuple<bool,bool,bool,int,int,float> collectSingleStat(string result){
    bool solved = false;
    bool error = true;
    bool cPlexError = false;
    int length = -1;
    int cost = -1;
    float time = -1;

    istringstream iss(result);
    for (std::string line; std::getline(iss, line); )
    {
        if(line.find("IloCplex::Exception")!=string::npos){
            cPlexError= true;
        }
        if(line.find("-Success")!=string::npos){
            error=false;
        }
        if(line.find("-Failure")!=string::npos){
            error=true;
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
    }
    return tuple<bool,bool,bool,int,int,float>(solved,error,cPlexError,length,cost,time);
}

//Given multiple outputs of Pandaengine this method will return an Array version of the stats in collectSingleStats
tuple<bool*,bool*,bool*,int*,int*,float*> collectMultipleStats(string* results,int sampleSize){
    bool* solved=new bool[sampleSize];
    bool* error=new bool[sampleSize];
    bool* cplexError=new bool[sampleSize];
    int* length=new int[sampleSize];
    int* cost=new int[sampleSize];
    float* time = new float[sampleSize];

    tuple<bool,bool,bool,int,int,float> singleStats;
    for(int i=0;i<sampleSize;i++){
        singleStats=collectSingleStat(results[i]);
        solved[i]=get<0>(singleStats);
        error[i]=get<1>(singleStats);
        cplexError[i]=get<2>(singleStats);
        length[i]=get<3>(singleStats);
        cost[i]=get<4>(singleStats);
        time[i]=get<5>(singleStats);
    }

    delete[] results;

    return tuple<bool*,bool*,bool*,int*,int*,float*>(solved,error,cplexError,length,cost,time);
}

//merges two sets of stats together and deleting the individual sets
tuple<bool*,bool*,bool*,int*,int*,float*> addStats(tuple<bool*,bool*,bool*,int*,int*,float*> stat1,int sampleSize1,tuple<bool*,bool*,bool*,int*,int*,float*> stat2, int sampleSize2){
    int resultSampleSize=sampleSize1+sampleSize2;
    bool* resultSolved = new bool[resultSampleSize];
    bool* resultError = new bool[resultSampleSize];
    bool* resultCPlex = new bool[resultSampleSize];
    int* resultLength = new int[resultSampleSize];
    int* resultCosts = new int[resultSampleSize];
    float* resultTime = new float[resultSampleSize];


    for(int i=0;i<resultSampleSize;i++){
        if(i<sampleSize1){
            resultSolved[i]=get<0>(stat1)[i];
            resultError[i]=get<1>(stat1)[i];
            resultCPlex[i]=get<2>(stat1)[i];
            resultLength[i]=get<3>(stat1)[i];
            resultCosts[i]=get<4>(stat1)[i];
            resultTime[i]=get<5>(stat1)[i];
        }
        if(i>=sampleSize1){
            resultSolved[i]=get<0>(stat2)[i-sampleSize1];
            resultError[i]=get<1>(stat2)[i-sampleSize1];
            resultCPlex[i]=get<2>(stat2)[i-sampleSize1];
            resultLength[i]=get<3>(stat2)[i-sampleSize1];
            resultCosts[i]=get<4>(stat2)[i-sampleSize1];
            resultTime[i]=get<5>(stat2)[i-sampleSize1];
        }
    }

    tuple<bool*,bool*,bool*,int*,int*,float*> result=tuple<bool*,bool*,bool*,int*,int*,float*>(resultSolved,resultError,resultCPlex,resultLength,resultCosts,resultTime);

    return  result;
}


tuple<int,int,int,float,float,float> calculateAverages(tuple<bool*,bool*,bool*,int*,int*,float*> stats, int amntProblems){
    bool* solved = get<0>(stats);
    bool* error = get<1>(stats);
    bool* cPlex = get<2>(stats);
    int* length = get<3>(stats);
    int* cost = get<4>(stats);
    float* time = get<5>(stats);
    int amntSolved=0;
    int amntError=0;
    int amntCplexError=0;
    int sumLength=0;
    int sumCost=0;
    float sumTime=0;
    int i = 0;
    for(i=0;i<amntProblems;i++){
        if(solved[i]){
            amntSolved++;
            sumLength=sumLength+length[i];
            sumCost=sumCost+cost[i];
            sumTime=sumTime+time[i];
        }
        if(error[i]){
            amntError++;
        }
        if(cPlex[i]){
            amntCplexError++;
        }
    }
    float avgLength=sumLength/i;
    float avgCost=sumCost/i;
    float avgTime=sumTime/i;

    return tuple<int,int,int,float,float,float>(amntSolved,amntError,amntCplexError,avgLength,avgCost,avgTime);

}

void outputSingleStat(tuple<int,int,int,float,float,float> stat, string problemName, ofstream& outputFile){
    outputFile << "result for " << problemName << endl;
    outputFile << "searchconfig " << engineConf << endl;
    outputFile << "solved: " << get<0>(stat) << endl;
    outputFile << "Error: " <<  get<1>(stat)<< endl;
    outputFile << "CPlex Error: " <<  get<2>(stat)<< endl;
    outputFile << "length: " << get<3>(stat) << endl;
    outputFile << "cost " << get<4>(stat) << endl;
    outputFile << "time " << get<5>(stat) << endl;

}

void outputAverageStats(tuple<int,int,int,float,float,float> averages, string domain, int amntProblems, ofstream& outputFile){
    outputFile << "results for " << domain << endl;
    outputFile << "searchconfig: "<< engineConf << endl;
    outputFile << "solved: " << get<0>(averages) << "from "<< amntProblems <<endl;
    outputFile << "Error: " <<  get<1>(averages)<< "from "<< amntProblems << endl;
    outputFile << "CPlex Error: " <<  get<2>(averages)<< "from "<< amntProblems << endl;
    outputFile << "average length: " << get<3>(averages) << endl;
    outputFile << "average cost " << get<4>(averages) << endl;
    outputFile << "average time " << get<5>(averages) << endl;
}
