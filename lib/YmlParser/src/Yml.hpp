#pragma once

#define MAX_STRING_LENGTH   1024
#define YML_NESTING_SPACES  2

#include "Node/Node.hpp"

#include <optional>
#include <string>
#include <cstdint>

namespace yml
{

    /**
     * @brief   Represents a YAML-like parser and manager.
     *
     * The Yml class provides functionality to load, parse, and search
     * for nodes within a YAML-style structured file. It internally builds
     * a tree structure from the file's contents.
     */
    class Yml final
    {
    public:
        /**
         * @brief   Constructs a Yml instance for a given file.
         *
         * @param   filepath        The path to the file to parse
         * @param   nestingLevel    The number of spaces used to represent one
         *                          level of nesting. Defaults to
         *                          YML_NESTING_SPACES.
         * @throws  exception::CouldNotOpenFile If file could not be opened for
         *                                      some reason
         * @throws  exception::IException       Parsing error
         */
        explicit Yml(
            std::string filepath,
            uint8_t nestingLevel = YML_NESTING_SPACES
        );

        /**
         * @brief   Retrieves a node from the parsed tree by its search key.
         *
         * @param   search  The key or identifier to search for within the tree
         * @returns An optional reference to the found Node, or std::nullopt if
         *          not found.
         */
        std::optional<std::reference_wrapper<Node>>
            getNode(const std::string& search);

        /**
         * @brief   Dumps the entire parsed tree structure to the standard
         *          output
         */
        void dump();

        /**
         * @brief   Provides access to the root-level nodes by name.
         *
         * This operator allows direct access to a child node of the root tree
         * using the bracket [] operator.
         *
         * @param   name    The name of the node to access
         * @returns A reference to the corresponding Node
         */
        Node& operator[](const std::string& name) { return this->_tree[name]; }

        /**
         * @brief   Provides read-only access to the root-level nodes by name.
         *
         * This operator allows direct, read-only access to a child node of the
         * root tree using the bracket [] operator.
         *
         * @param   name    The name of the node to access
         * @returns A const reference to the corresponding Node
         */
        const Node& operator[](const std::string& name) const { return this->_tree[name]; }

    private:
        const std::string _filepath;
        Tree _tree;

        /**
         * @brief   Reads the content of a file into a string.
         *
         * @param   filepath    The path to the file to read
         * @returns The content of the file as a std::string.
         * @throws  exception::CouldNotOpenFile If file could not be opened for
         *                                      some reason
         */
        static std::string getFileContent(const std::string& filepath);
    };

}
