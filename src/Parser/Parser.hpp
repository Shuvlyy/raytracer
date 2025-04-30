#pragma once

#include <string>
#include <vector>

namespace raytracer
{

    /**
     * @brief   A command-line argument parser.
     *
     * Responsible for parsing and querying command-line flags and parameters,
     * including identifying the scene file path and any runtime options.
     */
    class Parser final
    {
    public:

        /**
         * @brief   Constructs a Parser and immediately parses command-line
         *          arguments.
         *
         * @param   argc    Argument amount
         * @param   argv    Arguments
         */
        explicit Parser(int argc, char *argv[]);

        /**
         * @brief   Determines if the application should terminate after flag
         *          processing.
         *
         * @return  true if early exit is required, false otherwise
         */
        [[nodiscard]] bool processFlags() const;

        /**
         * @brief   Checks if a specific flag was passed in the command-line
         *          arguments.
         *
         * @param   flag    The flag to search for
         * @return  true if the flag was found, false otherwise
         */
        [[nodiscard]] bool hasFlag(const std::string &flag) const;

        /**
         * @brief   Retrieves the value associated with a flag
         *          (e.g., --config=config.yml) => "config.yml".
         *
         * @param   flag    The flag name
         * @return  The value associated with the flag, or an empty string if
         *          not found
         */
        [[nodiscard]] std::string getFlagValue(const std::string &flag) const;

        /**
         * @brief   Returns the path to the scene file extracted from the
         *          arguments.
         *
         * @return  The scene file path as a string
         */
        [[nodiscard]] std::string getSceneFilepath() const { return this->_sceneFilepath; }

    private:
        std::vector<std::string> _tokens;
        std::string _sceneFilepath;

        /**
         * @brief   Parses the command-line arguments and stores flags, values,
         *          and the scene path.
         *
         * @param   argc    Argument amount
         * @param   argv    Arguments
         */
        void parse(int argc, char *argv[]);
    };

}
