
#include <iostream>
#include "client.hpp"
#include "utils/logs.hpp"

using namespace logger;
using namespace std;


int main(int argc, char** argv) {
    //logs::logOnTheDisk("client.log");
    logs(DEBUG) << "Started";

    if(argc != 3) {
        cout << "Usage: ./client host port\n";
        raiseError("Not enough args");
    }

    logs(DEBUG) << "Host: " << argv[1];
    logs(DEBUG) << "Port: " << argv[2];

    //start
    Client client(argv[1], argv[2]);
    client.run();


    logs(DEBUG) << "Finished";
    return 0;
}
