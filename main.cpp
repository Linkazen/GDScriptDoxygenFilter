#include <fstream>
#include <iostream>

// Parses a GDScript into a C++ file to be utilized by Doxygen.
// Returns True on success and False on fail.
bool parseFile(const std::string &fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error opening file " << fileName << "\n";
        return false;
    }

    // Parse functions -- TEST
    for (std::string line; std::getline(file, line); ) {
        // Checks if it is a function
        if (line.find("func", 0) == 0) {
            std::string lineToReturn = "";

            // Gets the type. Defaults to "void" for now.
            if (int typeLine = line.find("->"); typeLine != std::string::npos) {
                typeLine += 2;
                std::string typeString = line.substr(typeLine, line.find(":", 0) - typeLine);
                std::erase_if(typeString, isspace);
                lineToReturn += typeString;
            }
            else {
                lineToReturn += "void";
            }

            // We know this is a function, so append "func" here.
            lineToReturn += " func";

            // Gets the function name
            if (int typeLine = line.find("func "); typeLine != std::string::npos) {
                typeLine += 5;
                lineToReturn += " " + line.substr(typeLine, line.find("(", 0) - typeLine);
            }

            // Gets the function parameters
            if (int typeLine = line.find("("); typeLine != std::string::npos) {
                lineToReturn += " " + line.substr(typeLine, line.find(")", 0) - typeLine + 1);
            }

            lineToReturn += ";";
            std::cout << lineToReturn << std::endl;
        }
    }

    return true;
}

int main(const int argc, const char *argv[]) {
    for (int i = 0; i < argc; i++) {
        parseFile(argv[i]);
    }

    return 0;
}