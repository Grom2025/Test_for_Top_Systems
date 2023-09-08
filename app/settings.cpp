#include <sstream>

#include "settings.h"

#include "../util/xml_attribute.h"

using namespace std;
using namespace app;
using namespace util;
using namespace pugi;

Settings Settings::initializeFromFile(const std::string& path) {
    Settings settings;
    xml_document doc;

    auto loadSuccess = doc.load_file(path.c_str());
    if (loadSuccess) {
        xml_node settingsNode = doc.child("Settings");

        xml_node displayNode = settingsNode.child("Display");
        xml_node displaySizeNode = displayNode.child("Size");
        xml_node fullscreenNode = displayNode.child("Fullscreen");

        tryReadXmlAttribute(settings.displayWidth, displaySizeNode.attribute("width"));
        tryReadXmlAttribute(settings.displayHeight, displaySizeNode.attribute("height"));
        tryReadXmlAttribute(settings.fullscreen, fullscreenNode.attribute("value"));
    }

    return settings;
}
