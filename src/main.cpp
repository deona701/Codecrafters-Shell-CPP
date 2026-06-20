#include <iostream>
#include <string>
#include <vector>
#include <sstream>   
#include <cstdlib>   
#include <unistd.h> 

using namespace std;

int main() {
    // Flush after every std::cout / std::cerr
    cout << unitbuf;
    cerr << unitbuf;

    vector<string> commands = {"exit", "echo", "type"};

    while (true) {
        cout << "$ ";
        string command;
        getline(cin, command);

        if (command == "exit") {
            break;
        }
       
        else if (command.size() >= 5 && command.substr(0, 5) == "echo ") {
            cout << command.substr(5) << endl;
        }
        else if (command.size() >= 5 && command.substr(0, 5) == "type ") {
            string requested_command = command.substr(5);
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
                stringstream ss(path);
                string directory;
                bool binary_found = false;

                while (getline(ss, directory, ':')) {
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
        else {
            cout << command << ": command not found" << endl; 
        }
    }

    return 0;
}