//
//  jsonReader.cpp
//  KidneyExchangeMechanisms
//
//  Created by Sameer Desai on 24/08/18.
//  Copyright © 2018 Sameer Desai. All rights reserved.
//

#include "jsonHandler.hpp"

/* Read all the JSON file data into a JSON Value Object */
jsonData::jsonData(string filename)
{
        this->filename = filename;
        ifstream ifs(filename,std::ifstream::binary);
        ifs >> Compatibilities;
        ifs.close();
}

/* Returns the original ID of a patient/donor given the program generated ID as parameter*/
unsigned int jsonData::getKeyID(unsigned int pid)
{
    auto it = revMap.find(pid);
    return it->second;
}
/* Returns the program generated ID of a patient/donor given the original ID as parameter*/
unsigned int jsonData::getID(unsigned int pid)
{
       return myMap[pid];
}

/* Prints a 2D Vector passed as a parameter */
void jsonData::print2DVector(vector<vector<unsigned int>>&myVector)
{
        for(unsigned int i = 0; i < myVector.size(); i++)
        {
            for(unsigned int j = 0; j < myVector[i].size(); j++)
            {
                cout<<myVector[i][j]<<" ";
            }
            cout<<endl;
        }
}

/* myMap  -> Key: Source ID as in JSON  || Value: Index assigned by me strting from 0. */
/* revMap -> Key: Index assigned by me  || Value: Source ID as in JSON. */
void jsonData::mapAllPairs()
{
        unsigned int id_seed = 0;
    
        unsigned int n = Compatibilities["data"].size();
        Compatibility_Graph.resize(n);
        for(unsigned int i = 0; i < n; i++)
        {
            Compatibility_Graph[i].resize(n,0);
        }
    
        for (Json::Value::iterator it = Compatibilities["data"].begin(); it != Compatibilities["data"].end(); ++it)
        {
            
            auto itr = (*it)["sources"].begin();
            unsigned int source = (*itr).asInt();
            /* Map every patient to an unique ID generated incrementally */
            myMap[source] = id_seed;
            /* Map every unique ID generated incrementally to original patient ID */
            revMap[id_seed] = source;
            id_seed++;
            /* Log the mapping done */
            cout<<source<<"mapped to"<<id_seed-1<<endl;
        }
}

/* Populates the Compatibility Graph from JSON data read */
void jsonData::buildCompatibilityGraph()
{
    
        for (Json::Value::iterator it = Compatibilities["data"].begin(); it != Compatibilities["data"].end(); ++it)
        {
            
            auto it_source = (*it)["sources"].begin();  // Read PID
            unsigned int source = (*it_source).asInt();
            unsigned int sourceID = getID(source);      // Get its UID
            
            auto it_recipients = (*it)["matches"].begin();
            for(    ;   it_recipients !=(*it)["matches"].end()  ;   it_recipients++)
            {
                
                unsigned int recipient = (*it_recipients)["recipient"].asInt(); // Read PID
                unsigned int recipientID = getID(recipient); // Get its UID
                Compatibility_Graph[recipientID][sourceID] = 1;
            }
        }
        
}
/* Prints the matched pairs to a JSON file */
void jsonData::printToJson(map<unsigned int, unsigned int>&myPairs, string file_name)
{
    Json::Value root;
    int i = 0;
    for(auto it = myPairs.begin(); it != myPairs.end() ; it++)
    {
        string id = to_string(i);
        root[id]["Patient"] = getKeyID(it->first);
        root[id]["Donor"] = getKeyID(it->second);
        i++;
    }
    
//
//    boost::filesystem::path filePath(file_name);
//    string fileID;
//    if(filePath.has_stem())
//    {
//        fileID = filePath.stem().string();
//    }
//    string folderID = filePath.parent_path().string();
//    boost::filesystem::path dir(folderID);
//    if(!(boost::filesystem::exists(dir)))
//    {
//        cout<<"Doesn't Exists"<<endl;
//        if (boost::filesystem::create_directory(dir))
//            cout << "....Successfully Created !" << endl;
//    }
    //string folderID = "";
    string fileID = filename;
    //int size = fileID.size();
    string filer(fileID.begin(),fileID.end()-5);
    ofstream file;
    string resultFileName = filer  + "Results.json";
    file.open(resultFileName,ios::out);
    file << root;
    file.close();
}
    
//void jsonData::printExchangesToJson(vector<vector<unsigned int>>&cycles,unsigned int pairsIncluded)
//{
//
//}



