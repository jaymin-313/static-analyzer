#include "checks/ConstCorrectnessCheck.h"
#include <sstream>
#include <string>
#include <regex>
#include <set>
#include <vector>
#include <algorithm>

namespace checks
{

    bool isLineCommentOrEmpty(const std::string &line)
    {
        std::string trimmed = line;
        trimmed.erase(trimmed.begin(),
                      std::find_if(trimmed.begin(), trimmed.end(),
                                   [](unsigned char c)
                                   { return !std::isspace(c); }));
        return trimmed.empty() || trimmed.substr(0, 2) == "//";
    }

    std::string trim(const std::string &s)
    {
        size_t start = s.find_first_not_of(" \t");
        if (start == std::string::npos)
            return "";
        size_t end = s.find_last_not_of(" \t");
        return s.substr(start, end - start + 1);
    }

    void ConstCorrectnessCheck::run(const std::string &source,
                                    const std::string &filename,
                                    std::vector<analyzer::ReportItem> &report)
    {
        std::istringstream iss(source);
        std::string line;
        int lineno = 0;

        bool inClass = false;
        std::set<std::string> memberVars;
        std::string className;

        std::regex funcRegex(R"(([\w:<>]+)\s+([\w:]+)\s*\(([^)]*)\)\s*(const)?)");

        while (std::getline(iss, line))
        {
            ++lineno;
            if (isLineCommentOrEmpty(line))
                continue;

            std::string trimmed = trim(line);

            // Track class scope
            if (trimmed.find("class ") == 0 || trimmed.find("struct ") == 0)
            {
                inClass = true;
                memberVars.clear();
                std::istringstream ss(trimmed);
                std::string word;
                ss >> word; // class/struct
                ss >> className;
                continue;
            }

            if (inClass && trimmed.find('}') != std::string::npos)
            {
                inClass = false;
                continue;
            }

            // Track member variables (lines ending with ; and not functions)
            if (inClass && trimmed.back() == ';' && trimmed.find('(') == std::string::npos)
            {
                std::istringstream ts(trimmed);
                std::string word, varName;
                while (ts >> word)
                    varName = word;
                if (!varName.empty() && varName.back() == ';')
                    varName.pop_back();
                memberVars.insert(varName);
            }

            std::smatch match;
            if (std::regex_search(trimmed, match, funcRegex))
            {
                std::string funcName = match[2].str();
                std::string params = match[3].str();
                bool hasConst = match.size() >= 4 && match[4].matched;
                if (hasConst)
                    continue;

                // --- Capture entire function body ---
                std::string funcBody;
                int braceDepth = 0;
                bool started = (trimmed.find('{') != std::string::npos);
                if (started)
                    braceDepth++;
                std::streampos funcStartPos = iss.tellg();

                if (!started)
                {
                    // look for opening brace in next lines
                    std::streampos tempPos = iss.tellg();
                    std::string nextLine;
                    while (std::getline(iss, nextLine))
                    {
                        funcBody += nextLine + "\n";
                        if (nextLine.find('{') != std::string::npos)
                        {
                            braceDepth++;
                            started = true;
                            break;
                        }
                    }
                    iss.seekg(tempPos);
                }

                if (started)
                {
                    std::string bodyLine;
                    while (std::getline(iss, bodyLine))
                    {
                        funcBody += bodyLine + "\n";
                        braceDepth += std::count(bodyLine.begin(), bodyLine.end(), '{');
                        braceDepth -= std::count(bodyLine.begin(), bodyLine.end(), '}');
                        if (braceDepth <= 0)
                            break;
                    }
                }

                // --- Check reference parameters ---
                std::istringstream pstream(params);
                std::string token;
                while (std::getline(pstream, token, ','))
                {
                    if (token.find('&') != std::string::npos && token.find("const") == std::string::npos)
                    {
                        std::istringstream tstream(token);
                        std::string name, word;
                        while (tstream >> word)
                            name = word;
                        if (!name.empty())
                        {
                            std::regex assignRegex("\\b" + name + "\\s*=");
                            std::regex incRegex("\\b" + name + "\\s*\\+\\+");
                            std::regex decRegex("\\b" + name + "\\s*--");
                            if (!std::regex_search(funcBody, assignRegex) &&
                                !std::regex_search(funcBody, incRegex) &&
                                !std::regex_search(funcBody, decRegex))
                            {
                                report.push_back({filename, lineno, "Function parameter '" + name + "' could be const reference"});
                            }
                        }
                    }
                }

                // --- Check member function const correctness ---
                bool modifiesMember = false;
                for (const auto &var : memberVars)
                {
                    std::regex assignRegex("\\b(this->)?" + var + "\\s*=");
                    if (std::regex_search(funcBody, assignRegex))
                    {
                        modifiesMember = true;
                        break;
                    }
                }

                if (!modifiesMember && !funcName.empty() && inClass)
                {
                    report.push_back({filename, lineno, "Member function '" + funcName + "' could be marked const"});
                }
            }
        }
    }

} // namespace checks