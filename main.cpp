#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// Variable for determining what we should put for a type when GDScript is unclear
constexpr std::string TYPE_ANY = "unknown";

std::vector<std::string> comments = {};

/** Parses a GDScript into a C++ file to be utilized by Doxygen.
 *  Returns True on success and False on fail.
 */
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

            for (int i = 0; i < comments.size(); i++) {
                size_t pos = comments[i].find("#", 0);
                std::string replaceString = "//!";

                while (pos != std::string::npos) {
                    if (comments[i].starts_with(replaceString)) {
                        replaceString = "";
                    }

                    comments[i].replace(pos, 1, replaceString);
                    pos = comments[i].find("#", pos);
                }
                lineToReturn += comments[i] + "\n";
            }

            // Gets the type
            if (int typeLine = line.find("->"); typeLine != std::string::npos) {
                typeLine += 2;
                std::string typeString = line.substr(typeLine, line.find_last_of(':') - typeLine);
                std::erase_if(typeString, isspace);
                lineToReturn += typeString;
            }
            else {
                // As GDScript doesn't always specify types, we can instead use C++ std::any for now
                lineToReturn += TYPE_ANY;
            }

            // Gets the function name
            if (int typeLine = line.find("func "); typeLine != std::string::npos) {
                typeLine += 5;
                lineToReturn += " " + line.substr(typeLine, line.find("(", 0) - typeLine);
            }

            // Gets the function parameters
            if (int typeLine = line.find("("); typeLine != std::string::npos) {
                typeLine += 1;
                std::string allParams = line.substr(typeLine, line.find(")", 0) - typeLine);
                std::stringstream ss(allParams);
                std::string param;

                lineToReturn += '(';

                while (getline(ss, param, ',')) {
                    if (int paramSeparatorIndex = param.find(':'); paramSeparatorIndex != std::string::npos) {
                        std::string typeString = param.substr(paramSeparatorIndex + 1, param.length() - paramSeparatorIndex - 1);
                        std::erase_if(typeString, isspace);
                        lineToReturn += typeString + " ";

                        typeString = param.substr(0, paramSeparatorIndex);
                        std::erase_if(typeString, isspace);
                        lineToReturn += typeString + ", ";
                    }
                    else {
                        lineToReturn += TYPE_ANY + param + ", ";
                    }
                }

                // Pop the last ", "
                if (lineToReturn.ends_with(", ")) {
                    lineToReturn.pop_back();
                    lineToReturn.pop_back();
                }

                lineToReturn += ')';
            }

            lineToReturn += ";";
            std::cout << lineToReturn << "\n";
        }

        if (line[0] == '#') {
            comments.push_back(line);
        }
        else if (!std::all_of(line.begin(), line.end(), isspace)) {
            comments.erase(comments.begin(), comments.end());
            comments.clear();
        }
    }

    return true;
}

int main(const int argc, const char *argv[]) {
    // Ignore first argv as that's just the executable
    for (int i = 1; i < argc; i++) {
        parseFile(argv[i]);
    }

    return 0;
}