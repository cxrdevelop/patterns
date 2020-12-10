#include <iostream>
#include "polymorphic_visitor.h"
#include "visitor.h"

int main()
{
    std::cout << "Hello World!" << std::endl;

    example_polymorphic_visitor();
    example_visitor();

    return 0;
}
