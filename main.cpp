//
//  main.cpp
//  KidneyExchangeMechanisms
//
//  Created by Sameer Desai on 30/07/18.
//  Copyright © 2018 Sameer Desai. All rights reserved.
//  

#include <ilcplex/ilocplex.h>     /* Library for CPLEX */
#include "JohnsonCycleFinder.hpp" /* Library to enumerate all cycles in a given directed graph */
#include "jsonHandler.hpp"         /* Library to read and manipulate JSON data */
#include <chrono>                   /* Library for tracking execution time */
using namespace std::chrono;

ILOSTLBEGIN

typedef IloArray<IloIntArray> IntMatrix; /* 2D Matrix to store Weight-Matrix*/
typedef IloArray<IloNumVarArray> NumVarMatrix; /* 2D Matrix of Decision Variables */


class LpSolver
{
private:
    string json_filename; /* JSON filename specified by user */
    string dat_filename;  /* DAT filename specified by user */
    string default_filename_json; /* Default JSON filename */
    string default_filename_dat;  /* Default DAT filename */
    
public:
    unsigned int nodes;   /* Number of P-D Pairs / Vertices in the Compatibility Graph */
    unsigned int pLength; /* Disallow paths of length greater than or equal to */
    vector<vector<unsigned int>> Adj_Matrix; /* Stores the Adj-Matrix of given instance */
    vector<list<unsigned int>> AdjacencyList; /* Stores the Adj-List of given instance */
    vector<double> Res_Array; /* Stores the result of LP Solver i.e. which cycles were chosen */
    vector<vector<unsigned int>> AllValidCycles;  /* Stores all cycles of length atmost L */
    list<list<unsigned int>> Cycles;        /* Stores all cycles found in given instance */
    vector<vector<unsigned int>> PermittedCycles; /* Stores cycles part of the OPTIMAL SOLUTION to given instance */
    jsonData *jd;                                           /* Object of JsonData Class*/
    map<unsigned int, unsigned int> myPairs;   /* Map to store the matched pairs as in solution */
    
    //map<unsigned int, unsigned int> myPairs;   /* Map to store the matched pairs as in solution */
    int pairsIncluded = 0;
    
    high_resolution_clock::time_point start;
    
    
    /* Initializes the default parameters */
    LpSolver()
    {
        this->default_filename_dat = "/Users/sameerdesai/Documents/KidneyExchangeMechanisms/KidneyExchangeMechanisms/kidneyexample.dat";
        this->default_filename_json = "/Users/sameerdesai/Documents/KE2.json";
        this->pLength = 3;
    }
    
    /* Sets the JSON filename */
    void setJsonFilename(string filename)
    {
        json_filename = filename;
        jd = new jsonData(json_filename);
    }
    
    /* Sets the DAT filename */
    void setDatFilename(string filename)
    {
        dat_filename = filename;
    }
    
    /* Sets the path length specified */
    void setPathLength(unsigned int len)
    {
        pLength = len;
    }
    
    /* Read JSON file data and populates all cycles in the given graph*/
    void readJsonFile()
    {
        jd->mapAllPairs(); /* Invokes the mapper from JSON class */
        jd->buildCompatibilityGraph(); /* Invokes the graph builder */
        
        nodes = (int)jd->Compatibility_Graph.size();
        Adj_Matrix.resize(nodes,vector<unsigned int>(nodes,0));
        //Res_Matrix.resize(nodes,vector<double>(nodes,0.0));
        AdjacencyList.resize(nodes,list<unsigned int>());
        Adj_Matrix = jd->Compatibility_Graph;
        for(unsigned int i = 0; i < Adj_Matrix.size(); i++)
        {
            for(unsigned int j = 0; j < Adj_Matrix[i].size(); j++)
            {
                if(Adj_Matrix[i][j])
                {
                    AdjacencyList[i].push_back(j);
                }
            }
        }
        
        cycleFinder C(nodes,AdjacencyList);
        Cycles = C.simpleCycles();
    }
    
    /* Read in the Weight/Adjacency Matrix from a DAT file directly */
    void readDatFile()
    {
        ifstream f(dat_filename,ios::in);
        if(!f)
        {
            cerr<<"File doesnot exist by the name of:"<<dat_filename<<endl;
            throw(1);
        }
        f>> nodes;
        Adj_Matrix.resize(nodes,vector<unsigned int>(nodes,0));
       // Res_Matrix.resize(nodes,vector<double>(nodes,0.0));
        for(unsigned int i = 0; i < nodes; i++)
        {
            for(unsigned int j = 0; j < nodes; j++)
            {
                f>>Adj_Matrix[i][j];
            }
        }

    }

    
    /* Populate all Cycles Needed i.e. Cycles of size ATMOST 'L' */
    void populateAllValidCycles()
    {
        
        for(auto i = Cycles.begin(); i!= Cycles.end(); i++)
        {
            
            list<unsigned int> cycle = *i;
            if(cycle.size() > 4)
            {
                continue;
            }
            vector<unsigned int> recycle(cycle.begin(),cycle.end());
            AllValidCycles.push_back(recycle);
        }
    }
    
    

    /* Builds a Decision Variable name with given integer IDs */
    char* createDVName(unsigned int id1)
    {
        string idx = to_string(id1);
        //string idy = to_string(id2);
        string numericID = "s_" + idx;
        unsigned long length = numericID.size();
        char *dvName = new char[length+1];
        strcpy(dvName, numericID.c_str());
        return dvName;
    }
    
    /* Builds a constraint name with given string identifier and integer ID */
    char* createConstraintName(string constraintText, unsigned int id )
    {
        string idx = to_string(id);
        string constraintID = constraintText + idx;
        unsigned long length = constraintID.size();
        char *constraintName = new char[length+1];
        strcpy(constraintName, constraintID.c_str());
        return constraintName;
    }

    /* Map all cplex-matched donors and patients */
    void setMatchedPairs()
    {
        for(unsigned int i = 0; i < PermittedCycles.size(); i++)
        {
            for(unsigned int j = 0; j < PermittedCycles[i].size()-1; j++)
            {
                unsigned int curr = PermittedCycles[i][j];
                unsigned int next = PermittedCycles[i][j+1];
                myPairs[curr] = next;
            }
        }
    }


    /* Prints the cycles involved in the given data structure */
    void printCycles(vector<vector<unsigned int>>&P)
    {
        for(unsigned int i = 0; i < P.size(); i++)
        {
            for(unsigned int j = 0; j < P[i].size(); j++)
            {
                cout<<P[i][j]<<"->";
            }
            cout<<endl;
        }
    }

    
    
    /* Prints the matched pairs to JSON file and logfile */
    void printMatches()
    {
        for(auto i = myPairs.begin(); i != myPairs.end(); i++)
        {
            cout<<"Patient "<<jd->getKeyID(i->first)<<" gets Kidney from "<<jd->getKeyID(i->second)<<endl;
        }
        jd->printToJson(myPairs,json_filename);
    }
    

    
    void interactor()
    {
        string pathName;
        unsigned int len;
        cout<<"Welcome to Kidney Exchange Solver"<<endl;
        cout<<"Enter the pathname for your JSON file:"<<endl;
        cin>>pathName;
        cout<<"Enter the value of L for path length constraint:";
        cin>>len;
        setPathLength(len);
        setJsonFilename(pathName);
        start = high_resolution_clock::now();
        readJsonFile();
    }
    
};


int main(int argc, const char * argv[])
{
    IloEnv env;  /* Construct the CPLEX environment */
    try
    {
        LpSolver solver;
        solver.interactor(); /* Interact with user */
        
        /* Initialise the CPLEX variables and read in the Weight/Adjacency Matrix*/
        IntMatrix weights(env,solver.nodes);
        for(unsigned int i = 0; i < solver.nodes; i++)
        {
            weights[i] = IloIntArray(env,solver.nodes);
        }
        
        for(unsigned int i = 0; i < solver.nodes; i++)
        {
            for(unsigned int j = 0; j < solver.nodes; j++)
            {
                weights[i][j] = solver.Adj_Matrix[i][j];
            }
        }
        
        solver.populateAllValidCycles();
        /* Array to hold the final ILP solution */
        solver.Res_Array.resize(solver.AllValidCycles.size(),0.0);
        //cout<<solver.Res_Array.size();

        
        /* 2D Matrix: Rows: All vertices, Columns: All Cycles C1,C2,...*/
        IntMatrix cyclePresence(env,solver.nodes);
        for(unsigned int i = 0; i < solver.nodes; i++)
        {
            cyclePresence[i] = IloIntArray(env,solver.AllValidCycles.size());
        }
        
        
        for(unsigned int i = 0; i < solver.nodes; i++)
        {
            for(unsigned int j = 0; j < solver.AllValidCycles.size(); j++)
            {
                cyclePresence[i][j] = 0;
            }
        }
        
        /* cyclePresence[i][j] = 1 iff vertex v_i is part of cycle C_j*/
        for(unsigned int i = 0; i < solver.AllValidCycles.size(); i++)
        {
            for(unsigned int j = 0; j < solver.AllValidCycles[i].size()-1; j++)
            {
                unsigned int row = solver.AllValidCycles[i][j];
                cyclePresence[row][i] = 1;
            }
        }
        auto mid = high_resolution_clock::now();
        auto mid_duration = duration_cast<milliseconds>(mid - solver.start);
        //cout<<"Time Elapsed in Pre-Computation:"<<(double)mid_duration.count()/1000 << " seconds" << endl;
        //solver.printCycles(solver.AllValidCycles);
    
        /* Define a complete optimization model that can later be extracted to a IloCplex object */
        IloModel model(env);

        /* Adding Constraint 0: For all cycles c in C , where C is the set of all cycles, c can take value either 0 or 1
           (aka) Integrality Constraint */
        IloNumVarArray s(env,solver.AllValidCycles.size(),0,1,ILOBOOL);
        
        /* Naming all the decision variables */
        for (unsigned int i=0; i<solver.AllValidCycles.size(); i++)
        {
            //s[i] = 0;
            char* name = solver.createDVName(i);
            s[i].setName(name);
        }
        
        
       

        /* For all vertices   v in V, k: Sigma(1 : v_i is in c), for all c in C */
        /* Adding Constraint 1: Sigma(k) <= 1       (aka) Cycle Disjointness Constraint.*/
       
        for(unsigned int i = 0; i < solver.nodes; i++)
        {
            IloNumVarArray vSum(env);
            for(unsigned int j = 0; j < solver.AllValidCycles.size(); j++)
            {
                if(cyclePresence[i][j] == 1)
                {
                    vSum.add(s[j]);
                }
            }
            IloConstraint vertexConstraint(IloSum(vSum) <=  1);
            char* constraintName1 = solver.createConstraintName("CycleDisjointness_",i);
            vertexConstraint.setName(constraintName1);
            model.add(vertexConstraint);
        }
        
        /* C is the set of all cycles,
           Create and Add the Objective function:
           MAXIMIZE : For All cycles c in C, Sigma (cycle_size * S_c)
           where S_c = 0 if cycle c is not part of final solution and 1 otherwise */
        
        IloExpr obj(env);
        for(IloInt i = 0; i < solver.AllValidCycles.size(); i++)
        {
            IloInt sizec = solver.AllValidCycles[i].size() - 1;
            obj+= sizec * s[i];
        }
        
        IloNumArray sol(env,solver.AllValidCycles.size());
        
        /* Adding the Objective Function */
        IloObjective MaxCovers = IloMaximize(env, obj);
        MaxCovers.setName("Max_Cycle_Covers_Objective");
        model.add(MaxCovers);
        obj.end();
        
        IloCplex cplex(env); /* Create a CPLEX object for solving th model */
        
        cplex.setName("KidneyExchangeByCycleCovers");
        cplex.extract(model); /* Extract the model created into CPLEX object */
        cplex.exportModel ("/Users/sameerdesai/Downloads/MTP\ Project\ Workspace/KidneyExchangeMechanisms\ (Cycle\ Formulation)/KidneyExchangeMechanisms\ (Cycle\ Formulation)/KEbyCycleFormulation.lp");
        /* Export the OPtimization model created to a file */
        
        /* Keep track of time elapsed in solving the LP */
        IloTimer clock(env);
        clock.start();
        /* Solve the model */
        if (cplex.solve()) {
            cout << "Solution status: " << cplex.getStatus() << endl;
            cout << "Number of Exchanges(Optimal Value) = " << cplex.getObjValue() << endl;
            //cout << "Solve Time    = "<<clock.getTime()<<" seconds"<<endl;
            clock.stop();
            cplex.getValues(s,sol);
            for(int i = 0; i < solver.AllValidCycles.size(); i++)
            {
                //cout<<sol[i]<<" ";
                solver.Res_Array[i] = sol[i];
                if(sol[i])
                {
                    solver.PermittedCycles.push_back(solver.AllValidCycles[i]);
                    //solver.printCycles(solver.PermittedCycles);
                }
            }
            
        }
        
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - mid);
        cout<<"Time Elapsed in Pre-Computation:"<<(double)mid_duration.count()/1000 << " seconds" << endl;
        cout<<"Time Elapsed in LP Solving     :"<< (double)duration.count()/1000 << " seconds" << endl;
        //solver.setMatchedPairs(); /* Invokes the function to Map all matched patients and donors */
        //solver.printMatches();    /* Invokes the function to print all matched pairs */
        //solver.printCycles();     /* Print all cycles involved */
        //solver.printResult();
    }
    catch(IloException &e)
    {
        cerr<<"Error:"<<e<<endl;
    }
    catch(...)
    {
        cerr<<"Encountered Error"<<endl;
    }
    env.end();
    return 0;
}

