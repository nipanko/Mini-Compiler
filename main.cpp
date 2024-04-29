#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "emit.h"
#include "Parse.h"

int main(int argc, char* argv[]) {
    std::cout << "Teeny Tiny Compiler" << std::endl;

    if (argc != 2) {
        std::cerr << "Error: Compiler needs source file as argument." << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open source file." << std::endl;
        return 1;
    }

    std::string source((std::istreambuf_iterator<char>(inputFile)),
        (std::istreambuf_iterator<char>()));

    // Initialize the lexer, emitter, and parser.
    Lexer lexer(source);
    Emitter emitter("out.c");
    Parser parser(lexer, emitter);

    parser.program(); // Start the parser.
    emitter.writeFile(); // Write the output to file.
    std::cout << "Compiling completed." << std::endl;

    return 0;
}
