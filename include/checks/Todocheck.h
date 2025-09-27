#pragma once
#include "analyzer/Analyzer.h"
#include <string>

namespace checks
{

    class TodoCheck : public analyzer::ICheck
    {
    public:
        TodoCheck() = default;
        ~TodoCheck() override = default;
        void run(const std::string &source,
                 const std::string &filename,
                 std::vector<analyzer::ReportItem> &report) override;
    };

} // namespace checks