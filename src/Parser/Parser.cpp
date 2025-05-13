#include "Parser.hpp"

#include "Exception/Exceptions/InvalidUsage.hpp"
#include "Exception/Exceptions/UnknownFlag.hpp"

#include <iostream>
#include <unordered_map>

namespace raytracer
{

    static std::string normalizeFlag
    (
        const std::string& flag
    )
    {
        /* FIXME: euh fix ça stp bâtard */
        static const std::unordered_map<std::string, std::string> aliasMap =
        {
            {"-m", "--mode"},
            {"--cores", "--threads"},
            {"-c", "--threads"},
            {"-t", "--threads"},
            {"--debug", "-d"},
            {"-v", "-d"},
            {"--verbose", "-d"}
        };

        auto it = aliasMap.find(flag);
        return it != aliasMap.end() ? it->second : flag;
    }

    bool
    Parser::parse
    (
        const int argc,
        char *argv[]
    )
    {
        for (int k = 1; k < argc; ++k) {
            this->_tokens.emplace_back(argv[k]);
        }

        for (size_t i = 0; i < _tokens.size(); ++i) {
            const std::string& token = normalizeFlag(_tokens[i]);

            if (token == "--mode") {
                if (++i >= _tokens.size())
                    throw exception::InvalidUsage("Expected mode after --mode");

                const std::string value = _tokens[i];

                if (value == "self")        _attributes.programMode = Mode::SELF;
                else if (value == "server") _attributes.programMode = Mode::SERVER;
                else if (value == "client") _attributes.programMode = Mode::CLIENT;
                else throw exception::InvalidUsage("Invalid mode: " + value);
            }
            else if (token == "--threads") {
                if (++i >= _tokens.size())
                    throw exception::InvalidUsage("Expected value after --threads");

                const std::string value = _tokens[i];

                if (value != "auto") {
                    try {
                        _attributes.threadsAmount = static_cast<uint16_t>(std::stoi(value));
                    } catch (...) {
                        throw exception::InvalidUsage("Invalid thread count: " + value);
                    }
                }
            }
            else if (token == "--config") {
                if (++i >= _tokens.size())
                    throw exception::InvalidUsage("Expected value after --config");

                _attributes.serverConfigFilepath = _tokens[i];
            }
            else if (token == "-h") {
                if (++i >= _tokens.size())
                    throw exception::InvalidUsage("Expected host after -h");

                _attributes.host = _tokens[i];
            }
            else if (token == "-p") {
                if (++i >= _tokens.size())
                    throw exception::InvalidUsage("Expected port after -p");

                try {
                    _attributes.port = static_cast<uint16_t>(std::stoi(_tokens[i]));
                } catch (...) {
                    throw exception::InvalidUsage("Invalid port number: " + _tokens[i]);
                }
            }
            else if (token == "-d") {
                _attributes.debugMode = true;
            }
            else if (token.starts_with("-")) {
                // Leave for processFlags to handle unknown flags like --help
                continue;
            }
            else {
                if (!_attributes.sceneFilepath.empty()) {
                    throw exception::InvalidUsage("Only one scene file path can be provided");
                }
                _attributes.sceneFilepath = token;
            }
        }

	    if (!processFlags()) {
	        this->validateArguments();
	        return false;
	    }

        return true;
    }

    bool
    Parser::processFlags()
        const
    {
	    if (!(this->_tokens.size() == 1 && this->_tokens.at(0).at(0) == '-')) {
	        return false;
	    }

        if (this->hasFlag("--help") || this->hasFlag("-h")) {
            std::cout
                << "USAGE: ./raytracer [--mode=program_mode] [--threads=threads_amount] [-h host] [-p port] [-d] scene_filepath" << std::endl
                << "\tprogram_mode\tProgram mode (`self`, `server`, `client`). Defaults to `self`." << std::endl
                << "\tthreads_amount\tNumber of threads that will be used to render the image. Defaults to `auto` (maximum threads available)." << std::endl
                << "\thost\t\tHost to connect to. Only works if mode is set to `client`." << std::endl
                << "\tport\t\tPort to connect to / host from. Only works if clustering is used (mode is set to either `client` or `server`)." << std::endl
                << "\tscene_filepath\tFile path of the scene to render. If mode is set to `client`, an exception will be thrown." << std::endl
                << std::endl;
            return true;
        }

        if (this->hasFlag("--about") || this->hasFlag("-a")) {
            std::cout
                << "OOP-400: Raytracer" << std::endl
                << "Made by:" << std::endl
                << "\tLysandre BOURSETTE (@shuvlyy)" << std::endl
                << "\tPierre MARGUERIE (@PierreMarguerie)" << std::endl
                << "\tPierre MONTARET (@impierrooo)" << std::endl;
            return true;
        }

        return false; // ?
        // throw exception::UnknownFlag(this->_tokens.at(0));
    }

    bool
    Parser::hasFlag
    (
        const std::string &flag
    )
        const
    {
        return std::ranges::find(this->_tokens, flag) != this->_tokens.end();
    }

    std::string
    Parser::getFlagValue
    (
        const std::string &flag
    )
        const
    {
        for (size_t k = 0; k + 1 < this->_tokens.size(); ++k) {
            if (this->_tokens.at(k) == flag) {
                return this->_tokens.at(k + 1);
            }
        }
        return {};
    }

    void
    Parser::validateArguments()
        const
    {
        const auto& attrs = _attributes;

        switch (attrs.programMode) {
            case Mode::CLIENT:
                if (attrs.host.empty())
                    throw exception::InvalidUsage("Client mode requires a host (-h <host>)");
                if (!attrs.port)
                    throw exception::InvalidUsage("Client mode requires a port (-p <port>)");
                if (!attrs.sceneFilepath.empty())
                    throw exception::InvalidUsage("Client mode must not include a scene file path");
                break;

            case Mode::SERVER:
                if (!attrs.port)
                    throw exception::InvalidUsage("Server mode requires a port (-p <port>)");
                if (attrs.serverConfigFilepath.empty())
                    throw exception::InvalidUsage("Server mode requires a configuration file (--config <file>)");
                if (attrs.sceneFilepath.empty())
                    throw exception::InvalidUsage("Server mode requires a scene file path");
                if (!attrs.host.empty())
                    throw exception::InvalidUsage("Server mode must not include a host");
                break;

            case Mode::SELF:
                if (attrs.sceneFilepath.empty())
                    throw exception::InvalidUsage("Self mode requires a scene file path");
                if (!attrs.host.empty())
                    throw exception::InvalidUsage("Self mode must not include a host");
                if (attrs.port)
                    throw exception::InvalidUsage("Self mode must not include a port");
                break;
        }
    }

}
