//
// Created by linus on 26.03.24.
//

#include "maxMethod.h"
#include "queue";

void calculateMaxMethod(Model *htn,searchNode *n,int* method_max) {

    if (htn->numCyclicSccs == 0) {
        //max_action_algorithm approach 1
        //for (int task_x = pg->reachableTasksSet.getFirst(); (task_x >= 0) && (task_x < htn->numActions); task_x = pg->reachableTasksSet.getNext()) {
        for (int method_x = 0; method_x < htn->numMethods; method_x++) {
            method_max[method_x] = 0;

            //1.Value:m-> Method that produces
            //2.Value:n -> task x that will be produced
            //Method -1 is a dummy task that decomposes x into tx
            std::deque<tuple<int, int>> producer;
            tuple<int, int> initialProducer = {method_x, htn->decomposedTask[method_x]};
            //queue to keep track of all direct and indirect producer of task_x
            producer.push_back(initialProducer);

            //datastructure to store contributions of tasks that cant be calculated right away
            deque<tuple<int, deque<int>>> postCalcContributions[htn->numTasks];
            //if contribution of task cant be calculated right away contribution[task] = -1
            int contribution[htn->numTasks];
            for (int i = 0; i < htn->numTasks; i++) {
                //primitve tasks cant contribute to produce task_x since they cant be decomposed
                if (htn->isPrimitive[i]) {
                    contribution[i] = 0;
                } else {
                    contribution[i] = -1;
                }
                postCalcContributions[i] = {};
            }
            contribution[htn->decomposedTask[method_x]] = 1;

            while (!producer.empty()) {
                //try to calculate contribution of the next producer in Queue
                tuple<int, int> currProducer = producer.front();
                producer.pop_front();
                int currM = get<0>(currProducer);
                int currT = get<1>(currProducer);

                bool primitiveContribution = true;

                //new contribution will be the sum of all primitive contribution of the current producer
                int newContribution = 0;
                //newPostCalc is a list of all tasks that will be produced with currently unknown contribution to task_x
                deque<int> newPostCalc;


                if (currM != method_x) {
                    for (int subTaskNr = 0; subTaskNr < htn->numSubTasks[currM]; subTaskNr++) {
                        int currSubTask = htn->subTasks[currM][subTaskNr];
                        if (contribution[currSubTask] > -1) {
                            newContribution += contribution[currSubTask];
                        } else {
                            newPostCalc.push_back(currSubTask);
                            primitiveContribution = false;
                        }
                    }
                }

                //if all produced subtask are primitive or task_x then contribution is primitive and can be expressed by a simple int value
                if (primitiveContribution) {
                    contribution[htn->decomposedTask[currM]] = max(newContribution,
                                                                   contribution[htn->decomposedTask[currM]]);
                } else {
                    //store all abstract tasks to calculate their contribution afterwards
                    contribution[htn->decomposedTask[currM]] = -1;
                    postCalcContributions[htn->decomposedTask[currM]].push_back({newContribution, newPostCalc});
                }

                //search new producer by iterating through all Methods with currN as subtask
                int nextT = -1;
                if (currM == -1) {
                    nextT = currT;
                } else {
                    nextT = htn->decomposedTask[currM];
                }
                for (int i = 0; i < htn->stToMethodNum[nextT]; i++) {
                    int nextM = htn->stToMethod[nextT][i];
                    tuple<int, int> indirectProducer = {nextM, nextT};
                    bool alreadyPushed = false;
                    for (tuple<int, int> elem: producer) {
                        bool first = get<0>(elem) == get<0>(indirectProducer);
                        bool second = get<1>(elem) == get<1>(indirectProducer);
                        alreadyPushed = first && second;
                    }
                    if (!alreadyPushed) {
                        //new producer found contribution needs to be calculated
                        producer.push_back(indirectProducer);
                    }
                }

            }
            bool postCalcFinished = false;
            while (!postCalcFinished) {
                postCalcFinished = true;
                for (int i = 0; i < htn->numTasks; i++) {
                    if (!postCalcContributions[i].empty()) {
                        //only stop postCalc if all stored tasks have been considered for their contribution
                        postCalcFinished = false;
                    } else if (contribution[i] == -1) {
                        //if there are no abstract task have been stored the task does not contribute toward production of task_x
                        contribution[i] = 0;
                    }
                    bool all_lists_empty = true;
                    int outerIt = 0;
                    //check for new primitive contribution
                    for (tuple<int, deque<int>> curr: postCalcContributions[i]) {
                        if (get<1>(curr).size() > 0) {
                            all_lists_empty = false;
                        }
                        deque<int>::iterator it = get<1>(curr).begin();
                        for (int z = 0; z < get<1>(curr).size(); z++) {
                            int tsk = get<1>(curr)[z];
                            if (contribution[tsk] > -1) {
                                //if contribution is known delete task from postCalc list
                                get<0>(curr) += contribution[tsk];
                                it = get<1>(curr).erase(it);
                                z--;
                            } else {
                                it++;
                            }
                        }
                        postCalcContributions[i][outerIt] = curr;
                        outerIt++;
                    }

                    if (all_lists_empty) {
                        //if all contributions of task_i towards task_x have been calculated
                        //the postCalcContribution can be stored as int in contribution array
                        while (!postCalcContributions[i].empty()) {
                            tuple<int, deque<int>> curr = postCalcContributions[i].front();
                            postCalcContributions[i].pop_front();
                            contribution[i] = max(contribution[i], get<0>(curr));
                        }
                    }
                }

            }
            //sum up every contribution of every task that is contained in the current node
            for (int i = 0; i < n->numContainedTasks; i++) {
                method_max[method_x] += (n->containedTaskCount[i]) * contribution[n->containedTasks[i]];
            }

        }
    }
}

void calculateMaxMethod2(Model *htn,searchNode *n,int* method_max){
    //max_method_algorithm approach 2
    if (htn->numCyclicSccs == 0) {
        deque<tuple<deque<int>, deque<int>>> fringe;
        deque<deque<int>> leafs;

        deque<tuple<deque<int>, deque<int>>> initialNode;
        deque<int> initialTasks;
        deque<int> initialMethods = {};
        for (int i = 0; i < n->numContainedTasks; i++) {
            for (int cnt = 0; cnt < n->containedTaskCount[i]; cnt++) {
                if (!htn->isPrimitive[n->containedTasks[i]]) {
                    initialTasks.push_back(n->containedTasks[i]);
                }
            }
        }
        fringe.push_back({initialMethods, initialTasks});

        while (!fringe.empty()) {
            tuple<deque<int>, deque<int>> currNode = fringe.front();
            fringe.pop_front();
            deque<int> currMethods = get<0>(currNode);
            deque<int> currTasks = get<1>(currNode);
            if (currTasks.empty()) {
                leafs.push_back(currMethods);
            } else {
                int tsk = currTasks.front();
                currTasks.pop_front();
                for (int i = 0; i < htn->numMethodsForTask[tsk]; i++) {
                    deque<int> newMethods = currMethods;
                    deque<int> newTasks = currTasks;
                    int currMethod = htn->taskToMethods[tsk][i];
                    for (int stNo = 0; stNo < htn->numSubTasks[currMethod]; stNo++) {
                        int nextSt = htn->subTasks[currMethod][stNo];
                        if (!htn->isPrimitive[nextSt]) {
                            newTasks.push_back(nextSt);
                        }
                    }
                    newMethods.push_back(currMethod);
                    fringe.push_back({newMethods, newTasks});
                }
            }

        }

        for (int method_x = 0; method_x < htn->numMethods; method_x++) {
            method_max[method_x] = 0;
            for (deque<int> leaf: leafs) {
                int occInLeaf = 0;
                for (int elem: leaf) {
                    if (elem == method_x) {
                        occInLeaf++;
                    }
                }
                method_max[method_x] = max(method_max[method_x], occInLeaf);
            }
        }
    }
}

void subtractMethodsCurrNode(searchNode *n,int* method_max,int* curr_max,int numMethods){
    for(int i = 0; i<numMethods;i++){
        curr_max[i]=method_max[i];
    }
    solutionStep* sost = n->solution;
    bool done = sost == nullptr || sost->prev == nullptr;
    while (!done) {
        if(sost->method > -1){
            curr_max[sost->method]--;
        }
        done = sost->prev == nullptr;
        sost = sost->prev;
    }
}
