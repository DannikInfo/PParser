#ifndef PPARSER_PROJECT_H
#define PPARSER_PROJECT_H

#include <iostream>
#include <list>
#include <vector>
#include "json.hpp"

class project {
private:
    int id;
    std::string name;
    std::string nameNoOff;
    int commissioning;
    int modify;
    int manufacturerID;
    int workshopID;
    int buildingID;
    std::string dir = "";
    std::string note = "";
    std::vector <std::string> persons;
    std::list <int> subPrjs;
    std::vector <std::string> subPrjsNames;
    long lastModify;

public:
    project(){}

    project(int id, std::string name, std::string nameNoOff, int commissioning, int modify, int manufacturerID, int workshopID, int buildingID, std::string dir, std::string note, const std::string& personFName, int subPrj, long lastModify){
        this->id = id;
        this->name = name;
        this->nameNoOff = nameNoOff;
        this->commissioning = commissioning;
        this->modify = modify;
        this->manufacturerID = manufacturerID;
        this->workshopID = workshopID;
        this->buildingID = buildingID;
        this->dir = dir;
        this->note = note;

        if(!personFName.empty()){
            this->addPerson(personFName);
        }
        if(subPrj > 0)
            this->addSubPrj(subPrj);

        this->lastModify = lastModify;
    }

    project(int id, std::string name, std::string nameNoOff, int commissioning, int modify, int manufacturerID, int workshopID, int buildingID, std::string dir, std::string note, std::vector<std::string> persons, std::list<int> subPrjs, long lastModify){
        this->id = id;
        this->name = name;
        this->nameNoOff = nameNoOff;
        this->commissioning = commissioning;
        this->modify = modify;
        this->manufacturerID = manufacturerID;
        this->workshopID = workshopID;
        this->buildingID = buildingID;
        this->dir = dir;
        this->note = note;
        this->persons = persons;
        this->subPrjs = subPrjs;
        this->lastModify = lastModify;
    }

    void setID(int id);
    void setName(std::string name);
    void setNameNoOff(std::string nameNoOff);
    void setCommissioning(int comm);
    void setModify(int modify);
    void setManufacturerID(int manID);
    void setWorkshopID(int workshopID);
    void setBuildingID(int buildingID);
    void setDir(std::string dir);
    void setNote(std::string note);
    void setPersons(std::vector<std::string> persons);
    void setSubPrjs(std::list<int> subPrjs);
    void setSubPrjsNames(std::vector<std::string> subPrjsNames);
    void setLastModify(long timeStamp);

    void addPerson(std::string fullName);
    void removePerson(std::string fullName);

    void addSubPrj(int id);
    void removeSubPrj(int id);

    void addSubPrjName(std::string name);
    void removeSubPrjName(std::string name);

    int getID();
    std::string getName();
    std::string getNameNoOff();
    int getCommissioning();
    int getModify();
    int getManufacturerID();
    int getWorkshopID();
    int getBuildingID();
    std::string getDir();
    std::string getNote();
    std::vector<std::string> getPersons();
    std::list<int> getSubPrjs();
    std::vector<std::string> getSubPrjsNames();
    long getLastModify();

    std::string dbQuery(int type);
};


#endif //PPARSER_PROJECT_H
