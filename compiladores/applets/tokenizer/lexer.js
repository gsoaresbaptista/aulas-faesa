const TokenType = {
    // Delimiters
    LPAREN: 'LPAREN', RPAREN: 'RPAREN',
    LBRACE: 'LBRACE', RBRACE: 'RBRACE',
    COLON: 'COLON', SEMICOLON: 'SEMICOLON', COMMA: 'COMMA',

    // Operators
    PLUS: 'PLUS', MINUS: 'MINUS', STAR: 'STAR', SLASH: 'SLASH',
    ASSIGN: 'ASSIGN', EQ: 'EQ', BANG_EQ: 'BANG_EQ',
    GT: 'GT', LT: 'LT', GT_EQ: 'GT_EQ', LT_EQ: 'LT_EQ',
    ARROW: 'ARROW', AND: 'AND', OR: 'OR', NOT: 'NOT',

    // Literals
    CHAR: 'CHAR', NUMBER: 'NUMBER', STRING: 'STRING', IDENTIFIER: 'IDENTIFIER',

    // Keywords
    KW_INT: 'INT', KW_FLOAT: 'FLOAT', KW_VOID: 'VOID', KW_BOOL: 'BOOL',
    KW_CHAR: 'CHAR_KW', KW_STRING: 'STRING_KW', KW_IF: 'IF', KW_ELSE: 'ELSE',
    KW_LOOP: 'LOOP', KW_RETURN: 'RETURN', KW_FN: 'FN',
    KW_TRUE: 'TRUE', KW_FALSE: 'FALSE',

    ERROR: 'ERROR',
    EOF: 'EOF'
};

const Keywords = {
    'int': TokenType.KW_INT,
    'float': TokenType.KW_FLOAT,
    'void': TokenType.KW_VOID,
    'bool': TokenType.KW_BOOL,
    'char': TokenType.KW_CHAR,
    'string': TokenType.KW_STRING,
    'if': TokenType.KW_IF,
    'else': TokenType.KW_ELSE,
    'loop': TokenType.KW_LOOP,
    'return': TokenType.KW_RETURN,
    'fn': TokenType.KW_FN,
    'true': TokenType.KW_TRUE,
    'false': TokenType.KW_FALSE,
    'and': TokenType.AND,
    'or': TokenType.OR,
    'not': TokenType.NOT
};

class Lexer {
    constructor(source) {
        this.source = source;
        this.start = 0;
        this.current = 0;
        this.line = 1;
    }

    isAtEnd() {
        return this.current >= this.source.length;
    }

    advance() {
        return this.source[this.current++];
    }

    peek() {
        if (this.isAtEnd()) return '\0';
        return this.source[this.current];
    }

    peekNext() {
        if (this.current + 1 >= this.source.length) return '\0';
        return this.source[this.current + 1];
    }

    match(expected) {
        if (this.isAtEnd()) return false;
        if (this.source[this.current] !== expected) return false;
        this.current++;
        return true;
    }

    makeToken(type) {
        return {
            type: type,
            lexeme: this.source.substring(this.start, this.current),
            line: this.line
        };
    }

    errorToken(message) {
        return {
            type: TokenType.ERROR,
            lexeme: message,
            line: this.line
        };
    }

    skipWhitespace() {
        while (true) {
            let c = this.peek();
            switch (c) {
                case ' ':
                case '\r':
                case '\t':
                    this.advance();
                    break;
                case '\n':
                    this.line++;
                    this.advance();
                    break;
                case '/':
                    if (this.peekNext() === '/') {
                        while (this.peek() !== '\n' && !this.isAtEnd()) this.advance();
                    } else if (this.peekNext() === '*') {
                        this.advance(); // /
                        this.advance(); // *
                        while (!this.isAtEnd()) {
                            if (this.peek() === '\n') this.line++;
                            if (this.peek() === '*' && this.peekNext() === '/') {
                                this.advance();
                                this.advance();
                                break;
                            }
                            this.advance();
                        }
                    } else {
                        return;
                    }
                    break;
                default:
                    return;
            }
        }
    }

    identifier() {
        while (this.isAlpha(this.peek()) || this.isDigit(this.peek())) this.advance();
        let text = this.source.substring(this.start, this.current);
        let type = Keywords[text] || TokenType.IDENTIFIER;
        return this.makeToken(type);
    }

    number() {
        while (this.isDigit(this.peek())) this.advance();
        if (this.peek() === '.' && this.isDigit(this.peekNext())) {
            this.advance();
            while (this.isDigit(this.peek())) this.advance();
        }
        return this.makeToken(TokenType.NUMBER);
    }

    string() {
        while (this.peek() !== '"' && !this.isAtEnd()) {
            if (this.peek() === '\n') this.line++;
            this.advance();
        }
        if (this.isAtEnd()) return this.errorToken("String não terminada.");
        this.advance(); // closing "
        return this.makeToken(TokenType.STRING);
    }

    character() {
        if (this.isAtEnd()) return this.errorToken("Caractere não terminado.");
        this.advance(); // the char itself
        if (this.peek() !== '\'') return this.errorToken("Esperado ' fechando o literal.");
        this.advance(); // closing '
        return this.makeToken(TokenType.CHAR);
    }

    isAlpha(c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c === '_';
    }

    isDigit(c) {
        return c >= '0' && c <= '9';
    }

    nextToken() {
        this.skipWhitespace();
        this.start = this.current;

        if (this.isAtEnd()) return this.makeToken(TokenType.EOF);

        let c = this.advance();

        if (this.isAlpha(c)) return this.identifier();
        if (this.isDigit(c)) return this.number();

        switch (c) {
            case '(': return this.makeToken(TokenType.LPAREN);
            case ')': return this.makeToken(TokenType.RPAREN);
            case '{': return this.makeToken(TokenType.LBRACE);
            case '}': return this.makeToken(TokenType.RBRACE);
            case ';': return this.makeToken(TokenType.SEMICOLON);
            case ',': return this.makeToken(TokenType.COMMA);
            case ':': return this.makeToken(TokenType.COLON);
            case '+': return this.makeToken(TokenType.PLUS);
            case '*': return this.makeToken(TokenType.STAR);
            case '/': return this.makeToken(TokenType.SLASH);
            case '-':
                return this.makeToken(this.match('>') ? TokenType.ARROW : TokenType.MINUS);
            case '!':
                return this.makeToken(this.match('=') ? TokenType.BANG_EQ : TokenType.NOT);
            case '=':
                return this.makeToken(this.match('=') ? TokenType.EQ : TokenType.ASSIGN);
            case '<':
                return this.makeToken(this.match('=') ? TokenType.LT_EQ : TokenType.LT);
            case '>':
                return this.makeToken(this.match('=') ? TokenType.GT_EQ : TokenType.GT);
            case '&':
                if (this.match('&')) return this.makeToken(TokenType.AND);
                return this.errorToken("Esperado '&' após '&'");
            case '|':
                if (this.match('|')) return this.makeToken(TokenType.OR);
                return this.errorToken("Esperado '|' após '|'");
            case '"': return this.string();
            case '\'': return this.character();
        }

        return this.errorToken("Caractere inesperado.");
    }

    scanAllTokens() {
        let tokens = [];
        while (true) {
            let token = this.nextToken();
            tokens.push(token);
            if (token.type === TokenType.EOF) break;
        }
        return tokens;
    }
}
