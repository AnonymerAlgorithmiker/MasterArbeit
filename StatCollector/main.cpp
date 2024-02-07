#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include "config.cpp"
#include "solver.cpp"
#include "stats.cpp"


using namespace std;
namespace fs = std::filesystem;


int main() {

    int amntSubDir = getDirTotalSize(planDir);
    string* subDirs = getSubDirNames(planDir,amntSubDir);
    for(int i=0;i< amntSubDir;i++){
        int amntProblems = getDirAmntProblems(subDirs[i]);
        string* names=getProblemNames(subDirs[i],amntProblems);
        string domainName = getDomainName(subDirs[i]);
        ofstream os;
        string outputFileName = "/home/linus/Git/PANDA/acyclic_problems/"+domainName+".txt";
        os.open(outputFileName);
        for(int n=0;n<amntProblems;n++){
            string domainFile=subDirs[i]+"/domain.hddl";
            string htn= getHtnPath(n,domainName);
            string sas = getSasPath(n,domainName);
            string result=solveSingleProblem(names[n],domainFile,htn,sas);
            tuple<bool,bool,float,float,float> stat = collectSingleStat(result);

            outputSingleStat(stat,names[n],os);
        }
        os.close();

        //string* engineResults = solveMultipleProblems(names,amntProblems);
        //tuple<bool*,bool*,int*,int*,float*> stats= collectMultipleStats(engineResults,amntProblems);
        //tuple<int,int,float,float,float> averages = calculateAverages(get<0>(stats),get<1>(stats),get<2>(stats),get<3>(stats),get<4>(stats),names,amntProblems);

        //print stats


    }
    return 0;
}
