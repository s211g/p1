#pragma once

#include <vector>
#include <iostream>

namespace pattern_builder {

    struct PartBase {
        PartBase(std::string name_, int i_) :
            name(name_), i(i_) {}
        std::string name;
        int i;
    };

    struct ResultBase {
        virtual void AddPart(PartBase p) = 0;
        virtual size_t GetCount()        = 0;
        virtual void print()             = 0;
    };

    class BuilderBase {
    public:
        virtual void Init()             = 0;
        virtual ResultBase* GetResult() = 0;
        virtual void BuildPartA(int i)  = 0;
        virtual void BuildPartB(int i)  = 0;
        ResultBase* result{nullptr};
    };

    struct PartAT1 : public PartBase {
        PartAT1(int i) :
            PartBase("PartAT1", i) {}
    };

    struct PartBT1 : public PartBase {
        PartBT1(int i) :
            PartBase("PartBT1", i) {}
    };

    struct ResultT1 : public ResultBase {
        std::vector<PartBase> v;
        void AddPart(PartBase p) override { v.push_back(p); };
        size_t GetCount() override { return v.size(); };
        void print() override {
            for (auto& i : v) std::cout << i.name << " " << i.i << ", ";
            std::cout << std::endl;
        }
    };

    class BuilderT1 : public BuilderBase {
    public:
        void Init() { result = new ResultT1(); }
        void BuildPartA(int i) override { result->AddPart(PartAT1{i}); };
        void BuildPartB(int i) override { result->AddPart(PartBT1{i}); };
        ResultBase* GetResult() override {
            return result;
        }
    };

}
