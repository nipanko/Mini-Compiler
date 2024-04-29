#pragma once

#include "Lexer.h"
#include "emit.h"
#include <set>
#include <string>
#include <cstdlib>



class Parser {
public:
    Parser(Lexer& lexer, Emitter& emitter);

    void program();

private:
    Lexer& lexer;
    Emitter& emitter;

    std::set<std::string> symbols;
    std::set<std::string> labelsDeclared;
    std::set<std::string> labelsGotoed;

    Token curToken;
    Token peekToken;

    void nextToken();
    bool checkToken(TokenType kind) const;
    bool checkPeek(TokenType kind) const;
    void match(TokenType kind);
    void comparison();
    void statement();
    void expression();
    void term();
    void unary();
    void primary();
    void nl();
    bool isComparisonOperator() const;
    void abort(const std::string& message);
    static const std::string tokenNames[];

};

// Define tokenNames array outside of the class
const std::string Parser::tokenNames[] = {
    "EOF_TOKEN",
    "NEWLINE",
    "NUMBER",
    "IDENT",
    "STRING",
    // Keywords
    "LABEL",
    "GOTO",
    "PRINT",
    "INPUT",
    "LET",
    "IF",
    "THEN",
    "ENDIF",
    "WHILE",
    "REPEAT",
    "ENDWHILE",
    // Operators
    "EQ",
    "PLUS",
    "MINUS",
    "ASTERISK",
    "SLASH",
    "EQEQ",
    "NOTEQ",
    "LT",
    "LTEQ",
    "GT",
    "GTEQ"
};



//
//
//#include <iostream>
//#include <string>
//#include <set> // For std::set
//#include "Lexer.h" // Include header file for Token and TokenType
//
//using namespace std;
//
//class Parser {
//private:
//    Lexer lexer;
//    Emitter emitter;
//    set<string> symbols;       // All variables we have declared so far
//    set<string> labelsDeclared; // Keep track of all labels declared
//    set<string> labelsGotoed;   // All labels goto'ed, so we know if they exist or not
//    Token curToken;
//    Token peekToken;
//
//    // Return true if the current token matches
//    bool checkToken(TokenType kind) {
//        return kind == curToken.kind;
//    }
//
//    // Return true if the next token matches
//    bool checkPeek(TokenType kind) {
//        return kind == peekToken.kind;
//    }
//
//    // Try to match current token. If not, error. Advances the current token.
//    void match(TokenType kind) {
//        if (!checkToken(kind)) {
//            abort("Expected " + tokenToString(kind) + ", got " + tokenToString(curToken.kind));
//        }
//        nextToken();
//    }
//
//    // Advances the current token.
//    void nextToken() {
//        curToken = peekToken;
//        peekToken = lexer.getToken();
//    }
//
//    // Aborts the program with an error message.
//    void abort(const string& message) {
//        cerr << "Error! " << message << endl;
//        exit(EXIT_FAILURE);
//    }
//
//public:
//    Parser(Lexer lex, Emitter emit) : lexer(lex), emitter(emit) {
//        nextToken();
//        nextToken(); // Call this twice to initialize current and peek.
//    }
//
//    // Production rules.
//
//    // program ::= {statement}
//    void program() {
//        emitter.headerLine("#include <stdio.h>");
//        emitter.headerLine("int main(void){");
//
//        // Since some newlines are required in our grammar, need to skip the excess.
//        while (checkToken(TokenType::NEWLINE)) {
//            nextToken();
//        }
//
//        // Parse all the statements in the program.
//        while (!checkToken(TokenType::EOF)) {
//            statement();
//        }
//
//        // Wrap things up.
//        emitter.emitLine("return 0;");
//        emitter.emitLine("}");
//
//        // Check that each label referenced in a GOTO is declared.
//        for (const auto& label : labelsGotoed) {
//            if (labelsDeclared.find(label) == labelsDeclared.end()) {
//                abort("Attempting to GOTO to undeclared label: " + label);
//            }
//        }
//    }
//
//    // One of the following statements...
//    void statement() {
//        // Check the first token to see what kind of statement this is.
//
//        // "PRINT" (expression | string)
//        if (checkToken(TokenType::PRINT)) {
//            nextToken();
//
//            if (checkToken(TokenType::STRING)) {
//                // Simple string, so print it.
//                emitter.emitLine("printf(\"" + curToken.text + "\\n\");");
//                nextToken();
//            }
//            else {
//                // Expect an expression and print the result as a float.
//                emitter.emit("printf(\"%" + ".2f\\n\", (float)(");
//                expression();
//                emitter.emitLine("));");
//            }
//        }
//
//        // "IF" comparison "THEN" block "ENDIF"
//        else if (checkToken(TokenType::IF)) {
//            nextToken();
//            emitter.emit("if(");
//            comparison();
//
//            match(TokenType::THEN);
//            emitter.emitLine("){");
//
//            // Zero or more statements in the body.
//            while (!checkToken(TokenType::ENDIF)) {
//                statement();
//            }
//
//            match(TokenType::ENDIF);
//            emitter.emitLine("}");
//        }
//
//        // "WHILE" comparison "REPEAT" block "ENDWHILE"
//        else if (checkToken(TokenType::WHILE)) {
//            nextToken();
//            emitter.emit("while(");
//            comparison();
//
//            match(TokenType::REPEAT);
//            emitter.emitLine("){");
//
//            // Zero or more statements in the loop body.
//            while (!checkToken(TokenType::ENDWHILE)) {
//                statement();
//            }
//
//            match(TokenType::ENDWHILE);
//            emitter.emitLine("}");
//        }
//
//        // "LABEL" ident
//        else if (checkToken(TokenType::LABEL)) {
//            nextToken();
//
//            // Make sure this label doesn't already exist.
//            if (labelsDeclared.find(curToken.text) != labelsDeclared.end()) {
//                abort("Label already exists: " + curToken.text);
//            }
//            labelsDeclared.insert(curToken.text);
//
//            emitter.emitLine(curToken.text + ":");
//            match(TokenType::IDENT);
//        }
//
//        // "GOTO" ident
//        else if (checkToken(TokenType::GOTO)) {
//            nextToken();
//            labelsGotoed.insert(curToken.text);
//            emitter.emitLine("goto " + curToken.text + ";");
//            match(TokenType::IDENT);
//        }
//
//        // "LET" ident = expression
//        else if (checkToken(TokenType::LET)) {
//            nextToken();
//
//            // Check if ident exists in symbol table. If not, declare it.
//            if (symbols.find(curToken.text) == symbols.end()) {
//                symbols.insert(curToken.text);
//                emitter.headerLine("float " + curToken.text + ";");
//            }
//
//            emitter.emit(curToken.text + " = ");
//            match(TokenType::IDENT);
//            match(TokenType::EQ);
//
//            expression();
//            emitter.emitLine(";");
//        }
//
//        // "INPUT" ident
//        else if (checkToken(TokenType::INPUT)) {
//            nextToken();
//
//            // If variable doesn't already exist, declare it.
//            if (symbols.find(curToken.text) == symbols.end()) {
//                symbols.insert(curToken.text);
//                emitter.headerLine("float " + curToken.text + ";");
//            }
//
//            // Emit scanf but also validate the input. If invalid, set the variable to 0 and clear the input.
//            emitter.emitLine("if(0 == scanf(\"%" + "f\", &" + curToken.text + ")) {");
//            emitter.emitLine(curToken.text + " = 0;");
//            emitter.emit("scanf(\"%");
//            emitter.emitLine("*s\");");
//            emitter.emitLine("}");
//            match(TokenType::IDENT);
//        }
//
//        // This is not a valid statement. Error!
//        else {
//            abort("Invalid statement at " + curToken.text + " (" + tokenToString(curToken.kind) + ")");
//        }
//
//        // Newline.
//        match(TokenType::NEWLINE);
//    }
//
//    // comparison ::= expression (("==" | "!=" | ">" | ">=" | "<" | "<=") expression)+
//    void comparison() {
//        expression();
//        // Must be at least one comparison operator and another expression.
//        if (isComparisonOperator(curToken.kind)) {
//            emitter.emit(curToken.text);
//            nextToken();
//            expression();
//        }
//        // Can have 0 or more comparison operator and expressions.
//        while (isComparisonOperator(curToken.kind)) {
//            emitter.emit(curToken.text);
//            nextToken();
//            expression();
//        }
//    }
//
//    // expression ::= term {( "-" | "+" ) term}
//    void expression() {
//        term();
//        // Can have 0 or more +/- and expressions.
//        while (checkToken(TokenType::PLUS) || checkToken(TokenType::MINUS)) {
//            emitter.emit(curToken.text);
//            nextToken();
//            term();
//        }
//    }
//
//    // term ::= unary {( "/" | "*" ) unary}
//    void term() {
//        unary();
//        // Can have 0 or more *// and expressions.
//        while (checkToken(TokenType::ASTERISK) || checkToken(TokenType::SLASH)) {
//            emitter.emit(curToken.text);
//            nextToken();
//            unary();
//        }
//    }
//
//    // unary ::= ["+" | "-"] primary
//    void unary() {
//        // Optional unary +/-
//        if (checkToken(TokenType::PLUS) || checkToken(TokenType::MINUS)) {
//            emitter.emit(curToken.text);
//            nextToken();
//        }
//        primary();
//    }
//
//    // primary ::= number | ident
//    void primary() {
//        if (checkToken(TokenType::NUMBER)) {
//            emitter.emit(curToken.text);
//            nextToken();
//        }
//        else if (checkToken(TokenType::IDENT)) {
//            // Ensure the variable already exists.
//            if (symbols.find(curToken.text) == symbols.end()) {
//                abort("Referencing variable before assignment: " + curToken.text);
//            }
//
//            emitter.emit(curToken.text);
//            nextToken();
//        }
//        else {
//            // Error!
//            abort("Unexpected token at " + curToken.text);
//        }
//    }
//};
