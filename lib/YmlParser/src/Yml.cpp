#include "Yml.hpp"
#include "Parser/Parser.hpp"

#include "Exception/Exceptions/CouldNotOpenFile.hpp"

#include <utility>
#include <fstream>
#include <sstream>
#include <iostream>

namespace yml
{

    Yml::Yml
    (
        std::string filepath,
        const uint8_t nestingLevel
    )
        : _filepath(std::move(filepath))
    {

        std::string rawContent = getFileContent(this->_filepath);

        Parser parser(*this, rawContent, this->_tree, nestingLevel);
    }

    std::optional<std::reference_wrapper<Node>>
    Yml::getNode
    (
        const std::string &search
    )
    {
        std::optional<std::reference_wrapper<Node>> current;

        for (const auto& part : Parser::split(search, '.')) {
            current = current
                ? current->get().children[part]
                : this->_tree[part];

            if (current == std::nullopt) {
                break;
            }
        }

        return current;
    }

    void
    Yml::dump()
    {
        std::cout << "---=== YML Dump ===---\n" << std::endl;
        for (const auto& [_, node] : this->_tree.getNodes()) {
            node.dump();
        }
        std::cout << "\n---=== -------- ===---" << std::endl;
    }

    std::string
    Yml::getFileContent
    (
        const std::string& filepath
    )
    {
        std::stringstream buf;
        std::ifstream file(filepath);

        if (!file.is_open()) {
            throw exception::CouldNotOpenFile(filepath);
        }

        buf << file.rdbuf();
        file.close();

        return buf.str();
    }

}
