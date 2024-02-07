//
// Created by linus on 03.02.24.
//

#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

//Paths to the various directory configure for your own System
const string parserPth="/home/linus/Git/PANDA/pandaPIparser/pandaPIparser";
const string grounderPth="/home/linus/Git/PANDA/pandaPIgrounder/pandaPIgrounder -q";
const string enginePth="/home/linus/Git/PANDA/pandaPIengine/build/pandaPIengine";
const string planDir="/home/linus/Git/PANDA/acyclic_problems/hddl";
const string htnDir="/home/linus/Git/PANDA/htn";
const string sasDir="/home/linus/Git/PANDA/sas";

//Name of the Domain is only used to generate Names for Sas and htn files
const string engineConf= "-p --heuristic=\"dof(pg=relaxed;tdg=allowUC)\"";
//const string engineConf = "";
//const string engineConf= "--timelimit=30";

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
        result=htnDir+"/"+domain+"_0"+to_string(i)+".htn";
    } else{

        result=htnDir+"/"+domain+"_"+to_string(i)+".htn";
    }
    return result;
}

//create a generic name for the sas file related to the ith planning problem
string getSasPath(int i, string domain){
    string result;
    if(i<10){
        result=sasDir+"/"+domain+to_string(i)+".sas";
    } else{

        result=sasDir+"/"+domain+to_string(i)+".sas";
    }
    return result;
}
