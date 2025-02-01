//
// Created by Sheldon Lobo on 1/28/25.
//

#include <iostream>

#include "constants.h"

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequest.h"

#include "Poco/Util/ServerApplication.h"

class MyRequestHandler : public Poco::Net::HTTPRequestHandler {
private:
    int count;
    std::string version;

public:
    explicit MyRequestHandler(int count) : count(count) {
        version = pocoVersion();
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
    int main(const std::vector<std::string> &args) override {
        int port = PORT;
        if (args.size() == 1) {
            port = std::stoi(args[0]);
        }
        Poco::Net::ServerSocket socket(port);
        Poco::Net::HTTPServer server(new MyRequestHandlerFactory(), socket, new Poco::Net::HTTPServerParams);

        std::cout << "Starting C++ POCO Server on port "<< port << std::endl;
        server.start();

        waitForTerminationRequest(); // Wait for CTRL+C or termination signal

        std::cout << "Shutting down C++ POCO server on port 8080." << std::endl;
        server.stop();

        return Application::EXIT_OK;
    }
};

int main(int argc, char** argv) {
    if (argc > 2) {
        std::cout << "Usage: " << argv[0] << " [port]" << std::endl;
        std::cout << "\tdefault : 8080" << std::endl;
        exit(1);
    }

    MyHTTPServer app;
    return app.run(argc, argv);
}
