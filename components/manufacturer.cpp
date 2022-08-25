#include "manufacturer.h"

//Setters
void manufacturer::setID(int id){
    this->id = id;
}

void manufacturer::setName(std::string name){
    this->name = name;
}

void manufacturer::setCountry(std::string country){
    this->country = country;
}

void manufacturer::setLastModify(long timeStamp){
    this->lastModify = timeStamp;
}

//Getters
int manufacturer::getID(){
    return id;
}

std::string manufacturer::getName(){
    return name;
}

std::string manufacturer::getCountry(){
    return country;
}

long manufacturer::getLastModify(){
    return lastModify;
}

//Data
std::string manufacturer::dbQuery(int type){
    return "";//generating SQL update or insert query
}