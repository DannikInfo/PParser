#include "logger.h"
#include "dataManager.h"
#include "config.h"
#include "configPaths.h"

int main() {
    std::vector<std::string> markerDirs = {"Документ", "Фото", "Чертежи", "Программ", "Doc", "Схемы", "Draw", "Prog", "SOFT", "Исходники"};
    std::vector<std::string> markerFiles = {"ttt.txt", "project.txt", "info_project.txt", "project.json"};

    logger::init("PParser", 50, 10);
    config::init("config");
    config::set(DB_URL, "localhost");
    config::set(DB_USER, "root");
    config::set(DB_PASS, "root");
    config::set(DB_NAME, "db");
    config::set(SMB_ROOT, "/media/srv-adnt");
    config::set(REAL_ROOT, "//srv-adnt/projects$");
    config::set(MAX_SEARCH_DEPTH, 4);
    config::set(DEBUG, false);
    config::set(DELAY, 60);
    config::set(MARKER_DIRS, markerDirs);
    config::set(MARKER_FILES, markerFiles);
    config::loadConfig();

    while(true) {
        dataManager dm;
        dm.searchTask();
        dm.fixTask();
        sleep(config::get<int>(DELAY));
    }
}
