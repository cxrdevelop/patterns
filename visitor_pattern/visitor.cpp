#include "visitor.h"
#include <iostream>
#include <variant>
#include <vector>
#include <algorithm>
#include "make_visitor_std11.h"

struct A {
    int value;
};

struct B {
    float value;
};

struct C {
    double value;
};

struct ConcreteVisitor {
    void operator()(A &a) { std::cout << "ConcreteVisitor for A: " << ++a.value << '\n'; };
    void operator()(B &b) { std::cout << "ConcreteVisitor for B: " << ++b.value << '\n'; };
    void operator()(C &c) { std::cout << "ConcreteVisitor for C: " << ++c.value << '\n'; };
};

struct AnotherConcreteVisitor {
    template<typename T>
    void operator()(const T &t) const { std::cout << "AnotherConcreteVisitor for T: " << t.value << '\n'; };

    void operator()(const C &c) const { std::cout << "AnotherConcreteVisitor for C: " << c.value << '\n'; };
};

void example_visitor() {

    std::vector<std::variant<A,B,C>> variants;

    variants.emplace_back(A{});
    variants.emplace_back(B{});
    variants.emplace_back(C{});

    auto lambda_visitor = utils::make_visitor(
        [&](const auto &) { std::cout << "LambdaVisitor for T\n"; },
        [&](const C& c) { std::cout << "LambdaVisitor for C: " << c.value << '\n'; }
    );


    std::for_each(
                std::begin(variants),
                std::end(variants),
                [](auto &v) {std::visit(ConcreteVisitor{}, v); });

    std::for_each(
                std::begin(variants),
                std::end(variants),
                [](auto &v) {std::visit(AnotherConcreteVisitor{}, v); });

    std::for_each(
                std::begin(variants),
                std::end(variants),
                [&](auto &v) {std::visit(lambda_visitor, v); });
}
