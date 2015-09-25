#include <boost/test/unit_test.hpp>

#include <kaleidoscope/syntax/lexer.hpp>

namespace  {

    using namespace kaleidoscope::syntax;

    BOOST_AUTO_TEST_SUITE(lexer_test)

        BOOST_AUTO_TEST_CASE(keywords_test)
        {
            auto l = kaleidoscope::syntax::lexer("def extern ident");
            BOOST_CHECK(kaleidoscope::syntax::token(kaleidoscope::syntax::keyword::DEF) == l.next());
            BOOST_CHECK(kaleidoscope::syntax::token(kaleidoscope::syntax::keyword::EXTERN) == l.next());
            BOOST_CHECK(kaleidoscope::syntax::token(kaleidoscope::syntax::ident{"ident"}) == l.next());
        }

        BOOST_AUTO_TEST_CASE(num_test)
        {
            auto l = kaleidoscope::syntax::lexer("12.3 ");
            BOOST_CHECK(kaleidoscope::syntax::token(kaleidoscope::syntax::number{12.3}) == l.next());
            BOOST_CHECK(kaleidoscope::syntax::token(kaleidoscope::syntax::keyword::EOF_) == l.next());
        }

        BOOST_AUTO_TEST_CASE(symbol_test)
        {
            auto l = lexer("(){},");
            BOOST_CHECK(token(symbol::LPAREN) == l.next());
            BOOST_CHECK(token(symbol::RPAREN) == l.next());
            BOOST_CHECK(token(symbol::LBRACE) == l.next());
            BOOST_CHECK(token(symbol::RBRACE) == l.next());
            BOOST_CHECK(token(symbol::COMMA) == l.next());
            BOOST_CHECK(token(keyword::EOF_) == l.next());
        }

        BOOST_AUTO_TEST_CASE(op_test)
        {
            lexer l("1 + 2");
            BOOST_CHECK(token(number{1}) == l.next());
            BOOST_CHECK(token(op{'+'}) == l.next());
            BOOST_CHECK(token(number{2}) == l.next());
            BOOST_CHECK(token(keyword::EOF_) == l.next());
        }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace 
