#include "workshop.h"

//Setters
void workshop::setID(int id){
    this->id = id;
}

void workshop::setName(std::string name){
    this->name = name;
}

void workshop::setBuildings(std::list<int> buildings){
    this->buildings = buildings;
}

void workshop::setLastModify(long timeStamp){
    this->lastModify = timeStamp;
}

//Getters
int workshop::getID(){
    return id;
}

std::string workshop::getName(){
    return name;
}

std::list<int> workshop::getBuildings(){
    return buildings;
}

long workshop::getLastModify(){
    return lastModify;
}

//DATA
std::string workshop::dbQuery(int type){
    return "";
}

//List helpers
void workshop::addBuilding(int id){
    this->buildings.push_back(id);
    this->buildings.unique();
}
void workshop::removeBuilding(int id){
    this->buildings.remove(id);
}