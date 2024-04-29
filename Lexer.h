#pragma once

#include <string>
#include <cstdlib> // For exit

// Define TokenType enum
enum class TokenType {
    EOF_TOKEN = -1,
    NEWLINE = 0,
    NUMBER = 1,
    IDENT = 2,
    STRING = 3,
    // Keywords
    LABEL = 101,
    GOTO = 102,
    PRINT = 103,
    INPUT = 104,
    LET = 105,
    IF = 106,
    THEN = 107,
    ENDIF = 108,
    WHILE = 109,
    REPEAT = 110,
    ENDWHILE = 111,
    // Operators
    EQ = 201,
    PLUS = 202,
    MINUS = 203,
    ASTERISK = 204,
    SLASH = 205,
    EQEQ = 206,
    NOTEQ = 207,
    LT = 208,
    LTEQ = 209,
    GT = 210,
    GTEQ = 211

    
};
 // Array to map TokenType values to their string representations
// Define Token class



class Token {
public:
    Token() : text(""), kind(TokenType::EOF_TOKEN) {}
    Token(const std::string& tokenText, TokenType tokenKind);

    std::string text; // The token's actual text. Used for identifiers, strings, and numbers.
    TokenType kind;   // The TokenType that this token is classified as.

    static TokenType checkIfKeyword(const std::string& tokenText);

};
// Default constructor for Token class


// Define Lexer class
class Lexer {
public:
    Lexer(const std::string& source);

    void nextChar();
    char peek() const;
    void abort(const std::string& message) const;
    Token getToken();

private:
    std::string source; // Source code to lex as a string. Append a newline to simplify lexing/parsing the last token/statement.
    char curChar;       // Current character in the string.
    int curPos;         // Current position in the string.

    void skipWhitespace();
    void skipComment();
};
