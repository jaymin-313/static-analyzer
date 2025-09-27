// File: test_input.cpp
#include <iostream>

// Missing include guard for testing IncludeGuardCheck

class my_class
{ // NamingConventionCheck error: class not PascalCase
public:
    void DoWork()
    {                        // NamingConventionCheck error: function not camelCase
        int MagicValue = 42; // NamingConventionCheck + MagicNumberCheck errors
        int good_var = 0;    // OK
        // TODO: remove temporary code
    }
};

// Function with too many parameters (FunctionParameterCountCheck)
void tooManyParams(int a, int b, int c, int d, int e, int f)
{
    int x = 1; // MagicNumberCheck
}

void longFunctionExample()
{ // LongFunctionCheck
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;
    int h = 8;
    int i = 9;
    int j = 10;
    int k = 11;
    int l = 12;
    int m = 13;
    int n = 14;
    int o = 15;
    int p = 16;
    int q = 17;
    int r = 18;
    int s = 19;
    int t = 20;
}

int main()
{
    my_class c;
    c.DoWork();
    longFunctionExample();
    tooManyParams(1, 2, 3, 4, 5, 6);
    return 1; // MagicNumberCheck
}