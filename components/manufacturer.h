#ifndef PPARSER_MANUFACTURER_H
#define PPARSER_MANUFACTURER_H

#include <iostream>

class manufacturer {
private:
    int id;
    std::string name;
    std::string country;
    long lastModify;

public:
    manufacturer(){}

    manufacturer(int id, std::string name, std::string country, long lastModify){
        this->id = id;
        this->name = name;
        this->country = country;
        this->lastModify = lastModify;
    }

    void setID(int id);
    void setName(std::string name);
    void setCountry(std::string country);
    void setLastModify(long timeStamp);

    int getID();
    std::string getName();
    std::string getCountry();
    long getLastModify();

    std::string dbQuery(int type);
};


#endif //PPARSER_MANUFACTURER_H
