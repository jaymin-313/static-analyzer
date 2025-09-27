#pragma once
#include <string>
#include <vector>
#include <memory>

namespace analyzer
{

    struct ReportItem
    {
        std::string file;
        int line = 0;
        std::string message;
    };

    class ICheck
    {
    public:
        virtual ~ICheck() = default;
        virtual void run(const std::string &source,
                         const std::string &filename,
                         std::vector<ReportItem> &report) = 0;
    };

    class Analyzer
    {
    public:
        Analyzer() = default;
        ~Analyzer() = default;

        // non-copyable
        Analyzer(const Analyzer &) = delete;
        Analyzer &operator=(const Analyzer &) = delete;

        void addCheck(std::unique_ptr<ICheck> check);
        void analyzeFile(const std::string &path);
        const std::vector<ReportItem> &reports() const noexcept { return reports_; }

    private:
        std::vector<std::unique_ptr<ICheck>> checks_;
        std::vector<ReportItem> reports_;
    };

} // namespace analyzer