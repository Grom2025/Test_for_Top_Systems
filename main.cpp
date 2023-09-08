
#include <string>
#include "app/app.h"
#include "app/settings.h"
using namespace std;
using namespace app;
const std::string SETTINGS_FILE_PATH = "settings.xml";

int main( int argc, char* args[] )
{
    app::Settings settings = app::Settings::initializeFromFile(SETTINGS_FILE_PATH);
    app::Application app;

    auto initSuccess = app.initialize("Test for Top Systems by Grom", settings);
    if (!initSuccess) {
        return EXIT_FAILURE;
    }

    app.setRunning(true);
    loop(app);

    return EXIT_SUCCESS;
}