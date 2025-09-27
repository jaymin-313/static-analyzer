#pragma once
#include "analyzer/Analyzer.h"
#include <regex>

namespace checks
{

    class NamingConventionCheck : public analyzer::ICheck
    {
    public:
        NamingConventionCheck() = default;
        ~NamingConventionCheck() override = default;

        void run(const std::string &source,
                 const std::string &filename,
                 std::vector<analyzer::ReportItem> &report) override;

    private:
        bool isPascalCase(const std::string &name) const;
        bool isCamelCase(const std::string &name) const;
        bool isSnakeCase(const std::string &name) const;
    };

} // namespace checks