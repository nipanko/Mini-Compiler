#pragma once
#include <iostream>
#include <fstream>
#include <string>

class Emitter {
private:
    std::string fullPath;
    std::string header;
    std::string code;

public:
    Emitter(const std::string& fullPath) : fullPath(fullPath), header(""), code("") {}

    void emit(const std::string& code) {
        this->code += code;
    }

    void emitLine(const std::string& code) {
        this->code += code + "\n";
    }

    void headerLine(const std::string& code) {
        this->header += code + "\n";
    }

    void writeFile() {
        std::ofstream outputFile(fullPath);
        if (outputFile.is_open()) {
            outputFile << header << code;
            outputFile.close();
        }
        else {
            std::cerr << "Unable to open file: " << fullPath << std::endl;
        }
    }
};
