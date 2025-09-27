#include <iostream>
#include "analyzer/Analyzer.h"
#include "checks/TodoCheck.h"
#include "checks/NamingConventionCheck.h"
#include "checks/MagicNumberCheck.h"
#include "checks/LongFunctionCheck.h"

int main(int argc, char **argv)
{
    std::string file = "test_input.cpp";
    if (argc > 1)
        file = argv[1];

    analyzer::Analyzer analyzer;
    analyzer.addCheck(std::make_unique<checks::TodoCheck>());
    analyzer.addCheck(std::make_unique<checks::NamingConventionCheck>());
    analyzer.addCheck(std::make_unique<checks::MagicNumberCheck>());
    analyzer.addCheck(std::make_unique<checks::LongFunctionCheck>(10)); // set max 10 lines for demo

    try
    {
        analyzer.analyzeFile(file);
        const auto &reports = analyzer.reports();
        if (reports.empty())
        {
            std::cout << "No issues found in " << file << std::endl;
            return 0;
        }
        for (const auto &r : reports)
        {
            std::cout << r.file << ":" << r.line << " - " << r.message << std::endl;
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}