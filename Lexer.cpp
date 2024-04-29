#include "Lexer.h"
#include <iostream>



// Token class constructor
Token::Token(const std::string& tokenText, TokenType tokenKind) : text(tokenText), kind(tokenKind) {}

// Method to check if a token is a keyword
TokenType Token::checkIfKeyword(const std::string& tokenText) {
    // Your implementation to check if tokenText is a keyword goes here
    // You can use a map, switch-case, or any other method to determine if tokenText is a keyword
    return TokenType::IDENT; // Placeholder return value
}

// Lexer class constructor
Lexer::Lexer(const std::string& source) : source(source + '\n'), curPos(-1) {
    nextChar();
}

// Method to advance to the next character in the source code
void Lexer::nextChar() {
    curPos++;
    if (curPos >= source.length()) {
        curChar = '\0';  // EOF
    }
    else {
        curChar = source[curPos];
    }
}

// Method to peek at the next character in the source code
char Lexer::peek() const {
    if (curPos + 1 >= source.length()) {
        return '\0';
    }
    return source[curPos + 1];
}

// Method to abort lexing with an error message
void Lexer::abort(const std::string& message) const {
    std::cerr << "Lexing error. " << message << std::endl;
    exit(EXIT_FAILURE);
}

// Method to get the next token from the source code
Token Lexer::getToken() {
    skipWhitespace();
    skipComment();
    Token token;


    // Your tokenization logic goes here
    // Implement tokenization based on the Python code provided

    return token; // Placeholder return value
}

// Method to skip whitespace characters in the source code
void Lexer::skipWhitespace() {
    while (curChar == ' ' || curChar == '\t' || curChar == '\r') {
        nextChar();
    }
}

// Method to skip comments in the source code
void Lexer::skipComment() {
    if (curChar == '#') {
        while (curChar != '\n') {
            nextChar();
        }
    }
}
