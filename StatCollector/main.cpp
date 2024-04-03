#include "config.cpp"
#include "solver.cpp"
#include "statCollector.cpp"
#include "fileCoordinator.cpp"
#include "statCompare.cpp"


using namespace std;
namespace fs = std::filesystem;

int main() {

    //printStatsPerProblem(outputFile,planDir);
    //printStatsPerDomain(outputFile,planDir);
    //printStatsMultipleDomains(outputFile,planDir,true);
    printCompleteStatsMultipleDomains(config::problemFile,config::average_obdFile,config::averageFile,config::planDir);

    return 0;
}
