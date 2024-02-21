//
// Created by linus on 03.02.24.
//

#include <iostream>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>

using namespace std;

//Decodes the Command into one of the three Panda programs
string cmdName(const char* cmd){
    string cmdName="Unknown Command";
    string engine = "pandaPIengine";
    string parser = "pandaPIparser";
    string grounder = "pandaPIgrounder";

    int multOccurunceCheck=0;
    if(((string)cmd).find(engine)!=string::npos){
        cmdName=engine;
        multOccurunceCheck++;
    }
    if(((string)cmd).find(parser)!=string::npos){
        cmdName=parser;
        multOccurunceCheck++;
    }
    if(((string)cmd).find(grounder)!=string::npos){
        cmdName=grounder;
        multOccurunceCheck++;
    }
    if(multOccurunceCheck>=2){
        cmdName="Mixed Signals :/";
    }
    return cmdName;
}


std::string exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    auto pipe = popen(cmd, "r");

    if (!pipe) throw std::runtime_error("popen() failed!");

    while (!feof(pipe))
    {
        if (fgets(buffer.data(), 128, pipe) != nullptr)
            result += buffer.data();
    }

    auto rc = pclose(pipe);
    //Adding "Success" or "Failure" string to result for Stat Evaluation
    if (rc == EXIT_SUCCESS)
    {
        std::cout << "Programm: " << cmdName(cmd) << endl;
        std::cout << "SUCCESS\n";
        result+="\n-Success";
    }
    else
    {
        std::cout << "Programm: " << cmdName(cmd) << endl;
        std::cout << "FAILED\n";
        result+="\n-Failure";
    }

    return result;
}



string solveSingleProblem(string problem,string domainFile,string htn,string sas){
    //get the Filenames
    string problemPath=problem;

    //put the Filenames in the commands
    string parserCmd = parserPth+" "+domainFile +" "+problemPath+" "+htn +" 2>&1";
    string grounderCmd = grounderPth +" "+htn+" "+sas;
    string engineCmd = "systemd-run --user -G --pipe -p MemoryMax=6000M "+ enginePth+" "+engineConf+" "+sas +" 2>&1";


    //execute commands
    const char* charParserCmd=parserCmd.c_str();
    const char* charGrounderCmd=grounderCmd.c_str();
    const char* charEngineCmd=engineCmd.c_str();
    string parserResult=exec(charParserCmd);
    string grounderResult=exec(charGrounderCmd);
    string engineResult=exec(charEngineCmd);

    //return results of pandaPIengine
    return engineResult;
}

//will create Htn and Sas Paths with generic names
//multiple uses will overwrite those Files!
string* solveMultipleProblems(string* names,int sampleSize,string domainName,string domainFile){
    string* results=new string[sampleSize];
    for(int i=0;i<sampleSize;i++){
        cout << "Domain: " << domainName << endl;
        cout << "Problem : " << (i+1) << "von " << sampleSize << endl;
        results[i]=solveSingleProblem(names[i], domainFile, getHtnPath(i,domainName), getSasPath(i,domainName));
    }
    return results;

}

