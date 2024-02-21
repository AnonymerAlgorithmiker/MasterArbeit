#include "config.cpp"
#include "solver.cpp"
#include "stats.cpp"
#include "fileCoordinator.cpp"


using namespace std;
namespace fs = std::filesystem;

int main() {

    //printStatsPerProblem(outputFile,planDir);
    //printStatsPerDomain(outputFile,planDir);
    printStatsMultipleDomains(outputFile,planDir,true);

    return 0;
}
//string* engineResults = solveMultipleProblems(names,amntProblems);
//tuple<bool*,bool*,int*,int*,float*> stats= collectMultipleStats(engineResults,amntProblems);
//tuple<int,int,float,float,float> averages = calculateAverages(get<0>(stats),get<1>(stats),get<2>(stats),get<3>(stats),get<4>(stats),names,amntProblems);

//print stats
