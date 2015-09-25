#include <gtest/gtest.h>
#include <kaleidoscope/lexer.h>

namespace  {

    using namespace kaleidoscope::syntax;

    class LexerTest : public ::testing::Test {};

    TEST_F(LexerTest, IDENT)
    {
        auto l = kaleidoscope::syntax::lexer("def extern ident");
        EXPECT_TRUE(kaleidoscope::syntax::token(kaleidoscope::syntax::keyword::DEF) == l.next());
        EXPECT_TRUE(kaleidoscope::syntax::token(kaleidoscope::syntax::keyword::EXTERN) == l.next());
        EXPECT_TRUE(kaleidoscope::syntax::token(kaleidoscope::syntax::ident{"ident"}) == l.next());
    }

    TEST_F(LexerTest, NUM)
    {
        auto l = kaleidoscope::syntax::lexer("12.3");
        EXPECT_TRUE(kaleidoscope::syntax::token(kaleidoscope::syntax::number{12.3}) == l.next());
        EXPECT_TRUE(kaleidoscope::syntax::token(kaleidoscope::syntax::keyword::EOF_) == l.next());
    }

    TEST_F(LexerTest, SYMBOL)
    {
        auto l = lexer("(){},");
        EXPECT_TRUE(token(symbol::LPAREN) == l.next());
        EXPECT_TRUE(token(symbol::RPAREN) == l.next());
        EXPECT_TRUE(token(symbol::LBRACE) == l.next());
        EXPECT_TRUE(token(symbol::RBRACE) == l.next());
        EXPECT_TRUE(token(symbol::COMMA) == l.next());
        EXPECT_TRUE(token(keyword::EOF_) == l.next());
    }

} // namespace 
