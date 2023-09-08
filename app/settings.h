#pragma once

#include <string>

namespace app {

    struct Settings {
        int                         displayWidth        = 1024;
        int                         displayHeight       = 768;
        bool                        fullscreen          = false;

        static Settings             initializeFromFile(const std::string& path);
    };

}
