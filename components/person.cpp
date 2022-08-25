#include "person.h"

//Setters
void person::setID(int id){
    this->id = id;
}

void person::setFullName(std::string fullName){
    this->fullName = fullName;
}

void person::setPhone(std::string phone){
    this->phone = phone;
}

void person::setEmail(std::string email){
    this->email = email;
}

void person::setLastModify(long timeStamp){
    this->lastModify = timeStamp;
}

//Getters
int person::getID(){
    return id;
}

std::string person::getFullName(){
    return fullName;
}

std::string person::getPhone(){
    return phone;
}

std::string person::getEmail(){
    return email;
}

long person::getLastModify(){
    return lastModify;
}

//DATA
std::string person::dbQuery(int type){
    return "";
}