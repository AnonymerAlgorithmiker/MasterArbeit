//
// Created by linus on 16.02.24.
//
#include <fstream>
//will return detailed stats of every problem in dir to outputFile
//dir has to contain a subDir with a name equal to the domain name
//this subDir has to contain a domain File and one or multiple problem files
void printStatsPerProblem(string outputFileName,string dir) {
    ofstream os;
    os.open(outputFileName);

    int dirTotalSize = getDirTotalSize(dir);
    if(dirTotalSize>1){
        cout << "Multiple Domains detected Exiting..." << endl;
        return;
    }
    string* subDirs = getSubDirNames(dir,dirTotalSize);
    string domainDir = subDirs[0];

    int amntProblems = getDirAmntProblems(domainDir);
    string *problemNames = getProblemNames(domainDir, amntProblems);
    string domainName = getDomainName(domainDir);
    string domainFile = domainDir + "/domain.hddl";

    for (int i = 0; i < amntProblems; i++) {
        cout << "Problem: " << i << "von " << amntProblems << endl;
        string htn = getHtnPath(i, domainName);
        string sas = getSasPath(i, domainName);
        string result = solveSingleProblem(problemNames[i], domainFile, htn, sas);
        pandaStat stat = collectSingleStat(result,domainName,problemNames[i],engineConf);

        stat.outputSingleStat(os);
    }
    os.close();
}
//will return the average stats of the problems in dir to outputFile
//dir has to contain a subDir with a name equal to the domain name
//this subdir hast to contain a domain File and one or multiple problem Files
void printStatsPerDomain(string outputFileName,string dir){
    ofstream os;
    os.open(outputFileName);

    int dirTotalSize = getDirTotalSize(dir);
    if(dirTotalSize>1){
        cout << "Multiple Domains detected Exiting..." << endl;
        return;
    }
    string* subDirs = getSubDirNames(dir,dirTotalSize);
    string domainDir = subDirs[0];

    int amntProblems = getDirAmntProblems(domainDir);
    string *problemNames = getProblemNames(domainDir, amntProblems);
    string domainName = getDomainName(domainDir);
    string domainFile = domainDir + "/domain.hddl";

    pandaStat* stats;
    string* result = solveMultipleProblems(problemNames,amntProblems,domainName,domainFile);
    stats = collectMultipleStats(result,amntProblems,problemNames,domainName,engineConf);

    pandaAverageStat averages = calculateAverages(stats,amntProblems,domainName,engineConf);
    averages.outputAverageStats(os);
    os.close();
}

//will return the average stats of the problems in dir and their subdirs to outputFile
//dir has to contain one subDir per domain that contains one or multiple problem Files and a domain file
//If orderedByDomain==true the averages will be calculated per domain else they will be calculated overall
void printStatsMultipleDomains(string outputFileName,string dir,bool orderedByDomain){
    ofstream os;
    os.open(outputFileName);

    int dirTotalSize = getDirTotalSize(dir);
    string* subDirs = getSubDirNames(dir,dirTotalSize);

    pandaStat* allStats;
    int currSampleSize=0;

    for(int i=0;i<dirTotalSize;i++){
        string currSubDir = subDirs[i];
        string currDomain = getDomainName(currSubDir);
        int amntProblems = getDirAmntProblems(currSubDir);
        string *problemNames = getProblemNames(currSubDir, amntProblems);
        string domainFile = currSubDir + "/domain.hddl";

        pandaStat* stats;
        string* result = solveMultipleProblems(problemNames,amntProblems,currDomain,domainFile);
        stats = collectMultipleStats(result,amntProblems,problemNames, currDomain,engineConf);

        if(orderedByDomain){
            pandaAverageStat averages = calculateAverages(stats,amntProblems,currDomain,engineConf);
            averages.outputAverageStats(os);
        }else if(i==0){
           allStats=stats;
           currSampleSize=amntProblems;
        }else{
            allStats= addStats(allStats,stats,currSampleSize,amntProblems);
            currSampleSize+= amntProblems;
        }
    }
    if(!orderedByDomain){
        pandaAverageStat averages = calculateAverages(allStats,currSampleSize,"Mixed Domains",engineConf);
        averages.outputAverageStats(os);
    }

    os.close();
}
