#include "viewer/AgendaUI.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <fstream>
#include <string>


int main() {

    // get config dir
    struct passwd *pw = getpwuid(getuid());
    string homeDir = pw->pw_dir;
    string envDir = homeDir + "/.miniagenda";
    // mkdir if not exist
    mkdir(envDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    // create json file if not exist
    string jsonFile = envDir + "/Agenda.json";
    if (!std::ifstream(jsonFile.c_str())) {
        std::ofstream outfile(jsonFile.c_str());;
        outfile.close();
    }

    AgendaUI *aui = new AgendaUI(jsonFile, envDir+"/Agenda.sqlite3");
    aui->OperationLoop();

    if (aui != NULL) {
        delete aui;
        aui = NULL;
    }
    
    return 0;
}
