//
// Created by Sheldon Lobo on 1/28/25.
//

#include <iostream>

#include "constants.h"

#include "Poco/Environment.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequest.h"

class MyRequestHandler : public Poco::Net::HTTPRequestHandler {
private:
    int count;
    std::string version;

public:
    explicit MyRequestHandler(int count) : count(count) {
        std::string pocoVersion = Poco::format("Poco %d.%d.%d",
            static_cast<int>(Poco::Environment::libraryVersion() >> 24),
            static_cast<int>((Poco::Environment::libraryVersion() >> 16) & 0xFF),
            static_cast<int>((Poco::Environment::libraryVersion() >> 8) & 0xFF));
        version = "C++ " + std::string(__VERSION__) + " / " + pocoVersion;
    }

    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override {
        std::string userAgent = request.get("User-Agent", "Unknown");
        std::cout << "[" << count << "] Received " << request.getURI() << " request from: " <<
            userAgent << std::endl;
        count++;

        response.setContentType("text/plain");
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.set("Server", version);
        std::ostream &responseStream = response.send();

        if (request.getURI() == ASIA_PATH) {
            responseStream << ASIA_COUNTRY;
        } else if (request.getURI() == AMERICA_PATH) {
            responseStream << AMERICA_COUNTRY;
        }
    }
};

class MyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
private:
    int count = 0;
public:
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&) override {
        return new MyRequestHandler(count++);
    }
};

class MyHTTPServer : public Poco::Util::ServerApplication {
protected:
    int main(const std::vector<std::string> &) override {
        Poco::Net::ServerSocket socket(8080); // Listen on port 8080
        Poco::Net::HTTPServer server(new MyRequestHandlerFactory(), socket, new Poco::Net::HTTPServerParams);

        std::cout << "Starting C++ POCO Server on port 8080 ..." << std::endl;
        server.start();

        waitForTerminationRequest(); // Wait for CTRL+C or termination signal

        std::cout << "Shutting down C++ POCO server on port 8080." << std::endl;
        server.stop();

        return Application::EXIT_OK;
    }
};

int main(int argc, char** argv) {
    MyHTTPServer app;
    return app.run(argc, argv);
}
