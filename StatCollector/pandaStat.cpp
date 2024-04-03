//
// Created by linus on 07.03.24.
//

class pandaStat {

public:
    bool solved;
    bool ramError;
    bool timeError;
    bool assertionError;
    int length;
    int cost;
    int searchnodes;
    float time;
    string domain;
    string problem;
    string config;

    pandaStat(){
       solved = false;
       ramError=false;
       timeError=false;
       assertionError= false;
       length=-1;
       cost=-1;
       time=-1;
       searchnodes=-1;
       domain="unknown";
       problem="unknown";
       config="unknown";
    }
    pandaStat(bool solved, bool ramError, bool timeError, bool assertionError, int length, int cost, int searchnodes,float time){
        this->solved=solved;
        this->ramError=ramError;
        this->timeError=timeError;
        this->assertionError=assertionError;
        this->length=length;
        this->cost=cost;
        this->time=time;
        this->searchnodes=searchnodes;
    }
    pandaStat(bool solved, bool ramError, bool timeError, bool assertionError, int length, int cost, int searchnodes,float time, string domain, string problem, string config){
        this->solved=solved;
        this->ramError=ramError;
        this->timeError=timeError;
        this->assertionError=assertionError;
        this->length=length;
        this->cost=cost;
        this->searchnodes=searchnodes;
        this->time=time;
        this->problem=problem;
        this->domain=domain;
        this->config=config;
    }

    void addContext(string domain,string problem,string config){
       this->domain=domain;
       this->problem=problem;
       this->config=config;
    }

    void outputSingleStat(ofstream& outputFile){
        outputFile << "Results for: "  << problem << endl;
        outputFile << "Searchconfig: " << config << endl;
        outputFile << "Domain: " << domain << endl;
        outputFile << "Solved: " << solved << endl;
        outputFile << "Out of Ram: " << +ramError << endl;
        outputFile << "Out of Time: " << timeError << endl;
        outputFile << "Assertion Error: " << assertionError << endl;
        outputFile << "Length: " << length << endl;
        outputFile << "Cost: " << cost << endl;
        outputFile << "Searchnodes: " << searchnodes << endl;
        outputFile << "Time " << time << endl;
    }
    void outputSingleStat_csv(ofstream& outputFile){
        outputFile << problem << ",";
        outputFile << config << ",";
        outputFile << domain << ",";
        outputFile << solved << ",";
        outputFile << ramError << ",";
        outputFile << timeError << ",";
        outputFile << assertionError << ",";
        outputFile << length << ",";
        outputFile << cost << ",";
        outputFile << searchnodes << ",";
        outputFile << time << ",";
        outputFile << endl;
    }

};


