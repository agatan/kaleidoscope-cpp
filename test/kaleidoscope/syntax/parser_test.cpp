#include <boost/test/unit_test.hpp>

#include <kaleidoscope/syntax/ast.hpp>
#include <kaleidoscope/syntax/lexer.hpp>
#include <kaleidoscope/syntax/parser.hpp>

namespace  {

    using namespace kaleidoscope::syntax;

    BOOST_AUTO_TEST_SUITE(parser_test)

        BOOST_AUTO_TEST_CASE(primary)
        {
            {
                parser p(lexer("123.0"));
                any_expr expr = p.parse_expression();
                BOOST_CHECK(*boost::get<number_expr_ptr>(expr) == number_expr(123.0));
            }

            {
                auto p = parser(lexer("name"));
                auto expr = p.parse_expression();
                BOOST_CHECK(*boost::get<variable_expr_ptr>(expr) == variable_expr("name"));
            }

            {
                parser p(lexer("(123.0)"));
                any_expr expr = p.parse_expression();
                BOOST_CHECK(*boost::get<number_expr_ptr>(expr) == number_expr(123.0));
            }
        }

        BOOST_AUTO_TEST_CASE(binop1)
        {
            parser p(lexer("1 + 2 * 3"));
            p.register_binop('+', 1);
            p.register_binop('*', 2);
            any_expr expr = p.parse_expression();
            auto b = boost::get<binary_expr_ptr>(expr);
            BOOST_CHECK('+' == b->op());
            auto lhs = boost::get<number_expr_ptr>(b->lhs());
            BOOST_CHECK(*lhs == number_expr(1));
            auto rhs = boost::get<binary_expr_ptr>(b->rhs());
            BOOST_CHECK('*' == rhs->op());
            auto rlhs = boost::get<number_expr_ptr>(rhs->lhs());
            auto rrhs = boost::get<number_expr_ptr>(rhs->rhs());
            BOOST_CHECK(number_expr(2) == *rlhs);
            BOOST_CHECK(number_expr(3) == *rrhs);
        }

        BOOST_AUTO_TEST_CASE(binop2)
        {
            parser p(lexer("1 * 2 + 3"));
            p.register_binop('+', 1);
            p.register_binop('*', 2);
            any_expr expr = p.parse_expression();
            auto b = boost::get<binary_expr_ptr>(expr);
            BOOST_CHECK('+' == b->op());
            auto rhs = boost::get<number_expr_ptr>(b->rhs());
            BOOST_CHECK(*rhs == number_expr(3));
            auto lhs = boost::get<binary_expr_ptr>(b->lhs());
            BOOST_CHECK('*' == lhs->op());
            auto llhs = boost::get<number_expr_ptr>(lhs->lhs());
            auto lrhs = boost::get<number_expr_ptr>(lhs->rhs());
            BOOST_CHECK(number_expr(1) == *llhs);
            BOOST_CHECK(number_expr(2) == *lrhs);
        }

        BOOST_AUTO_TEST_CASE(proto)
        {
            parser p(lexer("name(a b c)"));
            auto proto = p.parse_proto();
            BOOST_CHECK(prototype("name", {"a", "b", "c"}) == proto);
        }

        BOOST_AUTO_TEST_CASE(function)
        {
            parser p(lexer("def add(a b) a + b"));
            p.register_binop('+', 1);
            auto f = p.parse_func();
            BOOST_CHECK(prototype("add", {"a", "b"}) == f.proto());
            auto b = boost::get<binary_expr_ptr>(f.body());
            BOOST_CHECK(b->op() == '+');
            auto lhs = boost::get<variable_expr_ptr>(b->lhs());
            auto rhs = boost::get<variable_expr_ptr>(b->rhs());
            BOOST_CHECK("a" == lhs->name());
            BOOST_CHECK("b" == rhs->name());
        }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace 
