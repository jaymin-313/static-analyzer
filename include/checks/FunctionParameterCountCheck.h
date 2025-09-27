#pragma once
#include "analyzer/Analyzer.h"

namespace checks
{

    class FunctionParameterCountCheck : public analyzer::ICheck
    {
    public:
        explicit FunctionParameterCountCheck(int maxParams = 5) : maxParams_(maxParams) {}
        ~FunctionParameterCountCheck() override = default;

        void run(const std::string &source,
                 const std::string &filename,
                 std::vector<analyzer::ReportItem> &report) override;

    private:
        int maxParams_;
    };

} // namespace checks
