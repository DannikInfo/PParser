#ifndef PPARSER_PERSON_H
#define PPARSER_PERSON_H

#include <iostream>

class person {
private:
    int id;
    std::string fullName;
    std::string phone;
    std::string email;
    long lastModify;

public:
    person(){}

    person(int id, std::string fullName, std::string phone, std::string email, long lastModify){
        this->id = id;
        this->fullName = fullName;
        this->phone = phone;
        this->email = email;
        this->lastModify = lastModify;
    }

    void setID(int id);
    void setFullName(std::string fullName);
    void setPhone(std::string phone);
    void setEmail(std::string email);
    void setLastModify(long timeStamp);

    int getID();
    std::string getFullName();
    std::string getPhone();
    std::string getEmail();
    long getLastModify();

    std::string dbQuery(int type);
};


#endif //PPARSER_PERSON_H
