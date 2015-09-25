#pragma once

#include <iostream>
#include <map>

#include <kaleidoscope/syntax/ast.hpp>
#include <kaleidoscope/syntax/lexer.hpp>

namespace kaleidoscope {
    namespace syntax {

        struct parse_exception {
            std::string message;
        };

        class parser {
        public:
            parser(lexer&& l) : lex_(std::move(l))
            {
                current_ = lex_.next();
            }

            parser(lexer const& l) : lex_(l)
            {
                current_ = lex_.next();
            }

            token const& peek() const noexcept { return current_; }

            void next() noexcept
            {
                current_ = lex_.next();
            }

            any_expr parse_expression();
            any_expr parse_primary();
            any_expr parse_number();
            any_expr parse_paren_expr();
            any_expr parse_identifier();
            any_expr parse_binop_rhs(int, any_expr&&);

            prototype parse_proto();
            func parse_func();

            void register_binop(char op, int prec)
            {
                binop_precedence_.insert(std::make_pair(op, prec));
            }

        private:
            lexer lex_;
            token current_;
            std::map<char, int> binop_precedence_;

            void eat_keyword(keyword);
            void eat_symbol(symbol);
        };

    } // namespace syntax
} // namespace kaleidoscope
