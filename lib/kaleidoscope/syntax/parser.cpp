#include <kaleidoscope/syntax/parser.hpp>

#include <iostream>
#include <boost/variant.hpp>

namespace kaleidoscope {
    namespace syntax {

        any_expr parser::parse_expression()
        {
            auto prim = parse_primary();
            return parse_binop_rhs(0, std::move(prim));
        }

        any_expr parser::parse_primary()
        {
            if (boost::get<number>(&current_)) {
                return parse_number();
            } else if (boost::get<ident>(&current_)) {
                return parse_identifier();
            } else if (current_ == token(symbol::LPAREN)) {
                return parse_paren_expr();
            } else {
                throw parse_exception{"Unexpected token. Expected primary expression."};
            }
        }

        any_expr parser::parse_number()
        {
            if (!boost::get<number>(&current_)) {
                // current token is not number.
                throw parse_exception{"Unexpected token. Expected number."};
            }
            number const& n = boost::get<number>(current_);
            next();
            return std::make_shared<number_expr>(n.value);
        }

        any_expr parser::parse_paren_expr()
        {
            eat_symbol(symbol::LPAREN);
            auto const expr = parse_expression();
            eat_symbol(symbol::RPAREN);
            return expr;
        }

        any_expr parser::parse_identifier()
        {
            if (!boost::get<ident>(&current_)) {
                throw parse_exception{"Unexpected token. Expected identifier."};
            }
            ident const id = boost::get<ident>(current_);
            next();

            // func(...)
            if (current_ == token(symbol::LPAREN)) {
                eat_symbol(symbol::LPAREN);

                std::vector<any_expr> args;
                while (current_ != token(symbol::RPAREN)) {
                    auto const& expr = parse_expression();
                    args.push_back(expr);

                    if (current_ == token(symbol::RPAREN))
                        break;

                    eat_symbol(symbol::COMMA);
                }
                eat_symbol(symbol::RPAREN);

                return std::make_shared<call_expr>(id.value, args);
            }

            return std::make_shared<variable_expr>(id.value);
        }

        any_expr parser::parse_binop_rhs(int prec, any_expr&& lhs)
        {
            for (;;) {
                if (!boost::get<op>(&current_)) {
                    return lhs;
                }
                op const o = boost::get<op>(current_);
                auto it = binop_precedence_.find(o.value);
                if (it == std::end(binop_precedence_)) {
                    throw parse_exception{"Unknown operator character."};
                }

                int p = binop_precedence_[o.value];
                if (p  < prec)
                    return lhs;

                next();

                any_expr rhs = parse_primary();
                if (boost::get<op>(&current_)) {
                    op const o2 = boost::get<op>(current_);
                    it = binop_precedence_.find(o.value);
                    if (it == std::end(binop_precedence_)) {
                        throw parse_exception{"Unknown operator character."};
                    }

                    int p2 = binop_precedence_[o2.value];
                    if (p < p2) {
                        rhs = parse_binop_rhs(p + 1, std::move(rhs));
                    }
                }
                lhs = std::make_shared<binary_expr>(o.value, lhs, rhs);
            }
        }

        prototype parser::parse_proto()
        {
            if (!boost::get<ident>(&current_)) {
                throw parse_exception{"Unexpected token. Expected identifier."};
            }

            std::string name = boost::get<ident>(current_).value;
            next();

            eat_symbol(symbol::LPAREN);

            std::vector<std::string> args;
            while (boost::get<ident>(&current_)) {
                args.emplace_back(boost::get<ident>(current_).value);
                next();
            }

            eat_symbol(symbol::RPAREN);

            return prototype(name, args);
        }

        func parser::parse_func()
        {
            eat_keyword(keyword::DEF);
            auto proto = parse_proto();
            auto body = parse_expression();
            return func(proto, body);
        }

        prototype parser::parse_extern()
        {
            eat_keyword(keyword::EXTERN);
            return parse_proto();
        }

        func parser::parse_toplevel_expression()
        {
            auto e = parse_expression();
            prototype p("__anon_expr", {});
            return func(p, e);
        }

        void parser::eat_keyword(keyword k)
        {
            if (!boost::get<keyword>(&current_)) {
                throw parse_exception{"Unexpected token. Expected keyword."};
            }
            if (boost::get<keyword>(current_) != k) {
                throw parse_exception{"Unexpected token. Expected keyword."};
            }
            next();
            return;
        }

        void parser::eat_symbol(symbol k)
        {
            if (!boost::get<symbol>(&current_)) {
                throw parse_exception{"Unexpected token. Expected symbol."};
            }
            if (boost::get<symbol>(current_) != k) {
                throw parse_exception{"Unexpected token. Expected symbol."};
            }
            next();
            return;
        }
    } // namespace syntax
} // namespace kaleidoscope
