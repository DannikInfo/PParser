#include "project.h"

//Setters
void project::setID(int id){
    this->id = id;
}

void project::setName(std::string name){
    this->name = name;
}

void project::setNameNoOff(std::string nameNoOff){
    this->nameNoOff = nameNoOff;
}

void project::setCommissioning(int comm){
    this->commissioning = comm;
}

void project::setModify(int modify){
    this->modify = modify;
}

void project::setManufacturerID(int manID){
    this->manufacturerID = manID;
}

void project::setWorkshopID(int workshopID){
    this->workshopID = workshopID;
}

void project::setBuildingID(int buildingID){
    this->buildingID = buildingID;
}

void project::setDir(std::string dir){
    this->dir = dir;
}

void project::setNote(std::string note){
    this->note = note;
}

void project::setPersons(std::vector<std::string> persons){
    this->persons = persons;
}

void project::setSubPrjs(std::list<int> subPrjs){
    this->subPrjs = subPrjs;
}

void project::setSubPrjsNames(std::vector<std::string> subPrjsNames){
    this->subPrjsNames = subPrjsNames;
}

void project::setLastModify(long timeStamp){
    this->lastModify = timeStamp;
}


//Getters
int project::getID(){
    return id;
}

std::string project::getName(){
    return name;
}

std::string project::getNameNoOff(){
    return nameNoOff;
}

int project::getCommissioning(){
    return commissioning;
}

int project::getModify(){
    return modify;
}

int project::getManufacturerID(){
    return manufacturerID;
}

int project::getWorkshopID(){
    return workshopID;
}

int project::getBuildingID(){
    return buildingID;
}

std::string project::getDir(){
    return dir;
}

std::string project::getNote(){
    return note;
}

std::vector<std::string> project::getPersons(){
    return persons;
}

std::list<int> project::getSubPrjs(){
    return subPrjs;
}

std::vector<std::string> project::getSubPrjsNames(){
    return subPrjsNames;
}

long project::getLastModify(){
    return lastModify;
}


//DATA
std::string project::dbQuery(int type){
    return "";
}


//List helpers
void project::addPerson(std::string fullName){
    auto findP = std::find(persons.begin(), persons.end(), fullName);
    if(findP == persons.end())
        persons.push_back(fullName);
}

void project::removePerson(std::string fullName){
    auto findP = std::find(persons.begin(), persons.end(), fullName);
    if(findP != persons.end())
        this->persons.erase(findP);
}

void project::addSubPrj(int id){
    auto findP = std::find(subPrjs.begin(), subPrjs.end(), id);
    if(findP == subPrjs.end())
        subPrjs.push_back(id);
}

void project::removeSubPrj(int id){
    this->subPrjs.remove(id);
}


void project::addSubPrjName(std::string name){
    auto findP = std::find(subPrjsNames.begin(), subPrjsNames.end(), name);
    if(findP == subPrjsNames.end())
        subPrjsNames.push_back(name);
}

void project::removeSubPrjName(std::string name){
    auto findP = std::find(subPrjsNames.begin(), subPrjsNames.end(), name);
    if(findP != subPrjsNames.end())
        this->subPrjsNames.erase(findP);
}