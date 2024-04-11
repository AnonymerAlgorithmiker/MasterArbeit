//
// Created by linus on 03.02.24.
//

#include <filesystem>

namespace fs = std::filesystem;
using namespace std;
namespace config{
//Paths to the various directory configure for your own System
    const string parserPth="~/panda/pandaPIparser/pandaPIparser";
    const string grounderPth="~/panda/pandaPIgrounder/pandaPIgrounder -q";
    const string enginePth="~/panda/pandaPIengine/build/pandaPIengine";
    const string planDir="/home/linus/panda/StatCollectorPlans/StatCollectorInput";
    const string htnDir="~/panda/StatCollectorPlans/htn";
    const string sasDir="~/panda/StatCollectorPlans/sas";
    const string outputFile="/home/linus/panda/StatCollectorPlans/StatCollectorOutput/stats.txt";
    const string problemFile="/home/linus/panda/StatCollectorPlans/StatCollectorOutput/problems.csv";
    const string average_obdFile="/home/linus/panda/StatCollectorPlans/StatCollectorOutput/average_obd.csv";
    const string averageFile="/home/linus/panda/StatCollectorPlans/StatCollectorOutput/average_allDomains.csv";
    const string compareFile="/home/linus/panda/StatCollectorPlans/StatCollectorOutput/compare.csv";
    const string compareFile_avg="/home/linus/panda/StatCollectorPlans/StatCollectorOutput/compare_averages.csv";
    const string compareDir="/home/linus/panda/StatCollectorPlans/statcompare/";

    //const string confName = "C1-C8|Max-Method_and_Action|FirstNode|Subtraction";
    const string confName = "No Constraints";
//Name of the Domain is only used to generate Names for Sas and htn files
//C1-C4 & C7-C8
//const string engineConf = "--suboptimal --gValue=none --heuristic=\"dof(netchange=none;lmclmc=none;pg=relaxed;tdg=allowUC)\"";
//C1-C8
//const string engineConf = "--suboptimal --gValue=none --heuristic=\"dof(netchange=none;lmclmc=none;pg=full;tdg=allowUC)\"";
//C1-C12
//const string engineConf = "--suboptimal --gValue=none --heuristic=\"dof(netchange=none;lmclmc=none;pg=full;tdg=uc)\"";
//No Constraints
const string engineConf = "--suboptimal --gValue=none --heuristic=\"dof(netchange=none;lmclmc=none;pg=none;tdg=allowUC)\"";
}

//creates List fo all Files in dir
string* getProblemNames(string dir,int amntFiles){
    string* names=new string[amntFiles];
    int i =0;
    for (const auto & entry : fs::directory_iterator(dir)){
        string curr = entry.path();
        if(curr.find("domain.hddl")==string::npos && curr.find(".hddl")!=string::npos){
            names[i]=entry.path();
            i++;
        }
    }
    return names;
}
string* getSubDirNames(string dir,int amntFiles){
    string* names=new string[amntFiles];
    int i =0;
    for (const auto & entry : fs::directory_iterator(dir)){
        string curr = entry.path();
        names[i]=entry.path();
        i++;
    }
    return names;
}

string getDomainName(string dir){
    string name="notFound";
    size_t lastSlash = dir.rfind("/");
    if(lastSlash != string::npos){
        name = dir.substr(lastSlash+1);
    }
    return name;
}

int getDirTotalSize(string dir){
    int i =0;
    for (const auto & entry : fs::directory_iterator(dir)){
        i++;
    }
    return i;
}

int getDirAmntProblems(string dir){
    int i =0;
    for (const auto & entry : fs::directory_iterator(dir)){
        string curr = entry.path();
        if(curr.find("domain.hddl")==string::npos && curr.find(".hddl")!=string::npos){
            i++;
        }
    }
    return i;
}

//create a generic name for the htn file related to the ith planning problem
string getHtnPath(int i, string domain){
    string result;
    if(i<10){
        result=config::htnDir+"/"+domain+"_0"+to_string(i)+".htn";
    } else{

        result=config::htnDir+"/"+domain+"_"+to_string(i)+".htn";
    }
    return result;
}

//create a generic name for the sas file related to the ith planning problem
string getSasPath(int i, string domain){
    string result;
    if(i<10){
        result=config::sasDir+"/"+domain+to_string(i)+".sas";
    } else{

        result=config::sasDir+"/"+domain+to_string(i)+".sas";
    }
    return result;
}
