#pragma once

namespace pattern_visitor {

    class Element;

    class Visitor {
    public:
        virtual ~Visitor()           = default;
        virtual void Visit(Element*) = 0;
    };

    class Element {
    public:
        virtual ~Element() = default;

        virtual void Accept(Visitor& v) {
            v.Visit(this);
        }

    protected:
        Element() = default;
    };

    class CompositeElements {
    public:
        ~CompositeElements() = default;
        void AddElement(Element* element) {
            elements.push_back(element);
        }

        virtual void Accept(Visitor& v) {
            for (auto& e : elements)
                e->Accept(v);
        }

    protected:
        std::list<Element*> elements;
    };
}