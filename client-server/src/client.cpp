//
// Created by Sheldon Lobo on 1/31/25.
//

#include <iostream>
#include <sstream>
#include <random>

#include "constants.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"

using namespace std::chrono_literals;

void makeRequest(int port, const std::string& path) {
    static int count = 0;

    try {
        // Create a client session
        Poco::Net::HTTPClientSession session(SERVER, port);

        // Prepare the request
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path);
        request.setVersion(Poco::Net::HTTPMessage::HTTP_1_1);
        request.set("User-Agent", pocoVersion());

        // Send the request
        session.sendRequest(request);

        // Receive the response
        Poco::Net::HTTPResponse response;
        std::istream& responseStream = session.receiveResponse(response);

        // Print response status
        std::cout << "[" << count << "] To: " << port << "; Got: " << response.getStatus() << " {";

        // Print the `Server` header
        if (response.has("Server")) {
            std::cout << response.get("Server") << "} ";
        }

        // Read and print response body
        Poco::StreamCopier::copyStream(responseStream, std::cout);
        std::cout << std::endl;

        count++;
    }
    catch (Poco::Exception& ex) {
        std::cerr << "Exception: " << ex.displayText() << std::endl;
    }
}


[[noreturn]] int main(int argc, char** argv) {
    if (argc > 2) {
        std::cout << "Usage: " << argv[0] << " [start-port[:end-port]]" << std::endl;
        std::cout << "\tdefault : 8080:8080" << std::endl;
        exit(1);
    }
    int portStart = PORT;
    int portEnd = portStart;
    if (argc == 2) {
        std::string ports = argv[1];
        size_t end;
        if ((end = ports.find(':')) != std::string::npos) {
            portStart = std::stoi(ports.substr(0, end));
            portEnd = std::stoi(ports.substr(end + 1));
        } else {
            portStart = std::stoi(argv[1]);
            portEnd = portStart;
        }
    }
    std::cout << "Sending requests to port(s): " << portStart << ":" << portEnd << std::endl;

    // Random number generator
    std::random_device rd;   // Non-deterministic random seed
    std::mt19937 gen(rd());  // Mersenne Twister PRNG
    std::uniform_int_distribution<int> dist(portStart, portEnd);  // Range [1, 100]

    while (true) {
        makeRequest(dist(gen), ASIA_PATH);
        std::this_thread::sleep_for(2s);

        makeRequest(dist(gen), AMERICA_PATH);
        std::this_thread::sleep_for(2s);
    }
}
