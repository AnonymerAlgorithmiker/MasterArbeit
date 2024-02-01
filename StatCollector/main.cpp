#include <iostream>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <sstream>


using namespace std;
const bool laptop=false;
const string domain="Hiking";
const int sampleSize=30;
const int offset=0;

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

string getProblemPath(int i){
    string result;
    string dirPath;
    if(laptop){
        dirPath= "~/Git/ipc2020-domains/total-order/"+domain;
    }else{
        dirPath= "~/Git/PANDA/ipc2020-domains/total-order/"+domain;
    }
    if(i<10){
        result=dirPath+"/p0"+to_string(i)+".hddl";
    } else{
        result=dirPath+"/p"+to_string(i)+".hddl";
    }
    return result;
}

string getHtnPath(int i){
    string result;
    string dirPath;
    if(laptop){
        dirPath= "~/Git/htn";
    }else{
        dirPath= "~/Git/PANDA/htn";
    }
    if(i<10){
        result=dirPath+"/"+domain+"_0"+to_string(i)+".htn";
    } else{

        result=dirPath+"/"+domain+"_"+to_string(i)+".htn";
    }
    return result;
}

string getSasPath(int i){
    string result;
    string dirPath;
    if(laptop){
        dirPath= "~/Git/sas";
    }else{
        dirPath= "~/Git/PANDA/sas";
    }
    if(i<10){
        result=dirPath+"/"+domain+to_string(i)+".sas";
    } else{

        result=dirPath+"/"+domain+to_string(i)+".sas";
    }
    return result;
}

string solveProblem(int i){
    string grounderPath;
    string parserPath;
    string domainPath;
    string enginePath;
    //Create Parser Command
    if(!laptop){
        parserPath="~/Git/PANDA/pandaPIparser/pandaPIparser";
        domainPath="~/Git/PANDA/ipc2020-domains/total-order/"+domain+"/domain.hddl";
        grounderPath="~/Git/PANDA/pandaPIgrounder/pandaPIgrounder -q";
        enginePath="~/Git/PANDA/pandaPIengine/build/pandaPIengine";
    }else{
        parserPath="~/Git/pandaPIparser/pandaPIparser";
        domainPath="~/Git/ipc2020-domains/total-order/"+domain+"/domain.hddl";
        grounderPath="~/Git/pandaPIgrounder/pandaPIgrounder -q";
        enginePath="~/Git/pandaPIengine/build/pandaPIengine";
    }
    //string engineConf= "-p --heuristic=\"dof(pg=full;tdg=uc)\"";
    string engineConf= "--timelimit=1";


    string problemPath=getProblemPath(i);
    string htnPath=getHtnPath(i);
    string sasPath= getSasPath(i);
    //Execute Parser Command => Creating Htn File
    string parserCmd = parserPath+" "+domainPath+" "+problemPath+" "+htnPath;
    string grounderCmd = grounderPath +" "+htnPath+" "+sasPath;
    string engineCmd = enginePath+" "+engineConf+" "+sasPath;
    const char* charParserCmd=parserCmd.c_str();
    const char* charGrounderCmd=grounderCmd.c_str();
    const char* charEngineCmd=engineCmd.c_str();
    string parserResult=exec(charParserCmd);
    string grounderResult=exec(charGrounderCmd);
    string engineResult=exec(charEngineCmd);

    return engineResult;
}

void collectStat(string engineResult,bool* solved,bool* cyclic,int* length,int* cost, int instance){
    bool currSolved = false;

    istringstream iss(engineResult);
    for (std::string line; std::getline(iss, line); )
    {
        if(line.find("- The problem is cyclic")!=string::npos){
           cyclic[instance]=true;
        }
        if (line.find("Status: ") != std::string::npos) {
            if(line.find("Solved") != string::npos){
                currSolved=true;
                solved[instance]=true;
            }else{
                solved[instance]=false;
            }
        }
        if(currSolved && line.find("Found solution of length")!=string::npos){
            stringstream linestr(line);
            string word;
            while (linestr >> word){
            }
            length[instance] = stoi(word);
        }
        if(currSolved && line.find("Total costs of actions:")!=string::npos){
            stringstream linestr(line);
            string word;
            while (linestr >> word){
            }
            cost[instance] = stoi(word);
        }
    }
    if(currSolved==false){
        cost[instance] = -1;
        length[instance] = -1;
    }
}



int main() {

    bool* solved=new bool[sampleSize];
    bool* cyclic=new bool[sampleSize];
    int* length=new int[sampleSize];
    int* cost=new int[sampleSize];
    string currRes;
    for(int i =offset+1;i<(offset+sampleSize+1);i++){
        currRes= solveProblem(i);
        collectStat(currRes,solved,cyclic,length,cost,i-1-offset);
    }

    int amntSolved=0;
    int amntCyclic=0;
    int sumLength=0;
    int sumCost=0;

    for(int i=0;i<sampleSize;i++){
        if(solved[i]){
            amntSolved++;
        }
        if(cyclic[i]){
            amntCyclic++;
        }else{
            cout << "the "<< i << "th instance is not cyclic" << endl;
            cout << "solved " << solved[i] << "length " << length[i] << "cost " << cost[i] << endl;
        }
        sumLength=sumLength+length[i];
        sumCost=sumCost+cost[i];
    }
    float avgLength=sumLength/sampleSize;
    float avgCost=sumCost/sampleSize;

    cout << "solved: " << amntSolved << endl;
    cout << "cyclic: " << amntCyclic << endl;
    cout << "average length: " << avgLength << endl;
    cout << "average cost " << avgCost << endl;


    return 0;
}
