//
//  jsonReader.hpp
//  KidneyExchangeMechanisms
//
//  Created by Sameer Desai on 24/08/18.
//  Copyright © 2018 Sameer Desai. All rights reserved.
//

#ifndef jsonHandler_hpp
#define jsonHandler_hpp

#include <iostream>
#include <fstream>
#include <json/json.h>
//#include <boost/filesystem.hpp>
using namespace std;

class jsonData
{
    
private:
    /* myMap  -> Key: Source ID as in JSON  || Value: Index assigned by me strting from 0. */
    map<unsigned int,unsigned int> myMap;
    /* revMap -> Key: Index assigned by me  || Value: Source ID as in JSON. */
    map<unsigned int,unsigned int> revMap;
    /* JSON filename */
    string filename;
    /* JSON Value Object to read in all the JSON file data */
    Json::Value Compatibilities;
    
    
public:
    vector<vector<unsigned int>> Compatibility_Graph;
    jsonData(string filename);
    unsigned int getID(unsigned int pid);
    void print2DVector(vector<vector<unsigned int>>&myVector);
    void mapAllPairs();
    void buildCompatibilityGraph();
    void printToJson(map<unsigned int,unsigned int>&myPairs,string file_name);
    unsigned int getKeyID(unsigned int pid);
    //void printExchangesToJson(vector<vector<unsigned int>>&cycles,unsigned int pairsIncluded)
};


#endif /* jsonReader_hpp */
