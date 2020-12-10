#include "polymorphic_visitor.h"

#include <iostream>
#include <vector>
#include <memory>

struct A;
struct B;
struct C;

struct VisitorInterface {
    virtual ~VisitorInterface() = default;
    virtual void visit(A &a) = 0;
    virtual void visit(B &b) = 0;
    virtual void visit(C &c) = 0;
};

struct Base {
    virtual ~Base() = default;
    virtual void accept(VisitorInterface &visitor) = 0;
};

struct A : Base {
    virtual void accept(VisitorInterface &visitor) final { visitor.visit(*this); }
    int value;
};

struct B : Base {
    virtual void accept(VisitorInterface &visitor) final { visitor.visit(*this); }
    float value;
};

struct C : Base {
    virtual void accept(VisitorInterface &visitor) final { visitor.visit(*this); }
    double value;
};

struct ConcreteVisitor : VisitorInterface {
    virtual void visit(A &a) final { std::cout << "ConcreteVisitor for A: " << ++a.value << '\n'; };
    virtual void visit(B &b) final { std::cout << "ConcreteVisitor for B: " << ++b.value << '\n'; };
    virtual void visit(C &c) final { std::cout << "ConcreteVisitor for C: " << ++c.value << '\n'; };
};

struct AnotherConcreteVisitor : VisitorInterface {
    virtual void visit(A &a) final { std::cout << "AnotherConcreteVisitor for A: " << ++a.value << '\n'; };
    virtual void visit(B &b) final { std::cout << "AnotherConcreteVisitor for B: " << ++b.value << '\n'; };
    virtual void visit(C &c) final { std::cout << "AnotherConcreteVisitor for C: " << ++c.value << '\n'; };
};

void example_polymorphic_visitor() {

    std::vector<std::unique_ptr<Base>> items;
    items.emplace_back(std::make_unique<A>());
    items.emplace_back(std::make_unique<B>());
    items.emplace_back(std::make_unique<C>());

    for (auto &item : items) {
        ConcreteVisitor cv{};
        AnotherConcreteVisitor acv{};
        item->accept(cv);
        item->accept(acv);
    }
}
