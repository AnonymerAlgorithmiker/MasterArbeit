//
// Created by linus on 16.02.24.
//
#include <fstream>
#include "util.cpp"
#include "statCompare.cpp"
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
    problemNames = sortAlphabetically(problemNames,amntProblems);
    string domainName = getDomainName(domainDir);
    string domainFile = domainDir + "/domain.hddl";

    for (int i = 0; i < amntProblems; i++) {
        cout << "Problem: " << i << "von " << amntProblems << endl;
        string htn = getHtnPath(i, domainName);
        string sas = getSasPath(i, domainName);
        string result = solveSingleProblem(problemNames[i], domainFile, htn, sas);
        pandaStat stat = collectSingleStat(result,domainName,problemNames[i],config::confName);

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
    stats = collectMultipleStats(result,amntProblems,problemNames,domainName,config::engineConf);

    pandaAverageStat averages = calculateAverages(stats,amntProblems,domainName,config::confName);
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
        stats = collectMultipleStats(result,amntProblems,problemNames, currDomain,config::confName);

        if(orderedByDomain){
            pandaAverageStat averages = calculateAverages(stats,amntProblems,currDomain,config::confName);
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
        pandaAverageStat averages = calculateAverages(allStats,currSampleSize,"Mixed Domains",config::confName);
        averages.outputAverageStats(os);
    }

    os.close();
}


void printCompleteStatsMultipleDomains(string problemFile,string averageFile_obd,string averageFile,string dir){
    ofstream problemStream(problemFile);
    ofstream average_obdStream(averageFile_obd);
    ofstream averageStream(averageFile);
    init_problem_csv(problemStream);
    initAverageStats_csv(average_obdStream,true);
    initAverageStats_csv(averageStream,false);

    int dirTotalSize = getDirTotalSize(dir);
    string* subDirs = getSubDirNames(dir,dirTotalSize);

    pandaStat* allStats;
    int currSampleSize=0;

    for(int i=0;i<dirTotalSize;i++){
        string currSubDir = subDirs[i];
        string currDomain = getDomainName(currSubDir);
        int amntProblems = getDirAmntProblems(currSubDir);
        string *problemNames = getProblemNames(currSubDir, amntProblems);
        problemNames = sortAlphabetically(problemNames,amntProblems);
        string domainFile = currSubDir + "/domain.hddl";

        pandaStat* stats;
        string* result = solveMultipleProblems(problemNames,amntProblems,currDomain,domainFile);
        stats = collectMultipleStats(result,amntProblems,problemNames, currDomain,config::confName);

        //output every single stat
        for(int problem=0;problem<amntProblems;problem++){
            stats[problem].outputSingleStat_csv(problemStream);
        }

        //output averagestats ordered by domain
        pandaAverageStat averages = calculateAverages(stats,amntProblems,currDomain,config::confName);
        averages.outputAverageStats_csv(average_obdStream,true);

        if(i==0){
            allStats=stats;
            currSampleSize=amntProblems;
        }else{
            allStats= addStats(allStats,stats,currSampleSize,amntProblems);
            currSampleSize+= amntProblems;
        }
    }
    //output averagestats if all domains combined
    pandaAverageStat averages = calculateAverages(allStats,currSampleSize,"Mixed Domains",config::confName);
    averages.outputAverageStats_csv(averageStream,false);

    problemStream.close();
    averageStream.close();
    average_obdStream.close();
}

void compareStats(bool orderedByDomain){
    int dirTotalSize = getDirTotalSize(config::compareDir);
    string* statNames = getSubDirNames(config::compareDir,dirTotalSize);
    vector<string> statNameVec;
    for(int i=0;i<dirTotalSize;i++){
        statNameVec.push_back(statNames[i]);
    }
    if(orderedByDomain){
        compareAndPrintStats_obd(statNameVec,config::compareFile,config::compareFile_avg);
    }else{
        compareAndPrintStats(statNameVec,config::compareFile,config::compareFile_avg);
    }
}
