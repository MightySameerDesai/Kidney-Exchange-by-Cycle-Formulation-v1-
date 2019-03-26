//
//  TarjanSCC.hpp
//  KidneyExchangeMechanisms (Cycle Formulation)
//  Finds Strongly Connected Components in the given graph.
//  Created by Sameer Desai on 19/02/19.
//  Copyright © 2019 Sameer Desai. All rights reserved.
//

#ifndef TarjanSCC_hpp
#define TarjanSCC_hpp

#include<iostream>
#include<map>
#include<set>
#include<stack>
#include<vector>
#include<list>
#include<deque>

using namespace std;


class TarjanSCC
{
private:
    
    map<unsigned int, unsigned int> visitedTime;
    map<unsigned int, unsigned int> lowTime;
    set<unsigned int> onStack;
    deque<unsigned int> myStack;
    set<unsigned int> visited;
    list<set<unsigned int> > result;
    unsigned int time;
    vector<list<unsigned int> > AdjList;
    unsigned int nodes;
    
public:
    
    TarjanSCC();  /* Constructor to initialize the Adjacency list and number of nodes */
    void TarjanRefresh(); /* Clears all data structures for re-run */
    list<set<unsigned int> > SCC(unsigned int nodes, vector<list<unsigned int>> Adj); /* Method returns a list of SCCs with each SCC's vertices in a separate                set*/
    void SCCUtil(unsigned int vertex); /* Utility funtion to get all SCCs */
    
};
#endif /* TarjanSCC_hpp */
