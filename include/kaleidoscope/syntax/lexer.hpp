//
// Created by agatan on 15/09/25.
//

#ifndef KALEIDOSCOPE_LEXER_H
#define KALEIDOSCOPE_LEXER_H

#include <string>
#include <boost/variant.hpp>

namespace kaleidoscope {
    namespace syntax {

        struct unknown {
            bool operator==(unknown const&) const noexcept
            {
                return true;
            }
        };

        enum class keyword {
            EOF_,
            DEF,
            EXTERN
        };

        enum class symbol {
            LPAREN, // '('
            RPAREN, // ')'
            LBRACE, // '{'
            RBRACE, // '}'
            COMMA   // ','
        };

        struct number {
            double value;

            bool operator==(number const& rhs) const noexcept
            {
                auto diff = value - rhs.value;
                return diff < 0.001 && -diff < 0.001;
            }
        };

        struct ident {
            std::string value;

            bool operator==(ident const& rhs) const noexcept
            {
                return value == rhs.value;
            }
        };

        struct op {
            char value;

            bool operator==(op const& rhs) const noexcept
            {
                return value == rhs.value;
            }
        };

        using token = boost::variant<
                                unknown,
                                keyword,
                                symbol,
                                number,
                                ident,
                                op
                            >
                        ;

        class lexer {
        public:
            lexer(std::string const& src)
                : line_(1), line_head_(1), head_(std::begin(src)),
                  begin_(std::begin(src)), end_(std::end(src)) {}

            lexer(lexer const&) = default;

            lexer& operator=(lexer const&) = default;

            lexer(lexer&&) = default;

            token next();
        private:
            unsigned int line_;
            long line_head_;
            std::string::const_iterator head_;
            std::string::const_iterator begin_;
            std::string::const_iterator end_;

            char peek() const
            {
                return *begin_;
            }

            void next_char() noexcept
            {
                if (peek() == '\n') {
                    line_++;
                    line_head_ = std::distance(head_, begin_) + 1;
                }
                begin_++;
                return;
            }

            bool is_eof() const noexcept
            {
                return begin_ == end_;
            }

            void skip_ws();

            token parse_ident();
            token parse_num();
        };
    }
}

#endif //KALEIDOSCOPE_LEXER_H
