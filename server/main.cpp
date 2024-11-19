#include <exception>
#include <iostream>

#include "server.h"
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <vector>
#include <fstream>

const int REQUIRED_ARGS_QTY = 2;
const int SUCCESSFUL_RUN = 0;
const int ERROR = -1;


int main(int argc, char* argv[]) {
    try {


        const char* servname = nullptr;
        if (argc != REQUIRED_ARGS_QTY) {
            std::cerr << "Bad Server program call. Expected: ./server servname"
                      << "\n";
            return ERROR;
        }
        servname = argv[1];
        Server server(servname);
        server.run();
        return SUCCESSFUL_RUN;
    } catch (const std::exception& error) {
        std::cerr << "Thrown exception on server's side. Cause: " << error.what() << "\n";
        return ERROR;
    } catch (...) {
        std::cerr << "Unknown exception on server's side.\n";
        return ERROR;
    }
}
