#include <iostream>
#include <string>
#include <vector>
#include <sstream>   
#include <cstdlib>   
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

//functions to clean up code
void handle_pwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        cout << cwd << endl;
    }
}

void handle_echo(const vector<string>& args) {
    for (size_t i = 1; i < args.size(); ++i) {
        cout << args[i];

        if (i < args.size() - 1) {
            cout << " ";
        }
    }
    cout << endl;
}

void handle_type(const vector<string>& args, const vector<string>& commands) {
    if (args.size() < 2) {
        cout << "type: missing arguments" << endl;
        return; // Exits this function early and returns control to main
    }

    string requested_command = args[1];
    bool found = false;

    for (string builtin : commands) {
        if (requested_command == builtin) {
            cout << builtin << " is a shell builtin" << endl;
            found = true;
            break; 
        }
    }

    if (found == false) {
        string path = getenv("PATH");
        stringstream path_ss(path);
        string directory;
        bool binary_found = false;

        while (getline(path_ss, directory, ':')) {
            string full_path = directory + "/" + requested_command;
            if (!access(full_path.c_str(), X_OK)) {
                cout << requested_command << " is " << full_path << endl;
                binary_found = true;
                break; 
            }
        }

        if (!binary_found) {
            cout << requested_command << ": not found" << endl;
        }
    }
}

void handle_external_command(const vector<string>& args) {
    string base_cmd = args[0];
    string path = getenv("PATH");
    stringstream path_ss(path);
    string directory;
    bool binary_found = false;
    string full_path = "";

    while (getline(path_ss, directory, ':')) {
        string test_path = directory + "/" + base_cmd;
        if (!access(test_path.c_str(), X_OK)) {
            binary_found = true;
            full_path = test_path;
            break;
        }
    }

    if (binary_found) {
        pid_t pid = fork();

        if (pid == 0) {
            vector<char*> argv;
            for (const string& argument : args) {
                argv.push_back(const_cast<char*>(argument.c_str()));
            }
            argv.push_back(nullptr);
            
            execv(full_path.c_str(), argv.data());
            exit(1);
        }

        if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
        }
    } 
    else {
        cout << base_cmd << ": not found" << endl;
    }
}

int main() {
    cout << unitbuf;
    cerr << unitbuf;

    vector<string> commands = {"exit", "echo", "type", "pwd"};

    while (true) {
        cout << "$ ";
        string command;
        getline(cin, command);

        vector<string> args;
        stringstream ss(command);
        string token;
        while (ss >> token) {
            args.push_back(token);
        }
        
        if (args.empty()) {
            continue;
        }

        if (args[0] == "exit") {
            break;
        }
        else if (args[0] == "echo") {
            handle_echo(args);
        }
        else if (args[0] == "pwd") {
            handle_pwd();
        }
        else if (args[0] == "type") {
            handle_type(args, commands);
        } 
        else {
            handle_external_command(args);
        }
    }

    return 0;
}
