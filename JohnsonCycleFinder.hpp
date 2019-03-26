//
//  JohnsonCycleFinder.hpp
//  KidneyExchangeMechanisms (Cycle Formulation)
//
//  Created by Sameer Desai on 17/02/19.
//  Copyright © 2019 Sameer Desai. All rights reserved.
//

#ifndef JohnsonCycleFinder_hpp
#define JohnsonCycleFinder_hpp
#include"TarjanSCC.hpp"

using namespace std;



class cycleFinder
{
    set<unsigned int> blockedSet;
    map<unsigned int,set<unsigned int>> blockedMap;
    deque<unsigned int> stack;
    list<list<unsigned int>> allCycles;
    vector<list<unsigned int>> GraphAdj;
    unsigned int nodes;
    
public:
    
    cycleFinder(unsigned int nodes, vector<list<unsigned int>> Adj); /* Constructor to initialize the number of nodes in the graph, Adjacency List*/
    list<list<unsigned int>> simpleCycles(); /* Function to compute all cycles and return them as a list of lists*/
    vector<unsigned int> leastIndexSCC(list<set<unsigned int>> sccs, vector<list<unsigned int>> AdjSubGraph); /* Fetches the least index from all the Strongly Connected Components*/
    void unblock(unsigned int u); /* Unblocks given vertex*/
    bool findCyclesinSCG(unsigned int startVertex, unsigned int currentVertex); /*Finds cycles in the given SCG*/
    set<unsigned int> getBSet(unsigned int v); /* Returns the blocked set for given vertex */
    vector<list<unsigned int>> createSubGraph(long startVertex, vector<list<unsigned int>> AdjGraph); /* Returns the subgraph containing vertices >=startVertex*/
    
};


#endif /* JohnsonCycleFinder_hpp */
