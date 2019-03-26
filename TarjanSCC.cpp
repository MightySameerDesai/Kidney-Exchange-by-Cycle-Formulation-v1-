//
//  TarjanSCC.cpp
//  KidneyExchangeMechanisms (Cycle Formulation)
//
//  Created by Sameer Desai on 19/02/19.
//  Copyright © 2019 Sameer Desai. All rights reserved.
//

#include "TarjanSCC.hpp"

/* Constructor to initialize the Adjacency list and number of nodes */
TarjanSCC::TarjanSCC()
{
    time = 0;
}

/* Clears all data structures for re-run */
void TarjanSCC::TarjanRefresh()
{
    result.clear();
    visited.clear();
    visitedTime.clear();
    lowTime.clear();
    onStack.clear();
    myStack.clear();
    time = 0;
    AdjList.clear();
}

/* Method returns a list of SCCs with each SCC's vertices in a separate set*/
list<set<unsigned int>> TarjanSCC::SCC(unsigned int nodes, vector<list<unsigned int>> Adj)
{
    
    TarjanRefresh();
    this->AdjList = Adj;
    this->nodes = nodes;
    /* For each of the unvisited vertices call SCCUtil */
    for(unsigned int vertex = 0; vertex < nodes; vertex++)
    {
        if(visited.find(vertex)!=visited.end())
            continue;
        SCCUtil(vertex);
        
    }
    return result;
}

/* Utility funtion to get all SCCs */
void TarjanSCC::SCCUtil(unsigned int vertex)
{
    visited.insert(vertex); /* Mark current vertex as visited*/
    visitedTime.insert(make_pair(vertex, time)); /* Numbers the vertex in the order of visit */
    lowTime.insert(make_pair(vertex, time)); /* Intially lowTime is same as visitedTime*/
    time++;  /* Increment time */
    myStack.push_front(vertex); /* Push vertex to the front of deque*/
    onStack.insert(vertex);  /* Mark vertex as being ON stack*/
    if(AdjList.size() <= vertex)
        return;
    /* for every neighbouring vertex of current, */
    for(unsigned int vchild : AdjList[vertex])
    {
        /* If it was not visited, then recurse on it and update lowTime after*/
        if(!(visited.find(vchild)!= visited.end()))
        {
            SCCUtil(vchild);
            lowTime[vertex] = min(lowTime[vertex], lowTime[vchild]);
        }
        /* Else if it is already on current stack, update it lowTime value*/
        else if(onStack.find(vchild)!= onStack.end())
        {
            lowTime[vertex] = min(lowTime[vertex], visitedTime[vchild]);
        }
        
    }
    /* If reached the start vertex, empty stack contents and group them together as SCC */
    if(visitedTime[vertex] == lowTime[vertex])
    {
        set<unsigned int> scc;
        unsigned int v;
        do{
            v = myStack.front();
            myStack.pop_front();
            onStack.erase(v);
            scc.insert(v);
        }while(vertex != v);
        result.push_back(scc);
    }
    
}
