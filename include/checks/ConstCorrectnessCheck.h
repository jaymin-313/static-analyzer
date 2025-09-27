#pragma once
#include "analyzer/Analyzer.h"

namespace checks
{

    class ConstCorrectnessCheck : public analyzer::ICheck
    {
    public:
        ConstCorrectnessCheck() = default;
        ~ConstCorrectnessCheck() override = default;

        void run(const std::string &source,
                 const std::string &filename,
                 std::vector<analyzer::ReportItem> &report) override;
    };

} // namespace checks
