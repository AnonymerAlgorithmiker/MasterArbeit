//
// Created by linus on 03.02.24.
//

#include <filesystem>
#include <iostream>
#include <array>

using namespace std;
//executes cmd and redirects it's output in result
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

string solveSingleProblem(string problem,string domainFile,string htn,string sas){
    //get the Filenames
    string problemPath=problem;

    //put the Filenames in the commands
    string parserCmd = parserPth+" "+domainFile +" "+problemPath+" "+htn;
    string grounderCmd = grounderPth +" "+htn+" "+sas;
    string engineCmd = enginePth+" "+engineConf+" "+sas;

    //execute commands
    const char* charParserCmd=parserCmd.c_str();
    const char* charGrounderCmd=grounderCmd.c_str();
    const char* charEngineCmd=engineCmd.c_str();
    string parserResult=exec(charParserCmd);
    string grounderResult=exec(charGrounderCmd);
    string engineResult=exec(charEngineCmd);
    //string engineResult ="";
    //return results of pandaPIengine
    return engineResult;
}

//will create Htn and Sas Paths with generic names
//multiple uses will overwrite those Files!
string* solveMultipleProblems(string* names,int sampleSize,string domain){
    string* results=new string[sampleSize];
    for(int i=0;i<sampleSize;i++){
        results[i]=solveSingleProblem(names[i], domain, getHtnPath(i,domain), getSasPath(i,domain));
    }
    return results;

}

