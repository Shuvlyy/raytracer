#pragma once

#include "Common/ProgramMode.hpp"
#include "Common/Utils.hpp"

#include <string>
#include <vector>

namespace raytracer
{

    struct Attributes
    {
        Mode programMode = Mode::SELF;
        uint16_t threadsAmount = utils::getNbProcs();
        uint16_t port;
        std::string host;
        bool debugMode = false;
        std::vector<std::string> sceneFilepaths;
        std::string serverConfigFilepath;
        bool noPreview = false;
        int tileSize = -1; // Auto, managed in Cluster or Self
    };

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
         * @brief   Parses the command-line arguments and stores flags, values,
         *          and the scene path.
         *
         * @param   argc    Argument amount
         * @param   argv    Arguments
         */
        bool parse(int argc, char *argv[]);

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
         * @return  The value associated with the flag or an empty string if
         *          not found
         */
        [[nodiscard]] std::string getFlagValue(const std::string &flag) const;

        [[nodiscard]] const Attributes& getAttributes() const { return this->_attributes; };

    private:
        std::vector<std::string> _tokens;
        Attributes _attributes{};

        void validateArguments() const;
    };

}
