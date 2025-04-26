#pragma once

#include "Yml.hpp"

#include <vector>

namespace yml
{

    /**
     * @brief   Responsible for parsing YAML content into a tree structure.
     *
     * This class processes raw YAML strings, interprets their structure,
     * and organizes the data into a hierarchical Tree of Node objects.
     */
    class Parser final
    {
    public:
        /**
         * @brief   Constructs a Parser and immediately parses the given
         *          content.
         *
         * @param   yml         Reference to the associated Yml instance
         * @param   rawContent  The raw YAML file content as a string
         * @param   tree        Reference to the tree structure to populate
         */
        explicit Parser
        (
            Yml& yml,
            const std::string& rawContent,
            Tree& tree,
            const uint8_t nestingLevel
        )
            : _ymlInstance(yml), _tree(tree), _nestingLevel(nestingLevel)
        {
            parse(rawContent);
        }

        /**
         * @brief   Splits a string by a delimiter character.
         *
         * Trims leading spaces from each token after splitting.
         * Similar to the split() function in Python.
         *
         * @param   str     The string to split
         * @param   delim   The delimiter character
         * @returns A vector of trimmed substrings
         */
        static std::vector<std::string> split(
            const std::string& str,
            char delim
        );

    private:
        Yml& _ymlInstance;
        Tree& _tree;
        std::string _currentPath;
        uint8_t _nestingLevel;

        /**
         * @brief   Parses the entire raw YAML content.
         *
         * Reads the content line-by-line, while skipping irrelevant lines.
         *
         * @param   rawContent  The raw YAML content to parse
         */
        void parse(const std::string& rawContent);

        /**
         * @brief   Parses a single line of YAML content.
         *
         * Splits the line into key-value tokens and creates a Node.
         *
         * @param   needle  The line of text to parse
         */
        void parseLine(const std::string& needle);

        /**
         * @brief   Places a Node into the tree based on its indentation level.
         *
         * @param   needle  The original line string
         * @param   node    The Node to place
         * @param   spaces  The number of leading spaces (indentation level)
         */
        void placeNode(const std::string& needle, Node& node, size_t spaces);


        /**
         * @brief   Checks if a line should be skipped (empty or comment).
         *
         * @param   needle  The line to check
         * @returns True if the line should be skipped, false otherwise.
         */
        static bool shouldSkipLine(const std::string& needle);

        /**
         * @brief   Counts the number of leading spaces in a string.
         *
         * Used to determine the nesting level in YAML.
         *
         * @param   str The string to analyze
         * @returns The number of leading spaces
         */
        static size_t countLeadingSpaces(const std::string& str);

        /**
         * @brief   Calculates the substring size for a given path depth.
         *
         * Helps in managing nested paths when inserting nodes.
         *
         * @param   path    The full path string
         * @param   n       The desired depth
         * @returns The size (number of characters) up to depth n
         */
        static size_t getPathSize(const std::string& path, size_t n);

        /**
         * @brief   Determines if a line represents a new object (key with
         *          colon).
         *
         * @param   name    The name of the node.
         * @param   needle  The line to inspect.
         * @returns True if it represents an object, false otherwise.
         */
        static bool isObject(const std::string& name, const std::string& needle);
    };

}
