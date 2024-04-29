#include "Parse.h"

Parser::Parser(Lexer& lexer, Emitter& emitter)
    : lexer(lexer), emitter(emitter) {
    nextToken();
    nextToken();
}

void Parser::program() {
    emitter.headerLine("#include <stdio.h>");
    emitter.headerLine("int main(void) {");

    while (checkToken(TokenType::NEWLINE)) {
        nextToken();
    }

    while (!checkToken(TokenType::EOF_TOKEN)) {
        statement();
    }


    emitter.emitLine("return 0;");
    emitter.emitLine("}");

    for (const auto& label : labelsGotoed) {
        if (labelsDeclared.find(label) == labelsDeclared.end()) {
            abort("Attempting to GOTO undeclared label: " + label);
        }
    }
}

void Parser::nextToken() {
    curToken = peekToken;
    peekToken = lexer.getToken();
}

bool Parser::checkToken(TokenType kind) const {
    return kind == curToken.kind;
}

bool Parser::checkPeek(TokenType kind) const {
    return kind == peekToken.kind;
}

void Parser::match(TokenType kind) {
    if (!checkToken(kind)) {
        abort("ERROOR");
    }
    nextToken();
}

bool Parser::isComparisonOperator() const {
    return checkToken(TokenType::GT) || checkToken(TokenType::GTEQ) || checkToken(TokenType::LT) ||
        checkToken(TokenType::LTEQ) || checkToken(TokenType::EQEQ) || checkToken(TokenType::NOTEQ);
}

void Parser::abort(const std::string& message) {
    std::cerr << "Error! " << message << std::endl;
    std::exit(EXIT_FAILURE);
}

void Parser::statement() {
    if (checkToken(TokenType::PRINT)) {
        nextToken();

        if (checkToken(TokenType::STRING)) {
            emitter.emitLine("printf(\"" + curToken.text + "\\n\");");
            nextToken();
        }
        else {
            emitter.emit("printf(\"%" ".2f\\n\", (float)(");
            expression();
            emitter.emitLine("));");
        }
    }
    else if (checkToken(TokenType::IF)) {
        nextToken();
        emitter.emit("if(");
        comparison();

        match(TokenType::THEN);
        nl();
        emitter.emitLine("){");

        while (!checkToken(TokenType::ENDIF)) {
            statement();
        }

        match(TokenType::ENDIF);
        emitter.emitLine("}");
    }
    else if (checkToken(TokenType::WHILE)) {
        nextToken();
        emitter.emit("while(");
        comparison();

        match(TokenType::REPEAT);
        nl();
        emitter.emitLine("){");

        while (!checkToken(TokenType::ENDWHILE)) {
            statement();
        }

        match(TokenType::ENDWHILE);
        emitter.emitLine("}");
    }
    else if (checkToken(TokenType::LABEL)) {
        nextToken();

        if (labelsDeclared.find(curToken.text) != labelsDeclared.end()) {
            abort("Label already exists: " + curToken.text);
        }

        labelsDeclared.insert(curToken.text);
        emitter.emitLine(curToken.text + ":");
        match(TokenType::IDENT);
    }
    else if (checkToken(TokenType::GOTO)) {
        nextToken();
        labelsGotoed.insert(curToken.text);
        emitter.emitLine("goto " + curToken.text + ";");
        match(TokenType::IDENT);
    }
    else if (checkToken(TokenType::LET)) {
        nextToken();

        if (symbols.find(curToken.text) == symbols.end()) {
            symbols.insert(curToken.text);
            emitter.headerLine("float " + curToken.text + ";");
        }

        emitter.emit(curToken.text + " = ");
        match(TokenType::IDENT);
        match(TokenType::EQ);
        expression();
        emitter.emitLine(";");
    }
    else if (checkToken(TokenType::INPUT)) {
        nextToken();

        if (symbols.find(curToken.text) == symbols.end()) {
            symbols.insert(curToken.text);
            emitter.headerLine("float " + curToken.text + ";");
        }

        emitter.emitLine("if(0 == scanf(\"%lf\", &" + curToken.text + ")) {");
        emitter.emitLine(curToken.text + " = 0;");
        emitter.emit("scanf(\"%");
        emitter.emitLine("*s\");");
        emitter.emitLine("}");
        match(TokenType::IDENT);
    }
    else {
        abort("Invalid statement at " + curToken.text + " (" + ")");
    }

    nl();
}

void Parser::comparison() {
    expression();
    if (isComparisonOperator()) {
        emitter.emit(curToken.text);
        nextToken();
        expression();
    }
    while (isComparisonOperator()) {
        emitter.emit(curToken.text);
        nextToken();
        expression();
    }
}

void Parser::expression() {
    term();
    while (checkToken(TokenType::PLUS) || checkToken(TokenType::MINUS)) {
        emitter.emit(curToken.text);
        nextToken();
        term();
    }
}

void Parser::term() {
    unary();
    while (checkToken(TokenType::ASTERISK) || checkToken(TokenType::SLASH)) {
        emitter.emit(curToken.text);
        nextToken();
        unary();
    }
}

void Parser::unary() {
    if (checkToken(TokenType::PLUS) || checkToken(TokenType::MINUS)) {
        emitter.emit(curToken.text);
        nextToken();
    }
    primary();
}

void Parser::primary() {
    if (checkToken(TokenType::NUMBER)) {
        emitter.emit(curToken.text);
        nextToken();
    }
    else if (checkToken(TokenType::IDENT)) {
        if (symbols.find(curToken.text) == symbols.end()) {
            abort("Referencing variable before assignment: " + curToken.text);
        }

        emitter.emit(curToken.text);
        nextToken();
    }
    else {
        abort("Unexpected token at " + curToken.text);
    }
}

void Parser::nl() {
    match(TokenType::NEWLINE);
    while (checkToken(TokenType::NEWLINE)) {
        nextToken();
    }
}
