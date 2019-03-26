//
//  TarjanCycleFinder.hpp
//  KidneyExchangeMechanisms (Cycle Formulation)
//
//  Created by Sameer Desai on 20/03/19.
//  Copyright © 2019 Sameer Desai. All rights reserved.
//

#ifndef TarjanCycleFinder_hpp
#define TarjanCycleFinder_hpp

#include <stdio.h>

#endif /* TarjanCycleFinder_hpp */

class AllCyclesTarjan
{
private:
    
    set<unsigned int> visited;
    deque<unsigned int> pointStack;
    deque<unsigned int> markedStack;
    set<unsigned int> markedSet;
    
public:
    AllCyclesTarjan()
    {
        reset();
    }
    
    void reset()
    {
        visited.clear();
        pointStack.clear();
        markedSet.clear();
        markedStack.clear();
    }
  
    list<list<unsigned int>> findAllSimpleCycles(vector<list<unsigned int>> Adj)
    {
        reset();
        list<list<unsigned int>> result;
        for(unsigned int i = 0; i < nodes; i++)
        {
            findAllCycles(i,i,result);
            visited.insert(i);
            while(markedStack.size())
            {
                markedSet.erase(markedStack.front());
                markedStack.pop_front();
            }
        }
        return result;
    }
    
    bool findAllCycles(unsigned int start, unsigned int current, list<list<unsigned int>> result)
    {
        bool hasCycle = false;
        pointStack.push_front(current);
        markedSet.insert(current);
        markedStack.push_front(current);
    }
    
    for(unsigned int vertex = 0; vertex < nodes; vertex++)
    {
        if(visited.find(vertex)!= visited.end())
        {
            continue;
        }
        else if(vertex == start)
        {
            hasCycle = true;
            pointStack.push_front(vertex);
            list<unsigned int> cycle;
            auto itr = pointStack.rbegin();
            while(itr)
            {
                cycle.push_back(*itr);
                itr++;
            }
            pointStack.pop_front();
            result.push_back(cycle);
        }
        else if(markedSet.find(vertex) == markedSet.end())
        {
            hasCycle = findAllCycles(start,vertex,result) || hasCycle;
        }
    }
    
};


    
    public static void main(String args[]) {
        Graph<Integer> graph = new Graph<>(true);
        graph.addEdge(0, 1);
        graph.addEdge(1, 4);
        graph.addEdge(1, 7);
        graph.addEdge(1, 6);
        graph.addEdge(4, 2);
        graph.addEdge(4, 3);
        graph.addEdge(2, 4);
        graph.addEdge(2, 7);
        graph.addEdge(2, 6);
        graph.addEdge(7, 8);
        graph.addEdge(7, 5);
        graph.addEdge(5, 2);
        graph.addEdge(5, 3);
        graph.addEdge(3, 7);
        graph.addEdge(3, 6);
        graph.addEdge(3, 4);
        graph.addEdge(6, 5);
        graph.addEdge(6, 8);
        
        AllCyclesInDirectedGraphTarjan tarjan = new AllCyclesInDirectedGraphTarjan();
        List<List<Vertex<Integer>>> result = tarjan.findAllSimpleCycles(graph);
        result.forEach(cycle -> {
            cycle.forEach(v -> System.out.print(v.getId() + " "));
            System.out.println();
        });
    }
}
