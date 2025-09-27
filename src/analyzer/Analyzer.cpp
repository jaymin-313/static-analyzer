#include "analyzer/Analyzer.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace analyzer
{

    void Analyzer::addCheck(std::unique_ptr<ICheck> check)
    {
        if (check)
            checks_.push_back(std::move(check));
    }

    void Analyzer::analyzeFile(const std::string &path)
    {
        std::ifstream ifs(path);
        if (!ifs)
        {
            throw std::runtime_error("Failed to open file: " + path);
        }
        std::ostringstream oss;
        oss << ifs.rdbuf();
        const std::string content = oss.str();

        // Run all checks
        for (const auto &chk : checks_)
        {
            if (chk)
                chk->run(content, path, reports_);
        }
    }

} // namespace analyzer