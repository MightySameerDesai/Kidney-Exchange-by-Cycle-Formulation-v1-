//
//  JohnsonCycleFinder.cpp
//  KidneyExchangeMechanisms (Cycle Formulation)
//
//  Created by Sameer Desai on 17/02/19.
//  Copyright © 2019 Sameer Desai. All rights reserved.
//

#include "JohnsonCycleFinder.hpp"

/* Constructor to initialize the number of nodes in the graph, Adjacency List*/
cycleFinder::cycleFinder(unsigned int nodes, vector<list<unsigned int>> Adj)
{
    this->nodes = nodes;
    this->GraphAdj = Adj;
}




/* Function to compute all cycles and return them as a list of lists*/
list<list<unsigned int>> cycleFinder::simpleCycles()
{
    long startIndex  = 0;
    TarjanSCC tarjan;
    while(startIndex < nodes)
    {
        /* Creates a graph of all SCCs and returns the least indexed vertex among all SCCs*/
        vector<list<unsigned int>> subGraph = createSubGraph(startIndex, GraphAdj);
        list<set<unsigned int>> sccs = tarjan.SCC(nodes, subGraph);
        vector<unsigned int> maybeLeastVertex = leastIndexSCC(sccs, subGraph);
        if(maybeLeastVertex.size() > 0)
        {
            unsigned int leastVertex = maybeLeastVertex[0];
            blockedSet.clear();
            blockedMap.clear();
            /* Fetches all cycles starting from leastVertex in the graph */
            findCyclesinSCG(leastVertex,leastVertex);
            startIndex = leastVertex+1;
        }
        else
        {
            break;
        }
    }
    return allCycles;
}

/* Fetches the least index from all the Strongly Connected Components*/
vector<unsigned int> cycleFinder::leastIndexSCC(list<set<unsigned int>> sccs, vector<list<unsigned int>> AdjSubGraph)
{
    int min = INT_MAX;
    int minVertex = -1;
    set<unsigned int> minScc;
    /* returns the least indexed vertex among all SCCs */
    for(set<unsigned int> scc: sccs)
    {
        if(scc.size() == 1)
        {
            continue;
        }
        for(unsigned int vertex: scc)
        {
            if(vertex < min)
            {
                min = vertex;
                minVertex = vertex;
                minScc = scc;
            }
        }
    }
    /* If no minVertex found return */
    if(minVertex == -1)
    {
        return vector<unsigned int>();
    }
    
    /*For every edge in subgraphAdj, if both the vertices of the edge are in minscc set, then add that edge to graphscc. finally return least index in SCC*/
    set<unsigned int> allVertices;
    for(unsigned int i = 0; i < AdjSubGraph.size(); i++)
    {
        list<unsigned int> listi = AdjSubGraph[i];
        for(auto it = listi.begin(); it!= listi.end(); it++)
        {
            unsigned int curr = *it;
            if((minScc.find(i)!= minScc.end() ) && ( minScc.find(curr)!=minScc.end()))
            {
                allVertices.insert(i);
                allVertices.insert(curr);
            }
        }
    }
    
    //If minVertex does not exist in allVertices, return null set, else return minVertex
    vector<unsigned int> resultant;
    if(allVertices.find(minVertex)!= allVertices.end())
    {
        resultant.push_back(minVertex);
    }
    return resultant;
}



/* Unblocks given vertex*/
void cycleFinder::unblock(unsigned int u)
{
    blockedSet.erase(u);
    if(blockedMap.find(u)!= blockedMap.end())
    {
        set<unsigned int> v = blockedMap[u];
        for(auto i = v.begin(); i!= v.end(); i++)
        {
            unsigned int currv = *i;
            /* Recursively check and unblock as necessary */
            if(blockedSet.find(currv) != blockedSet.end())
            {
                unblock(currv);
            }
        }
        blockedMap.erase(u);
    }
    
}

/*Finds cycles in the given SCG*/
bool cycleFinder::findCyclesinSCG(unsigned int startVertex, unsigned int currentVertex)
{
    
    bool foundCycle = false;
    stack.push_back(currentVertex);
    blockedSet.insert(currentVertex);
    for(unsigned int neighbor : GraphAdj[currentVertex])
    {
        /* Cycle is found, so store the contents of stack as cycle */
        if(neighbor == startVertex)
        {
            
            stack.push_back(startVertex);
            list<unsigned int> cycle(stack.begin(), stack.end()); //check this
            cycle.reverse();
            stack.pop_back();
            allCycles.push_back(cycle);
            foundCycle = true;
        }/* If not blocked, explore this neighbor */
        else if(blockedSet.find(neighbor)== blockedSet.end())
        {
            bool gotCycle = findCyclesinSCG(startVertex, neighbor);
            foundCycle = foundCycle || gotCycle;
        }
    }
    /* Unblock this vertex and all vertices blocked due to it (recursively)*/
    if(foundCycle)
    {
        unblock(currentVertex);
    }
    else
    {
        /* If no cycle is found with current vertex then don't unblock it. Find all its neighbors and add this
        vertex to their blockedMap. If any of those neighbors ever get unblocked then unblock current vertex as well. */
        for(unsigned int neigh : GraphAdj[currentVertex])
        {
            set<unsigned int> bSet = getBSet(neigh);
            bSet.insert(currentVertex);
        }
    }
    stack.pop_back();
    return foundCycle;
}

/* Returns the blocked set for given vertex */
set<unsigned int> cycleFinder::getBSet(unsigned int v)
{
    set<unsigned int> res;
    if(blockedMap.find(v)!= blockedMap.end())
    {
        res = blockedMap[v];
    }
    //blockedMap
    return res;
}

/* Returns the subgraph containing vertices >=startVertex*/
vector<list<unsigned int>> cycleFinder::createSubGraph(long startVertex, vector<list<unsigned int>> AdjGraph)
{
    vector<list<unsigned int>> subGraph;
    for(int i = 0; i < AdjGraph.size(); i++)
    {
        list<unsigned int> currlist = AdjGraph[i];
        for(auto j = currlist.begin(); j!= currlist.end(); j++)
        {
            unsigned int currj = *j;
            if((i >= startVertex) && (currj >= startVertex))
            {
                if(subGraph.size() <= i)
                {
                    subGraph.resize(i+1,list<unsigned int>());
                    subGraph[i].push_back(currj);
                }
                else
                {
                    subGraph[i].push_back(currj);
                }
            }
        }
    }
    return subGraph;
}

