#include <iostream>

class my_class
{ // Bad: should be PascalCase
public:
    void DoWork()
    {                                             // Bad: should be camelCase
        int MagicValue = 42;                      // Bad: variable should be snake_case, magic number 42
        int good_var = 0;                         // OK
        std::cout << "TODO: Fix me" << std::endl; // TODO check
    }
};

int main()
{
    my_class c;
    c.DoWork();
    return 1; // magic number (but usually tolerated)
}
