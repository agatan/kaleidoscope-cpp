#include <cctype>

#include <kaleidoscope/lexer.h>

namespace kaleidoscope {
    namespace syntax {

        token lexer::next() {
            skip_ws();
            if (is_eof())
                return keyword::EOF_;

            auto const c = peek();

            // handle symbols
            switch (c) {
            case '(':
                next_char();
                return symbol::LPAREN;
            case ')':
                next_char();
                return symbol::RPAREN;
            case '{':
                next_char();
                return symbol::LBRACE;
            case '}':
                next_char();
                return symbol::RBRACE;
            case ',':
                next_char();
                return symbol::COMMA;
            }

            if (std::isalpha(c)) {
                return parse_ident();
            } else if (std::isdigit(c)) {
                return parse_num();
            }
            return unknown{};
        }

        void lexer::skip_ws() {
            while(std::isblank(peek())) {
                next_char();
                if (is_eof())
                    break;
            }
        }

        token lexer::parse_ident() {
            std::string buf;

            char c;
            while(std::isalnum(c = peek()) || c == '_') {
                buf += c;
                next_char();
                if (is_eof())
                    break;
            }

            if (buf == "def")
                return keyword::DEF;
            else if (buf == "extern")
                return keyword::EXTERN;

            return ident { buf };
        }

        token lexer::parse_num()
        {
            std::string buf;

            char c;
            while(std::isdigit(c = peek())) {
                buf += c;
                next_char();
                if (is_eof())
                    break;
            }
            if (c == '.') {
                buf += c;
                next_char();
                if (is_eof())
                    return unknown{};
            }
            while(std::isdigit(c = peek())) {
                buf += c;
                next_char();
                if (is_eof())
                    break;
            }

            return token(number{std::stod(buf)});
        }

    } // namespace syntax
} // namespace kaleidoscope
