#include "Parser.hpp"

#include <sstream>
#include <algorithm>
#include <optional>

#include <iostream>

namespace yml
{

    static
    std::string
    ltrim
    (
        const std::string& str
    )
    {
        size_t start = 0;

        for (; start < str.size() && str[start] == ' '; ++start) {}
        return str.substr(start);
    }

    std::vector<std::string>
    Parser::split
    (
        const std::string &str,
        const char delim
    )
    {
        std::vector<std::string> tokens;
        size_t start = 0, end = 0;

        while ((end = str.find(delim, start)) != std::string::npos) {
            tokens.push_back(ltrim(str.substr(start, end - start)));
            start = end + 1;
        }
        tokens.push_back(ltrim(str.substr(start)));
        return tokens;
    }

    void
    Parser::parse
    (
        const std::string &rawContent
    )
    {
        std::istringstream f(rawContent);
        std::string needle;

        while (std::getline(f, needle)) {
            if (shouldSkipLine(needle)) {
                continue;
            }
            this->parseLine(needle);
        }
    }

    void
    Parser::parseLine
    (
        const std::string &needle
    )
    {
        const size_t spaces = countLeadingSpaces(needle);
        const std::vector<std::string> tokens = split(needle, ':');

        if (tokens.empty() || tokens.size() > 2) {
            throw; // TODO: Throw exception.
        }

        Node node(
            tokens.at(0),
            tokens.size() == 2 ? tokens.at(1) : ""
        );

        this->placeNode(needle, node, spaces);
    }

    void
    Parser::placeNode
    (
        const std::string &needle,
        Node &node,
        size_t spaces
    )
    {
        std::optional<std::reference_wrapper<Node>> parent = std::nullopt;
        const size_t depth = spaces / this->_nestingLevel;

        this->_currentPath.resize(getPathSize(this->_currentPath, depth));

        if (!this->_currentPath.empty()) {
            parent = this->_ymlInstance.getNode(this->_currentPath);
        }

        if (node.value.empty() && isObject(node.name, needle)) {
            if (!this->_currentPath.empty()) {
                this->_currentPath += ".";
            }
            this->_currentPath += node.name;
        }

        if (parent == std::nullopt) {
            this->_tree.addNode(node);
        } else {
            parent.value().get().children.addNode(node);
        }
    }

    size_t
    Parser::countLeadingSpaces(const std::string &str)
    {
        const auto it = std::find_if(
            str.begin(),
            str.end(),
            [](const char c) {
                return c != ' ';
            }
        );

        return std::distance(str.begin(), it);
    }

    bool
    Parser::shouldSkipLine(const std::string &needle)
    {
        if (needle.empty()) {
            return true; // Empty line? Skip.
        }

        for (const char c : needle) {
            if (std::isspace(c)) {
                continue;
            }
            return c == '#'; // Comment? Skip.
        }
        return true; // Only spaces? Skip.
    }

    size_t
    Parser::getPathSize
    (
        const std::string &path,
        const size_t n
    )
    {
        size_t counter = 0;
        size_t size = 0;

        if (path.empty() || n == 0) {
            return 0;
        }

        for (const char c : path) {
            if (c == '.') {
                counter++;
            }
            if (counter >= n) {
                break;
            }
            size++;
        }
        return size;
    }

    bool
    Parser::isObject
    (
        const std::string &name,
        const std::string &needle
    )
    {
        const std::string n = name + ":";

        return needle.find(n) != std::string::npos;
    }

}
