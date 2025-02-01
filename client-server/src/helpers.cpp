//
// Created by Sheldon Lobo on 1/31/25.
//

#include <Poco/Environment.h>
#include <Poco/Format.h>

std::string pocoVersion() {
    return "C++ " + std::string(__VERSION__) + " / " + Poco::format("Poco %d.%d.%d",
        static_cast<int>(Poco::Environment::libraryVersion() >> 24),
        static_cast<int>((Poco::Environment::libraryVersion() >> 16) & 0xFF),
        static_cast<int>((Poco::Environment::libraryVersion() >> 8) & 0xFF)
    );
}
