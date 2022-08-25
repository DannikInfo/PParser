#ifndef PPARSER_BUILDING_H
#define PPARSER_BUILDING_H

#include <iostream>

class building {
private:
    int id;
    std::string name;
    long lastModify;

public:
    building(){}

    building(int id, std::string name, long lastModify){
        this->id = id;
        this->name = name;
        this->lastModify = lastModify;
    }

    void setID(int id);
    void setName(std::string name);
    void setLastModify(long timeStamp);

    int getID();
    std::string getName();
    long getLastModify();

    std::string dbQuery(int type);

};


#endif //PPARSER_BUILDING_H
