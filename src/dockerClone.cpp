#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
int main(int argc, char *argv[]) {
    if (strcmp(argv[1], "run") == 0) {
        std::string command = "";
        for (int i = 2; i < argc; i++) {
            command += argv[i];
            command += " ";
            // std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
        }
        if (!command.empty()) {
            command.pop_back(); // remove the last space
        }
        // std::cout << "command = " << command << std::endl;
        int returnFromCommand = system(command.c_str());
        // std::cout << "returnFromCommand = " << returnFromCommand << std::endl;
        return returnFromCommand;
    }

}