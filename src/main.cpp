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

        vector<string> args;
        stringstream ss(command);
        string token;
        while (ss >> token) {
          args.push_back(token);
          cout << "Number of arguemets passed: " << args.size() << endl;

          if (args.empty()) {
            continue;
          }
        }

        if (args[0] == "exit") {
            break;
        }
       
        else if (args[0] == "echo") {
            for (size_t i = 1; i < args.size(); ++i) {
              cout << args[i];

              if (i < args.size() - 1) {
                cout << " ";
              }
            }
            cout << endl;
        }

        else if (args[0] == "type") {
          if (args.size() < 2) {
            cout << "type: missing arguements" << endl;
            continue;
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
        else {
            cout << command << ": command not found" << endl; 
        }
    }

    return 0;
}