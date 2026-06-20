#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {

    if (input == "exit") {
      break;
    }
    std::cout << "$ ";
    std::string command;
    std::string input;
    std::getline(std::cin, command);
    std::cout << command << ": command not found" << std::endl; 
  }

}
