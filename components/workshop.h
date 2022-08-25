#ifndef PPARSER_WORKSHOP_H
#define PPARSER_WORKSHOP_H

#include <iostream>
#include <list>

class workshop {
private:
    int id;
    std::string name;
    std::list<int> buildings;
    long lastModify;


public:
    workshop(){}

    workshop(int id, std::string name, int building, long lastModify){
        this->id = id;
        this->name = name;
        this->lastModify = lastModify;
        this->addBuilding(building);
    }

    workshop(int id, std::string name, std::list<int> buildings, long lastModify){
        this->id = id;
        this->name = name;
        this->buildings = buildings;
        this->lastModify = lastModify;
    }

    void setID(int id);
    void setName(std::string name);
    void addBuilding(int id);
    void removeBuilding(int id);
    void setBuildings(std::list<int> buildings);
    void setLastModify(long timeStamp);

    int getID();
    std::string getName();
    std::list<int> getBuildings();
    long getLastModify();

    std::string dbQuery(int type);
};


#endif //PPARSER_WORKSHOP_H
