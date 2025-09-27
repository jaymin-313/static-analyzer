#pragma once
#include "analyzer/Analyzer.h"

namespace checks
{

    class LongFunctionCheck : public analyzer::ICheck
    {
    public:
        explicit LongFunctionCheck(int maxLines = 50) : maxLines_(maxLines) {}
        ~LongFunctionCheck() override = default;

        void run(const std::string &source,
                 const std::string &filename,
                 std::vector<analyzer::ReportItem> &report) override;

    private:
        int maxLines_;
    };

} // namespace checks