#include "Parser.hpp"

#include "Exception/Exceptions/InvalidUsage.hpp"
#include "Exception/Exceptions/UnknownFlag.hpp"

#include <iostream>

namespace raytracer
{

	Parser::Parser
    (
        const int argc,
        char *argv[]
    )
    {
        this->parse(argc, argv);
    }

    void
    Parser::parse
    (
        const int argc,
        char *argv[]
    )
    {
        for (int k = 1; k < argc; ++k) {
            this->_tokens.emplace_back(argv[k]);
        }

	    if (this->_tokens.size() != 1) {
	        throw exception::InvalidUsage();
	    }

	    this->_sceneFilepath = this->_tokens.at(0);
    }

    bool
    Parser::processFlags
    ()
        const
    {
	    if (this->_tokens.size() != 1) {
	        return false;
	    }

        if (this->hasFlag("--help") || this->hasFlag("-h")) {
            std::cout << "USAGE: ./raytracer <scene_file>" << std::endl
                      << "\tscene_file\tScene configuration file" << std::endl
                      << std::endl;
            return true;
        }

        if (this->hasFlag("--about") || this->hasFlag("-a")) { /* TODO: Finish. */
            std::cout << "OOP-400: Raytracer" << std::endl;
            return true;
        }

        throw exception::UnknownFlag(this->_tokens.at(0));
    }

    bool
    Parser::hasFlag
    (
        const std::string &flag
    )
        const
    {
        for (const auto &token : this->_tokens) {
            if (token == flag) {
                return true;
            }
        }
        return false;
    }

    std::string
    Parser::getFlagValue
    (
        const std::string &flag
    )
        const
    {
        for (size_t k = 0; k < this->_tokens.size(); ++k) {
            if (this->_tokens.at(k) == flag &&
                this->_tokens.size() > k + 1) {
                return this->_tokens.at(k + 1);
            }
        }
        return {};
    }

}
