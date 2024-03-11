#include "config.cpp"
#include "solver.cpp"
#include "statCollector.cpp"
#include "fileCoordinator.cpp"


using namespace std;
namespace fs = std::filesystem;

int main() {

    //printStatsPerProblem(outputFile,planDir);
    //printStatsPerDomain(outputFile,planDir);
    printStatsMultipleDomains(outputFile,planDir,true);

    return 0;
}
