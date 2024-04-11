#include "config.cpp"
#include "solver.cpp"
#include "fileCoordinator.cpp"

using namespace std;
namespace fs = std::filesystem;

int main() {

    //printStatsPerProblem(outputFile,planDir);
    //printStatsPerDomain(outputFile,planDir);
    //printStatsMultipleDomains(outputFile,planDir,true);
    //printCompleteStatsMultipleDomains(config::problemFile,config::average_obdFile,config::averageFile,config::planDir);
    compareStats();
    return 0;
}
