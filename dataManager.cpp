#include "dataManager.h"

dataManager::dataManager(){
    driver = get_driver_instance();
    conOpt["hostName"] = config::get<std::string>(DB_URL);
    conOpt["userName"] = config::get<std::string>(DB_USER);
    conOpt["password"] = config::get<std::string>(DB_PASS);
    conOpt["schema"] = config::get<std::string>(DB_NAME);
    conOpt["characterSetResults"] = "utf8";
    conOpt["OPT_CHARSET_NAME"] = "utf8";
    conOpt["OPT_SET_CHARSET_NAME"] = "utf8";

    markerFiles = config::get<std::vector<std::string>>(MARKER_FILES);
    markerDirs = config::get<std::vector<std::string>>(MARKER_DIRS);
}

void dataManager::searchProjects(const std::string& root){
    project prj;
    bool prjFound, isFile;
    std::vector<std::string> vPath;
    std::string nextPath, pName, eName, rootCp;
    std::vector<std::string> markers;
    counter++;

    //Check terms
    if(!std::filesystem::exists(root)){
        logger::error(root + "not exists!");
        return;
    }
    if(counter > config::get<int>(MAX_SEARCH_DEPTH)) {
        counter--;
        return;
    }

    //Recursive directory parsing
    for (const auto &entry : std::filesystem::directory_iterator(root)) {
        vPath.clear();
        utils::split(entry.path(), "/", vPath);
        eName = vPath.back(); //Get current path name

        vPath.clear();
        rootCp = root;
        utils::split(rootCp, "/", vPath);
        pName = vPath.back(); //get root path name

        if(!strcmp(eName.c_str(), ".") || !strcmp(eName.c_str(), "..")) continue; //exclude back and current links

        nextPath = root + "/" + eName; //Get next path to call recursive method

        //Clear marker vars
        prjFound = false;
        isFile = false;
        markers.clear();
        if(std::filesystem::is_directory(nextPath.c_str())) //Checking if this is a directory or a file and selecting the desired markers
            markers = markerDirs;
        else{
            markers = markerFiles;
            isFile = true;
        }

        for (const auto &item : markers) //Checking markers
            if(strcasestr(eName.c_str(), item.c_str()) != nullptr)
                prjFound = true;

        if(prjFound && counter > 2){ //If markers found, write prj to found
            prj.setDir(root);
            prj.setName(pName);
            if(isFile)
                prj.setNote("contains:" + eName);

            if(config::get<bool>(DEBUG))
                logger::success(pName+": "+root + ": " + std::to_string(counter));
            foundProjects.push_back(prj);
            counter--;
            return;
        }else
            if(!isFile)
                searchProjects(nextPath); //recursive call
    }
    counter--;
}

bool dataManager::dirExistsInDB(std::string path){
    bool exists = false;
    path.erase(0, config::get<std::string>(SMB_ROOT).length());

    std::map<int, project>::iterator itPrDB;
    for(itPrDB = projectsDB.begin(); itPrDB != projectsDB.end(); itPrDB++){
        project prDB = itPrDB->second;

        if(strstr(path.c_str(), prDB.getDir().c_str()) != nullptr || strstr(prDB.getDir().c_str()
                                                                            , path.c_str()) != nullptr){
            exists = true;
        }
    }
    return exists;
}

void dataManager::loadDataFromDB(){
    int id;
    std::unique_ptr<sql::Connection> con(driver->connect(conOpt));

    //Load buildings
    std::string sql = "SELECT * FROM buildings";
    sql::ResultSet *rs = con->createStatement()->executeQuery(sql);
    building b;
    while(rs->next()){
        id = rs->getInt("id");
        b = building(id, rs->getString("name"), rs->getInt64("lastModify"));
        buildingsDB[id] = b;
    }

    //Load Workshops
    sql = "SELECT * FROM workshops w ";
    sql += "LEFT JOIN workshopsInBuildings ib ON w.id = ib.workshopID";
    rs = con->createStatement()->executeQuery(sql);

    workshop w;
    while(rs->next()){
        id = rs->getInt("id");
        if(workshopsDB.find(id) == workshopsDB.end()){
            w = workshop(id, rs->getString("name"), rs->getInt("buildingID"), rs->getInt64("lastModify"));
            workshopsDB[id] = w;
        }else{
            w = workshopsDB[id];
            w.addBuilding(rs->getInt("buildingID"));
            workshopsDB[id] = w;
        }
    }

    //Load manufacturer
    sql = "SELECT * FROM manufacturers";
    rs = con->createStatement()->executeQuery(sql);

    manufacturer m;
    while(rs->next()){
        id = rs->getInt("id");
        m = manufacturer(id, rs->getString("name"), rs->getString("country"), rs->getInt64("lastModify"));
        manufacturersDB[id] = m;
    }

    //Load personal
    sql = "SELECT * FROM personal";
    rs = con->createStatement()->executeQuery(sql);

    person prsn;
    while(rs->next()){
        std::string nameS = rs->getString("fullName");
        prsn = person(rs->getInt("id"), nameS, rs->getString("phone"), rs->getString("email"), rs->getInt64("lastModify"));
        personsDB[nameS] = prsn;
    }

    //load projects
    sql = "SELECT prj.id as prjID, prj.nameOfficial as nameOfficial, prj.nameNoOfficial as nameNoOfficial,";
    sql += "prj.commissioning as commissioning, prj.modify as modify, prj.manufacturerID as mID,";
    sql +="prj.workshopID as wID, prj.buildingID as bID,";
    sql +="prj.dir as dir, prj.note as note, prj.lastModify as lastModify,";
    sql +="pn.id as pnID, pn.fullName as fullName, pn.phone as phone, pn.email as email,";
    sql +="sPrj.id as spID, sPrj.nameOfficial as spNameOff, sPrj.nameNoOfficial as spNameNoOff ";
    sql +="FROM projects prj ";
    sql +="LEFT JOIN personalInProjects ip ON prj.id=ip.projectID ";
    sql +="LEFT JOIN personal pn on ip.personID=pn.id ";
    sql +="LEFT JOIN subProjects sp ON prj.id=sp.projectID ";
    sql +="LEFT JOIN projects sPrj ON sp.subProjectID=sPrj.id";
    rs = con->createStatement()->executeQuery(sql);

    project prj;
    int person = 0;
    int subPrj = 0;
    while(rs->next()){
        id = rs->getInt("prjID");
        if(projectsDB.find(id) == projectsDB.end()){
            prj = project(id,
                          rs->getString("nameOfficial"),
                          rs->getString("nameNoOfficial"),
                          rs->getInt("commissioning"),
                          rs->getInt("modify"),
                          rs->getInt("mID"),
                          rs->getInt("wID"),
                          rs->getInt("bID"),
                          rs->getString("dir"),
                          rs->getString("note"),
                          rs->getString("fullName"),
                          rs->getInt("spID"),
                          rs->getInt64("lastModify"));
            std::string spNameOffStr = rs->getString("spNameOff");
            prj.addSubPrjName(spNameOffStr);
            projectsDB[id] = prj;
        }else{
            prj = projectsDB[id];
            std::string fName = rs->getString("fullName");
            subPrj = rs->getInt("spID");

            if(!fName.empty())
                prj.addPerson(fName);

            if(subPrj > 0)
                prj.addSubPrj(subPrj);

            projectsDB[id] = prj;
        }
    }
    con->close();
}

void dataManager::loadDataFromFile(std::string &path){
    if(strstr(path.c_str(), "project.json")){
        nlohmann::json j;
        std::ifstream f(path.c_str());
        if(f.is_open()) {
            f >> j;

            project prj;
            manufacturer m;
            std::vector<std::string> persons;
            building b;
            workshop w;

            //manufacturer
            if(j["manufacturer"].contains("id")) {
                m.setID(j["manufacturer"]["id"].get<int>());
                m.setName(j["manufacturer"]["name"].get<std::string>());
                m.setCountry(j["manufacturer"]["country"].get<std::string>());
            }
            //building
            if(j["building"].contains("id")) {
                b.setID(j["building"]["id"]);
                b.setName(j["building"]["name"].get<std::string>());
            }

            //workshop
            if(j["workshop"].contains("id")) {
                w.setID(j["workshop"]["id"]);
                w.setName(j["workshop"]["name"].get<std::string>());
            }
            //Project
            prj.setID(j["id"].get<int>());
            prj.setName(j["name"].get<std::string>());
            prj.setNameNoOff(j["nameNoOff"].get<std::string>());
            prj.setCommissioning(j["commissioning"].get<int>());
            prj.setModify(j["modify"].get<int>());
            prj.setManufacturerID(m.getID());
            prj.setBuildingID(b.getID());
            prj.setWorkshopID(w.getID());
            prj.setPersons(j["persons"].get<std::vector<std::string>>());


            std::vector<std::string> vPath;
            vPath.clear();
            utils::split(path, "/", vPath);

            std::string prjPath;
            for(int i = 2; i < vPath.size()-1; i++)
                prjPath += "/" + vPath[i];
            prj.setDir(prjPath);
            prj.setNote(j["note"].get<std::string>());

            prj.setSubPrjs(j["subProjects"]["id"].get<std::list<int>>());
            prj.setSubPrjsNames(j["subProjects"]["noOffNames"].get<std::vector<std::string>>());

            projectsF[prj.getID()] = prj;
            workshopsF[w.getID()] = w;
            buildingsF[b.getID()] = b;
            manufacturersF[m.getID()] = m;
            if(config::get<bool>(DEBUG))
                logger::info("loaded project from file: " + prj.getName() + " with id " + std::to_string(prj.getID()));
            f.close();
        }
    }
}

void dataManager::foundUpload(){
    std::map<int, project>::iterator itDB;
    std::unique_ptr<sql::Connection> con(driver->connect(conOpt));

    std::string globalPath, dir;
    project dbPrj, fPrj;

    //Excluding duplicates in found projects
    for(itDB = projectsDB.begin(); itDB != projectsDB.end(); itDB++){
         dbPrj = itDB->second;
        for(int i = 0; i < foundProjects.size(); i++){
            fPrj = foundProjects[i];

            dir = fPrj.getDir();
            dir.erase(0, config::get<std::string>(SMB_ROOT).length());

            globalPath = "//srv-adnt/projects$" + dir;
            if(!strcasecmp(dbPrj.getDir().c_str(), globalPath.c_str())|| //Check duplicates by dir and name
               !strcasecmp(dbPrj.getName().c_str(), fPrj.getName().c_str())
                    ){
                foundProjects.erase(foundProjects.begin() + i);
               if(config::get<bool>(DEBUG))
                   logger::info("duplicate: " + fPrj.getName());
            }
        }
    }

    logger::info("Count of found projects: " + std::to_string(foundProjects.size()));

    //Uploading projects to DB
    for (project &prj : foundProjects){
        logger::info("Upload project "+ prj.getName() + " in path "+ prj.getDir());

        dir = prj.getDir();
        dir.erase(0, config::get<std::string>(SMB_ROOT).length());
        globalPath = "//srv-adnt/projects$" + dir;

        std::string sql = "INSERT INTO projects SET nameOfficial='"+prj.getName()+"', nameNoOfficial='', commissioning=0, modify=0, lastModify=0,  dir='"+globalPath+"', note='"+prj.getNote()+"'";

        if(config::get<bool>(DEBUG))
            logger::info("SQL: "+sql);
        else
            con->createStatement()->execute(sql);
    }

    if(!foundProjects.empty())
        logger::success("Found projects uploaded!");
}

void dataManager::fixDirectories(){
    std::vector<project> vecToDB, vecToFiles;
    std::map<int, project>::iterator itDB;
    std::string dir, sql;
    std::unique_ptr<sql::Connection> con(driver->connect(conOpt));
    project dbPrj, fPrj;

    //Definition need update dir in DB or write .json copy to project path
    for(itDB = projectsDB.begin(); itDB != projectsDB.end(); itDB++){
        dbPrj = itDB->second;
        if(projectsF.contains(dbPrj.getID())){
            fPrj = projectsF[dbPrj.getID()];
            dir = fPrj.getDir();

            std::string dirSrv = config::get<std::string>(REAL_ROOT) + dir;

            if(strcasecmp(dirSrv.c_str(), dbPrj.getDir().c_str()) != 0 ||
               dirSrv.empty()
               ){
                dbPrj.setDir(dirSrv);
                vecToDB.push_back(dbPrj);
            }
        }else{
            logger::info("To file prj: " + std::to_string(dbPrj.getID()) + " " + dbPrj.getName());
            vecToFiles.push_back(dbPrj);
        }
    }

    //Upload to DB
    for(auto & i : vecToDB){
        dbPrj = i;
        sql = "UPDATE projects SET dir=\""+dbPrj.getDir()+"\" WHERE id="+std::to_string(dbPrj.getID());
        if(config::get<bool>(DEBUG))
            logger::info("SQL: "+sql);
        else
            con->createStatement()->execute(sql);
    }

    //Write to files
    for(auto &prj : vecToFiles){
        if(config::get<bool>(DEBUG))
            logger::info("Update in file prj:" + std::to_string(prj.getID())+ " " + prj.getName());
        else
            toFile(prj);
    }

    if(!vecToDB.empty())
        logger::success("Projects paths in DB updated!");
    if(!vecToFiles.empty())
        logger::success("Projects files updated!");
    if(vecToFiles.empty() && vecToDB.empty())
        logger::success("Nothing to update!");
}

void dataManager::toFile(project prj){
    std::string dir = prj.getDir();
    dir.erase(0, config::get<std::string>(REAL_ROOT).length());

    std::string dirLocal = config::get<std::string>(SMB_ROOT) + dir;

    if(!std::filesystem::exists(dirLocal.c_str())){
        logger::error("dir not exists: " + std::to_string(prj.getID()) + " - " + prj.getDir());
        return;
    }

    //Parsing objects into JSON
    nlohmann::json j;
    j["id"] = prj.getID();
    j["name"] = prj.getName();
    j["nameNoOff"] = prj.getNameNoOff();
    j["commissioning"] = prj.getCommissioning();
    j["modify"] = prj.getModify();

    if(manufacturersDB.contains(prj.getManufacturerID())){
        manufacturer m = manufacturersDB.at(prj.getManufacturerID());
        j["manufacturer"]["id"] = prj.getManufacturerID();
        j["manufacturer"]["name"] = m.getName();
        j["manufacturer"]["country"] = m.getCountry();
    }

    if(workshopsDB.contains(prj.getWorkshopID())) {
        workshop w = workshopsDB.at(prj.getWorkshopID());
        j["workshop"]["id"] = prj.getWorkshopID();
        j["workshop"]["name"] = w.getName();
    }

    if(buildingsDB.contains(prj.getBuildingID())) {
        building b = buildingsDB.at(prj.getBuildingID());
        j["building"]["id"] = prj.getBuildingID();
        j["building"]["name"] = b.getName();
    }

    j["dir"] = prj.getDir();
    j["note"] = prj.getNote();
    j["persons"] = prj.getPersons();

    j["subProjects"]["id"] = prj.getSubPrjs();
    j["subProjects"]["noOffNames"] = prj.getSubPrjsNames();

    std::string path = dirLocal + "/project.json";

    //Write pretty JSON to file
    std::ofstream o(path);
    if(o.is_open()) {
        o << std::setw(4) << j << std::endl;
        o.close();
    }else
        logger::error("Failed to save file for project: " + prj.getName());
}

void dataManager::fileData(const std::string& path, int function){
    counter++;

    //Check terms
    if(!std::filesystem::exists(path)) {
        logger::error("Error open dir: " + path);
        counter--;
        return;
    }
    if(counter > config::get<int>(MAX_SEARCH_DEPTH)) {
        counter--;
        return;
    }

    std::vector<std::string> vPath;
    std::string nextPath;
    for (const auto &entry : std::filesystem::directory_iterator(path)) { //Finding .json project files
        vPath.clear();
        utils::split(entry.path(), "/", vPath);
        std::string eName = vPath.back();
        if(!strcmp(eName.c_str(), ".") || !strcmp(eName.c_str(), "..")) continue;

        nextPath = path + "/" + eName;

        if(std::filesystem::is_directory(nextPath.c_str())){
            fileData(nextPath, function);
        }else{
            switch(function){ //Maybe in future it will be more
                case 0:
                    loadDataFromFile(nextPath);
                    break;
            }
        }
    }
    counter--;
}

void dataManager::searchTask(){
    logger::info("Start searching projects");

    buildingsDB.clear();
    workshopsDB.clear();
    manufacturersDB.clear();
    personsDB.clear();
    projectsDB.clear();

    loadDataFromDB();
    logger::success("Data loaded from DB");
    logger::info("Global projects: " + std::to_string(projectsDB.size()));

    foundProjects.clear();
    auto root = config::get<std::string>(SMB_ROOT);
    searchProjects(root);
    logger::success("Projects searching ended");

    //TODO: potential projects
//    for(int i = 0; i < potentialPrjs.size(); i++){
//        for(int j = 0; j < foundProjects.size(); j++){
//            if((strstr(potentialPrjs[j].c_str(), foundProjects[i].getDir().c_str()) != NULL ||
//                strstr(foundProjects[i].getDir().c_str(), potentialPrjs[j].c_str()) != NULL) &&
//               potentialPrjs.begin() + i != potentialPrjs.end()){
//                potentialPrjs.erase(potentialPrjs.begin() + i);
//            }
//        }
//    }

//    logger::info("Potential projects paths:");
//    for(auto &potentialPrj : potentialPrjs)
//        logger::warn("dir " + potentialPrj + " is potential project!!");

    foundUpload();
}

void dataManager::fixTask(){
    logger::info("Start fixing projects directories");

    loadDataFromDB();
    logger::success("Data loaded from DB");
    logger::info("Global projects: " + std::to_string(projectsDB.size()));

    //Clear data
    projectsF.clear();
    buildingsF.clear();
    workshopsF.clear();
    personsF.clear();
    manufacturersF.clear();

    fileData(config::get<std::string>(SMB_ROOT), 0);
    logger::success("Projects files loaded");
    logger::info("Local projects:" + std::to_string(projectsF.size()));

    fixDirectories();
}