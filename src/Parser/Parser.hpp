#pragma once

#include <string>
#include <vector>

namespace raytracer
{

    class Parser final
    {
    public:
        explicit Parser(
            int argc,
            char *argv[]
        );

        /**
         * @returns  true if program should terminate after processing flags. Otherwise, false.
         */
        [[nodiscard]] bool processFlags() const;

        [[nodiscard]] bool hasFlag(const std::string &flag) const;
        [[nodiscard]] std::string getFlagValue(const std::string &flag) const;

        [[nodiscard]] std::string getSceneFilepath() const { return this->_sceneFilepath; }

    private:
        std::vector<std::string> _tokens;
        std::string _sceneFilepath;

        void parse(int argc, char *argv[]);
    };

}
