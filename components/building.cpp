#include "building.h"

//Setters
void building::setID(int id){
    this->id = id;
}

void building::setName(std::string name){
    this->name = name;
}

void building::setLastModify(long timeStamp){
    lastModify = timeStamp;
}

//Getters
int building::getID(){
    return id;
}

std::string building::getName(){
    return name;
}

long building::getLastModify(){
    return lastModify;
}

//DATA
std::string building::dbQuery(int type){
    return "";
}
