#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <sys/wait.h>
#include <limits.h>

#define STACK_SIZE (1024 * 1024)    // Stack size for cloned child

int childFunc(void *arg) {
    const char* new_hostname = "container";
    if (sethostname(new_hostname, strlen(new_hostname)) != 0) {
        std::cout << "Failed to set hostname" << std::endl;
        return -1;
    }
    // get the current working directory
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        std::cout << "getcwd() error" << std::endl;
        return -1;
    }
    std::string newRoot = "/home/tobias/projects/codingChallenges/dockerClone/test/alpineTest";
    if (chroot(newRoot.c_str()) != 0) {
        std::cout << "Failed to change root directory" << std::endl;
        return -1;
    }

    // Execute the command received in arg
    char *command = (char *)arg;
    std::cout << "command = " << command << std::endl;
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
    std::cout << "returnCodeOfCommandExecution = " << returnCodeOfCommandExecution << std::endl;
    return returnCodeOfCommandExecution;
}