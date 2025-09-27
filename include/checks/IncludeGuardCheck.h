#pragma once
#include "analyzer/Analyzer.h"

namespace checks
{

    class IncludeGuardCheck : public analyzer::ICheck
    {
    public:
        IncludeGuardCheck() = default;
        ~IncludeGuardCheck() override = default;

        void run(const std::string &source,
                 const std::string &filename,
                 std::vector<analyzer::ReportItem> &report) override;
    };

} // namespace checks
