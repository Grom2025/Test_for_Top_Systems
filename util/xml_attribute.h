#pragma once

#include <sstream>
#include <stdexcept>

#include "pugixml.hpp"

namespace util {
    
    template <typename T>
    T readXmlAttribute(const pugi::xml_attribute& attr) {
        if (attr.empty()) {
            throw std::runtime_error("xml_attribute is empty");
        }

        std::istringstream iss(attr.value());
        T result;
        iss >> result;
        return result;
    }

    template <typename T>
    void tryReadXmlAttribute(T& value, const pugi::xml_attribute& attr) {
        try {
            value = readXmlAttribute<T>(attr);
        }
        catch (const std::runtime_error&) {
        }
    }

}
