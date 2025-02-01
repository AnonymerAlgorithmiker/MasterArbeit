//
// Created by linus on 26.03.24.
//
#include "maxAction.h"
#include <queue>

void calculateMaxAction(Model *htn,searchNode *n,int* task_max) {

    if (htn->numCyclicSccs == 0) {
        //max_action_algorithm approach 1
        //for (int task_x = pg->reachableTasksSet.getFirst(); (task_x >= 0) && (task_x < htn->numActions); task_x = pg->reachableTasksSet.getNext()) {
        for (int task_x = 0; task_x < htn->numTasks; task_x++) {
            task_max[task_x] = 0;

            //1.Value:m-> Method that produces
            //2.Value:n -> task x that will be produced
            //Method -1 is a dummy task that decomposes x into tx
            std::deque<tuple<int, int>> producer;
            tuple<int, int> initialProducer = {-1, task_x};
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
            contribution[task_x] = 1;

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


                if (currM != -1) {
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

                //if all produced subtask are primitive or task_x then contribution is primitve and can be expressed by a simple int value
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
                task_max[task_x] += (n->containedTaskCount[i]) * contribution[n->containedTasks[i]];
            }

        }
    }
}
void calculateMaxAction2(Model *htn,searchNode *n,int* task_max) {
    if (htn->numCyclicSccs == 0) {
        //max_action_algorithm approach 2
        deque <deque<tuple<int, bool>>> fringe;
        deque <deque<tuple<int, bool>>> leafs;

        deque <tuple<int, bool>> initialBoolNode;
        for (int i = 0; i < n->numContainedTasks; i++) {
            for (int cnt = 0; cnt < n->containedTaskCount[i]; cnt++) {
                initialBoolNode.push_back({n->containedTasks[i], false});
            }
        }
        fringe.push_back(initialBoolNode);

        while (!fringe.empty()) {
            deque <tuple<int, bool>> currBoolNode = fringe.front();
            fringe.pop_front();
            bool isLeaf = true;
            for (int elemNo = 0; elemNo < currBoolNode.size(); elemNo++) {
                tuple<int, bool> elem = currBoolNode.at(elemNo);
                if (!htn->isPrimitive[get<0>(elem)] && !get<1>(elem)) {
                    int currElemTask = get<0>(elem);
                    for (int i = 0; i < htn->numMethodsForTask[currElemTask]; i++) {
                        currBoolNode[elemNo] = {currElemTask, true};
                        int currMethod = htn->taskToMethods[currElemTask][i];

                        deque <tuple<int, bool>> newNode;
                        std::copy(currBoolNode.begin(), currBoolNode.end(), std::back_inserter(newNode));
                        for (int st = 0; st < htn->numSubTasks[currMethod]; st++) {
                            newNode.push_back({htn->subTasks[currMethod][st], false});
                        }
                        fringe.push_back(newNode);
                    }
                    isLeaf = false;
                }
            }
            if (isLeaf) {
                leafs.push_back(currBoolNode);
            }
        }

        for (int task_x = 0; task_x < htn->numTasks; task_x++) {
            task_max[task_x] = 0;
            for (deque <tuple<int, bool>> leaf: leafs) {
                int occInLeaf = 0;
                for (tuple<int, bool> elem: leaf) {
                    if (get<0>(elem) == task_x) {
                        occInLeaf++;
                    }
                }
                task_max[task_x] = max(task_max[task_x], occInLeaf);
            }
        }
    }
}

void subtractActionsCurrNode(searchNode *n,int* task_max,int* curr_max,int numTasks){
    for(int i = 0; i<numTasks;i++){
        curr_max[i]=task_max[i];
    }
    solutionStep* sost = n->solution;
    bool done = sost == nullptr || sost->prev == nullptr;
    while (!done) {
        curr_max[sost->task]--;
        done = sost->prev == nullptr;
        sost = sost->prev;
    }
}

