/*
 * main.cpp
 * */

#include <iostream>

#include <kaleidoscope/syntax/ast.hpp>
#include <kaleidoscope/syntax/lexer.hpp>
#include <kaleidoscope/syntax/parser.hpp>

void handle_definition(kaleidoscope::syntax::parser&);
void handle_extern(kaleidoscope::syntax::parser&);
void handle_toplevel_expression(kaleidoscope::syntax::parser&);

int main(const int, const char* const []) {
    kaleidoscope::syntax::parser p(kaleidoscope::syntax::lexer(""));
    p.register_binop('<', 10);
    p.register_binop('+', 20);
    p.register_binop('-', 20);
    p.register_binop('*', 40);

    for (;;) {
        std::cerr << "ready> ";
        std::string line;
        if (!std::getline(std::cin, line)) {
            std::cerr << "no input\n";
            break;
        }
        p.change_source(kaleidoscope::syntax::lexer(line));

        auto tok = p.peek();
        if (boost::get<kaleidoscope::syntax::keyword>(&tok)) {
            auto kwd = boost::get<kaleidoscope::syntax::keyword>(tok);
            if (kwd == kaleidoscope::syntax::keyword::DEF) {
                handle_definition(p);
            } else if (kwd == kaleidoscope::syntax::keyword::EXTERN) {
                handle_extern(p);
            } else {
                std::cerr << static_cast<long>(kwd) << std::endl;
                return 0;
            }
        } else {
            handle_toplevel_expression(p);
        }
    }

    return 0;
}


void handle_definition(kaleidoscope::syntax::parser& p)
{
    try {
        p.parse_func();
        std::cout << "Parsed a function definition." << std::endl;
    } catch (kaleidoscope::syntax::parse_exception e) {
        std::cerr << "Parse error... while parsing a function definition.\n";
    }
}

void handle_extern(kaleidoscope::syntax::parser& p)
{
    try {
        p.parse_extern();
        std::cout << "Parsed an extern." << std::endl;
    } catch (kaleidoscope::syntax::parse_exception e) {
        std::cerr << "Parse error... while parsing an extern.\n";
    }
}

void handle_toplevel_expression(kaleidoscope::syntax::parser& p)
{
    try {
        p.parse_toplevel_expression();
        std::cout << "Parsed an expression." << std::endl;
    } catch (kaleidoscope::syntax::parse_exception e) {
        std::cerr << "Parse error... while parsing an expression.\n";
    }
}
