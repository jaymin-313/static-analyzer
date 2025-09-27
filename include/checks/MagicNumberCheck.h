#pragma once
#include "analyzer/Analyzer.h"

namespace checks
{

    class MagicNumberCheck : public analyzer::ICheck
    {
    public:
        MagicNumberCheck() = default;
        ~MagicNumberCheck() override = default;

        void run(const std::string &source,
                 const std::string &filename,
                 std::vector<analyzer::ReportItem> &report) override;
    };

} // namespace checks