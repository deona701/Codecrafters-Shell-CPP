#include <iostream>
#include <string>
#include <stdbool.h>
#include <vector>
using namespace std;

int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;


  while (true) {
    cout << "$ ";
    string command;
    getline(cin, command);
    vector<string> commands = {"exit", "echo", "type"};

    if (command == "exit") {
      break;
    }

    else if (command.substr(0, 5) == "echo ") {
      cout << command.substr(5) << endl;
    }

    else if (command.substr(0, 5) == "type ") {
    string requested_command = command.substr(5);
    bool found = false;

    for (string builtin : commands) {
      if (requested_command == builtin) {
        cout << builtin << " is a shell builtin" << endl;
        found = true;
        break; 
      }
    }

    if (!found) {
      cout << requested_command << ": not found" << endl;
    }
  }

    else {
      cout << command << ": command not found" << endl; 
    }
  }

}
