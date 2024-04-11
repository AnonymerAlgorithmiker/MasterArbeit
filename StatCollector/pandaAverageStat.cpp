
//
// Created by linus on 07.03.24.
//

class pandaAverageStat {

public:
    int amntSolved;
    int amntRamError;
    int amntTimeError;
    int amntAssertionError;
    float avgLength;
    float avgCost;
    float avgSearchnodes;
    float avgTime;
    string domain;
    string config;
    int sampleSize;

    pandaAverageStat(){
        amntSolved=0;
        amntRamError=0;
        amntTimeError=0;
        amntAssertionError=0;
        avgLength=0;
        avgCost=0;
        avgTime=0;
        avgSearchnodes=0;
        sampleSize=0;
    }
    pandaAverageStat(int amntSolved, int amntRamError, int amntTimeError, int amntAssertionError, float avgLength, float avgCost, float avgSearchnodes, float avgTime,int sampleSize){
        this->amntSolved=amntSolved;
        this->amntRamError=amntRamError;
        this->amntTimeError=amntTimeError;
        this->amntAssertionError=amntAssertionError;
        this->avgLength=avgLength;
        this->avgCost=avgCost;
        this->avgTime=avgTime;
        this->avgSearchnodes=avgSearchnodes;
        this->sampleSize=sampleSize;
    }
    pandaAverageStat(int amntSolved, int amntRamError, int amntTimeError, int amntAssertionError, float avgLength, float avgCost, float avgSearchnodes, float avgTime,string domain, string config,int sampleSize){
        this->amntSolved=amntSolved;
        this->amntRamError=amntRamError;
        this->amntTimeError=amntTimeError;
        this->amntAssertionError=amntAssertionError;
        this->avgLength=avgLength;
        this->avgCost=avgCost;
        this->avgTime=avgTime;
        this->avgSearchnodes=avgSearchnodes;
        this->sampleSize=sampleSize;
    }

    void addContext(string domain, string config){
        this->domain=domain;
        this->config=config;
    }

    void outputAverageStats(ofstream& outputFile){
        outputFile << "Results for: "  << domain << endl;
        outputFile << "Searchconfig: " << config << endl;
        outputFile << "Solved: " << amntSolved << " from " << sampleSize << endl;
        outputFile << "Out of Ram: " << +amntRamError<< " from " << sampleSize  << endl;
        outputFile << "Out of Time: " << amntTimeError << " from " << sampleSize << endl;
        outputFile << "Assertion Error: " << amntAssertionError << " from " << sampleSize << endl;
        outputFile << "Average Length: " << avgLength<< endl;
        outputFile << "Average Cost: " << avgCost << endl;
        outputFile << "Searchnodes: " << avgSearchnodes << endl;
        outputFile << "Average time in s " << avgTime << endl;
    }

    void outputAverageStats_csv(ofstream& outputFile,bool orderedByDomain){
        if(orderedByDomain){
            outputFile << domain << ",";
        }
        outputFile << config << ",";
        outputFile << amntSolved << ",";
        outputFile << amntRamError<< ",";
        outputFile << amntTimeError << ",";
        outputFile << amntAssertionError << ",";
        outputFile << avgLength<< ",";
        outputFile << avgCost << ",";
        outputFile << avgSearchnodes << ",";
        outputFile << avgTime << ",";
        outputFile << sampleSize <<",";
        outputFile << endl;
    }
};


