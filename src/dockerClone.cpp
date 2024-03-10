#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <sys/wait.h>
#define STACK_SIZE (1024 * 1024)    // Stack size for cloned child

int childFunc(void *arg) {
    const char* new_hostname = "container";
    if (sethostname(new_hostname, strlen(new_hostname)) != 0) {
        std::cerr << "Failed to set hostname" << std::endl;
        return -1;
    }
    // Execute the command received in arg
    char *command = (char *)arg;
    // std::cout << "command = " << command << std::endl;
    int returnFromCommand = system(command);
    return returnFromCommand;
}
// void setHostnameToContainer(){
//     const char* = "container";
//     if (sethostname(new_hostname, strlen(new_hostname)) != 0) {
//         std::cerr << "Failed to set hostname" << std::endl;
//         return -1;
// }

int main(int argc, char *argv[]) {
    // if (strcmp(argv[1], "run") == 0) {

    //     // first create a new namespace, so the container is isolated from the host
    //     // then run the command in the new namespace
    //     int returnFromUnshare = system("unshare -u");
    //     if (returnFromUnshare != 0) {
    //         std::cerr << "Failed to create new UTS namespace" << std::endl;
    //         return returnFromUnshare;
    //     }
    //     // set the hostname for the new UTS namespace
    //     const char* new_hostname = "container";
    //     if (sethostname(new_hostname, strlen(new_hostname)) != 0) {
    //         std::cerr << "Failed to set hostname" << std::endl;
    //         return -1;
    //     }


    //     // std::cout << "command = " << command << std::endl;

    // }
    int returnCodeOfCommandExecution = 0;
    if (strcmp(argv[1], "run") == 0) {
        char *stack;                    // Start of stack buffer
        char *stackTop;                 // End of stack buffer
        stack = (char *)malloc(STACK_SIZE);
        if (stack == NULL) {
            std::cerr << "Failed to allocate memory for stack" << std::endl;
            return -1;
        }
        stackTop = stack + STACK_SIZE;  // Assume stack grows downward
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
        char *cStr = new char[command.length() + 1];
        std::strcpy(cStr, command.c_str());
        // Create child that has its own UTS namespace;
        // child commences execution in childFunc()
        returnCodeOfCommandExecution = clone(childFunc, stackTop, CLONE_NEWUTS | SIGCHLD, cStr);

        wait(NULL); // Wait for child
    }
    // std::cout << "returnCodeOfCommandExecution = " << returnCodeOfCommandExecution << std::endl;
    return returnCodeOfCommandExecution;
}