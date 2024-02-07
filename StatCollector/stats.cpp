//
// Created by linus on 03.02.24.
//


bool isFloat( string myString ) {
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail();
}

//analysing engineResult to return four values
//1. has the plan been solved
//2. is the problem cyclic
//3. the length of the solution
//4. the cost of the solution(cost of each action)
tuple<bool,bool,int,int,float> collectSingleStat(string result){
    bool solved = false;
    bool cyclic = false;
    int length = -1;
    int cost = -1;
    float time = -1;

    //search for
    istringstream iss(result);
    for (std::string line; std::getline(iss, line); )
    {
        if(line.find("- The problem is cyclic")!=string::npos){
            cyclic=true;
        }
        if (line.find("Status: ") != std::string::npos) {
            if(line.find("Solved") != string::npos){
                solved=true;
            }
        }
        if(solved && line.find("Found solution of length")!=string::npos){
            stringstream linestr(line);
            string word;
            while (linestr >> word){
            }
            length = stoi(word);
        }
        if(solved && line.find("Total costs of actions:")!=string::npos){
            stringstream linestr(line);
            string word;
            while (linestr >> word){
            }
            cost = stoi(word);
        }
        if(line.find("Search time")!=string::npos){
            stringstream linestr(line);
            string word;
            while(linestr >> word){
               if(isFloat(word)){
                   time = stof(word);
               }
            }
        }
    }
    return tuple<bool,bool,int,int,float>(solved,cyclic,length,cost,time);
}


tuple<bool*,bool*,int*,int*,float*> collectMultipleStats(string* results,int sampleSize){
    bool* solved=new bool[sampleSize];
    bool* cyclic=new bool[sampleSize];
    int* length=new int[sampleSize];
    int* cost=new int[sampleSize];
    float* time = new float[sampleSize];

    tuple<bool,bool,int,int,float> singleStats;
    for(int i=0;i<sampleSize;i++){
        singleStats=collectSingleStat(results[i]);
        solved[i]=get<0>(singleStats);
        cyclic[i]=get<1>(singleStats);
        length[i]=get<2>(singleStats);
        cost[i]=get<3>(singleStats);
        time[i]=get<4>(singleStats);
    }

    delete[] results;

    return tuple<bool*,bool*,int*,int*,float*>(solved,cyclic,length,cost,time);
}

tuple<int,int,float,float,float> calculateAverages(bool* solved,bool* cyclic,int* length,int* cost,float* time, string* problemNames,int amntProblems){
    int amntSolved=0;
    int amntCyclic=0;
    int sumLength=0;
    int sumCost=0;
    float sumTime=0;
    int i = 0;
    for(i=0;i<amntProblems;i++){
        if(solved[i]){
            amntSolved++;
            sumLength=sumLength+length[i];
            sumCost=sumCost+cost[i];
            sumTime=sumTime+time[i];
        }
        if(cyclic[i]){
            amntCyclic++;
        }else{
            /*cout << "the "<< i << "th instance is not cyclic" << endl;
            cout << "solved " << solved[i] << "length " << length[i] << "cost " << cost[i] << endl;
            cout << problemNames[i] << endl;*/
        }
    }
    float avgLength=sumLength/i;
    float avgCost=sumCost/i;
    float avgTime=sumTime/i;

    return tuple<int,int,float,float,float>(amntSolved,amntCyclic,avgLength,avgCost,avgTime);

}

void outputSingleStat(tuple<int,int,float,float,float> stat, string problemName, ofstream& outputFile){
    outputFile << "result for " << problemName << endl;
    outputFile << "searchconfig " << engineConf << endl;
    outputFile << "solved: " << get<0>(stat) << endl;
    outputFile << "cyclic: " <<  get<1>(stat)<< endl;
    outputFile << "length: " << get<2>(stat) << endl;
    outputFile << "cost " << get<3>(stat) << endl;
    outputFile << "time " << get<4>(stat) << endl;

}

void outputMultipleStats(tuple<int*,int*,float*,float*,float*> averages, string* problemNames, string dir, int amntProblems, ofstream outputFile){
    outputFile << "results for " << dir << endl;
    outputFile << "searchconfig: "<< engineConf << endl;
    outputFile << "solved: " << get<0>(averages) << "from "<< amntProblems <<endl;
    outputFile << "cyclic: " <<  get<1>(averages)<< "from "<< amntProblems << endl;
    outputFile << "average length: " << get<2>(averages) << endl;
    outputFile << "average cost " << get<3>(averages) << endl;
    outputFile << "average time " << get<4>(averages) << endl;
}
