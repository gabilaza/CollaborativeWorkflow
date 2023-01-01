
#include <iostream>
#include "server.hpp"
#include "utils/logs.hpp"

using namespace logger;
using namespace std;


int main(int argc, char** argv) {
    //logs::logOnTheDisk("server.log");
    logs(DEBUG) << "Started";

    if(argc != 3) {
        cout << "Usage: ./server host port\n";
        raiseError("Not enough args");
    }

    logs(DEBUG) << "Host: " << argv[1];
    logs(DEBUG) << "Port: " << argv[2];

    //start
    Server server(argv[1], argv[2]);
    server.run();


    logs(DEBUG) << "Finished";
    return 0;
}
