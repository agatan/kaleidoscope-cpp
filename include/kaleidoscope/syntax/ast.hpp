#pragma once

#include <memory>
#include <vector>
#include <boost/variant.hpp>

namespace kaleidoscope {
    namespace syntax {

        class ast_node {
        public:
            ast_node(): id_(i++) {}
            int id() const { return id_; }
        private:
            static int i;
            int id_;
        };

        // forward declarations
        class number_expr;
        class variable_expr;
        class binary_expr;
        class call_expr;

#       define DEFINE_PTR_ALIAS(node) using node##_ptr = std::shared_ptr<node>;

        DEFINE_PTR_ALIAS(number_expr);
        DEFINE_PTR_ALIAS(variable_expr);
        DEFINE_PTR_ALIAS(binary_expr);
        DEFINE_PTR_ALIAS(call_expr);

        using any_expr = boost::variant <
                            number_expr_ptr,
                            variable_expr_ptr,
                            binary_expr_ptr,
                            call_expr_ptr
                        >
                        ;

        class expr : public ast_node {};

        class number_expr : public expr {
        public:
            number_expr(double v)
                : value_(v) {}

            bool operator ==(number_expr const& rhs)
            {
                auto diff = value_ - rhs.value_;
                return diff < 0.0001 && -diff < 0.0001;
            }

            double value() const noexcept { return value_; }

        private:
            double value_;
        };

        class variable_expr : public expr {
        public:
            variable_expr(std::string const& name)
                : name_(name) {}

            bool operator ==(variable_expr const& rhs)
            {
                return name_ == rhs.name_;
            }

            std::string const& name() const noexcept { return name_; }

        private:
            std::string name_;
        };

        class binary_expr : public expr {
        public:
            binary_expr(char const op, any_expr const& lhs, any_expr const& rhs)
                : op_(op), lhs_(lhs), rhs_(rhs) {}

            bool operator==(binary_expr const& rhs)
            {
                return op_ == rhs.op_ && lhs_ == rhs.lhs_ && rhs_ == rhs.rhs_;
            }

            char op() const noexcept { return op_; }
            any_expr const& lhs() const noexcept { return lhs_; }
            any_expr const& rhs() const noexcept { return rhs_; }

        private:
            char op_;
            any_expr lhs_, rhs_;
        };

        class call_expr : public expr {
        public:
            call_expr(std::string const& callee,
                      std::vector<any_expr> const& args)
                : callee_(callee), args_(args) {}

            bool operator==(call_expr const& rhs)
            {
                return callee_ == rhs.callee_ && args_ == rhs.args_;
            }

        private:
            std::string callee_;
            std::vector<any_expr> args_;
        };


        class prototype : public ast_node {
        public:
            prototype(std::string const& name, std::vector<std::string> const& args)
                : name_(name), args_(args) {}

            bool operator==(prototype const& rhs)
            {
                return name_ == rhs.name_ && args_ == rhs.args_;
            }

        private:
            std::string name_;
            std::vector<std::string> args_;
        };


        class func : public ast_node {
        public:
            func(prototype const& proto, any_expr const& body)
                : proto_(proto), body_(body) {}

            bool operator==(func const& rhs)
            {
                return proto_ == rhs.proto_ && body_ == rhs.body_;
            }

            prototype const& proto() const noexcept { return proto_; }
            any_expr const& body() const noexcept { return body_; }

        private:
            prototype proto_;
            any_expr body_;
        };

    } // namespace syntax
} // namespace kaleidoscope
