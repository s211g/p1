#pragma once

#include <memory>
#include <string>
#include <map>

namespace pattern_interpreter {

    class Context;

    class BooleanExp {
    public:
        BooleanExp()                                          = default;
        virtual ~BooleanExp()                                 = default;
        virtual bool Evaluate(Context&)                       = 0;
        virtual BooleanExp* Replace(std::string, BooleanExp&) = 0;
        virtual BooleanExp* Copy() const                      = 0;
    };

    class Context {
    public:
        bool Lookup(std::string var) const {
            auto it = map.find(var);
            if (it != map.end())
                return it->second;
            return false;
        }
        void Assign(std::string var, bool b) {
            map[var] = b;
        }

    private:
        std::map<std::string, bool> map;
    };

    class VariableExp : public BooleanExp {
    public:
        VariableExp(std::string name_) :
            name(name_) {}

        bool Evaluate(Context& ctx) override {
            return ctx.Lookup(name);
        }
        BooleanExp* Copy() const override {
            return new VariableExp(name);
        }

        BooleanExp* Replace(std::string name_, BooleanExp& exp) override {
            return name == name_ ? exp.Copy() : new VariableExp(name);
        }

    private:
        std::string name;
    };

    template <typename T>
    class BinExp : public BooleanExp {
    public:
        virtual ~BinExp() {}
        BinExp(BooleanExp* exp1_, BooleanExp* exp2_) :
            exp1(exp1_), exp2(exp2_) {
        }

        virtual bool Evaluate(Context& ctx) override { return false; }

        virtual BooleanExp* Copy() const override {
            return new T(exp1->Copy(), exp2->Copy());
        }

        virtual BooleanExp* Replace(std::string name_, BooleanExp& exp) override {
            return new T(exp1->Replace(name_, exp), exp2->Replace(name_, exp));
        }

    protected:
        BooleanExp* exp1;
        BooleanExp* exp2;
    };

    class AndExp : public BinExp<AndExp> {
    public:
        ~AndExp() {}
        AndExp(BooleanExp* exp1_, BooleanExp* exp2_) :
            BinExp<AndExp>(exp1_, exp2_) {
        }
        bool Evaluate(Context& ctx) override {
            return exp1->Evaluate(ctx) && exp2->Evaluate(ctx);
        }
    };

    class OrExp : public BinExp<OrExp> {
    public:
        OrExp(BooleanExp* exp1_, BooleanExp* exp2_) :
            BinExp<OrExp>(exp1_, exp2_) {
        }
        bool Evaluate(Context& ctx) override {
            return exp1->Evaluate(ctx) || exp2->Evaluate(ctx);
        }
    };

}