//
// Created by wangrl on 19-3-29.
//

#include <iostream>
#include <memory>
#include <cassert>

struct B {
    virtual void bar() {
        std::cout << "B::bar\n";
    }

    virtual ~B() = default;
};

struct D : B {
    D() {
        std::cout << "D::D\n";
    }
    ~D() {
        std::cout << "D::~D\n";
    }
    void bar() override {
        std::cout << "D::bar\n";
    }
};

// a function consuming a unique_ptr can take it by value or by rvalue reference
std::unique_ptr<D> pass_through(std::unique_ptr<D> p) {
    p->bar();
    return p;
}

int main(int argc, char* argv[]) {
    std::cout << "unique ownership semantics demo\n";
    {
        auto p = std::make_unique<D>();     // P is a unique_ptr that own a D.
        auto q = pass_through(std::move(p));
        assert(!p); // now p owns nothing and holds a null pointer
        q->bar();       // and q owns the D object
    }   // ~D called here

    std::cout << "Runtime polymorphism demo\n";
    {

    }

    return 0;
}
