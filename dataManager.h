#ifndef PPARSER_DATAMANAGER_H
#define PPARSER_DATAMANAGER_H

#include <iostream>
#include <cstring>
#include <cstdio>
#include <filesystem>

#include "config.h"
#include "configPaths.h"
#include "logger.h"
#include "utils.h"

#include "components/project.h"
#include "components/person.h"
#include "components/building.h"
#include "components/workshop.h"
#include "components/manufacturer.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


class dataManager {
private:
    int counter = 0;
//FILES ORIGINAL
    std::map<int, project> projectsF;
    std::map<std::string, person> personsF;
    std::map<int, manufacturer> manufacturersF;
    std::map<int, workshop> workshopsF;
    std::map<int, building> buildingsF;

//DB ORIGINAL
    std::map<int, project> projectsDB;
    std::map<std::string, person> personsDB;
    std::map<int, manufacturer> manufacturersDB;
    std::map<int, workshop> workshopsDB;
    std::map<int, building> buildingsDB;

//TODO: data analyze and cross update
//FILES MODIFIED
//    std::map<int, project> projectsFM;
//    std::map<std::string, person> personsFM;
//    std::map<int, manufacturer> manufacturersFM;
//    std::map<int, workshop> workshopsFM;
//    std::map<int, building> buildingsFM;
//
////DB MODIFIED
//    std::map<int, project> projectsDBM;
//    std::map<std::string, person> personsDBM;
//    std::map<int, manufacturer> manufacturersDBM;
//    std::map<int, workshop> workshopsDBM;
//    std::map<int, building> buildingsDBM;


    std::vector<project> foundProjects;
    //std::vector<std::string> potentialPrjs;

    std::vector<std::string> markerDirs;
    std::vector<std::string> markerFiles;

    sql::Driver* driver;
    sql::ConnectOptionsMap conOpt;
public:
    dataManager();

    //searching data in directory
    void searchProjects(const std::string& root);

    //maybe delete
    bool dirExistsInDB(std::string path);

    //Data load
    void loadDataFromDB();
    void loadDataFromFile(std::string &path);

    //Data manipulation
    void foundUpload();
    void fixDirectories();
    void toFile(project prj);

    //handler for files
    void fileData(const std::string& path, int function);

    //Tasks
    void searchTask();
    void fixTask();
};


#endif //PPARSER_DATAMANAGER_H
