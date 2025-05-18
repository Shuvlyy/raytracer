#include "Parser.hpp"

#include "Exception/Exceptions/InvalidUsage.hpp"
#include "Exception/Exceptions/UnknownFlag.hpp"
#include "Exception/Exceptions/ValueOverflow.hpp"

#include <iostream>
#include <thread>
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
                        const int threads = std::stoi(value);

                        if (threads <= 0 || threads > std::numeric_limits<uint16_t>::max()) {
                            throw exception::ValueOverflow(threads);
                        }
                        _attributes.threadsAmount = threads;
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

                std::string host = _tokens[i];

                if (host == "localhost") {
                    host = "127.0.0.1";
                }
                _attributes.host = host;
            }
            else if (token == "-p") {
                if (++i >= _tokens.size())
                    throw exception::InvalidUsage("Expected port after -p");

                try {
                    const int port = std::stoi(_tokens[i]);

                    if (port <= 0 || port > std::numeric_limits<uint16_t>::max()) {
                        throw exception::ValueOverflow(port);
                    }
                    _attributes.port = port;
                } catch (...) {
                    throw exception::InvalidUsage("Invalid port number: " + _tokens[i]);
                }
            }
            else if (token == "-d") {
                _attributes.debugMode = true;
            }
            else if (token == "--no-preview") {
                _attributes.noPreview = true;
            }
            else if (token.starts_with("-")) {
                // Leave for processFlags to handle unknown flags like --help
                continue;
            }
            else {
                _attributes.sceneFilepaths.push_back(token);
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
                << "USAGE: ./raytracer [--mode program_mode] [--threads threads_amount] [--config config_filepath] [-h host] [-p port] [-d] scene_filepath" << std::endl
                << "\tprogram_mode\tProgram mode (`self`, `server`, `client`). Defaults to `self`." << std::endl
                << "\tthreads_amount\tNumber of threads that will be used to render the image. Defaults to `auto` (maximum threads available)." << std::endl
                << "\tconfig_filepath\tFile path of the server configuration. If mode is not set to `server`, an exception will be thrown." << std::endl
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
                if (!attrs.sceneFilepaths.empty())
                    throw exception::InvalidUsage("Client mode must not include a scene file path");
                if (attrs.noPreview)
                    std::cout << "Client mode does not support --no-preview. Ignoring." << std::endl;
                break;

            case Mode::SERVER:
                if (!attrs.port)
                    throw exception::InvalidUsage("Server mode requires a port (-p <port>)");
                if (attrs.serverConfigFilepath.empty())
                    throw exception::InvalidUsage("Server mode requires a configuration file (--config <file>)");
                if (attrs.sceneFilepaths.empty())
                    throw exception::InvalidUsage("Server mode requires a scene file path");
                if (!attrs.host.empty())
                    throw exception::InvalidUsage("Server mode must not include a host");
                break;

            case Mode::SELF:
                if (attrs.sceneFilepaths.empty())
                    throw exception::InvalidUsage("Self mode requires a scene file path");
                if (!attrs.host.empty())
                    throw exception::InvalidUsage("Self mode must not include a host");
                if (attrs.port)
                    throw exception::InvalidUsage("Self mode must not include a port");
                break;
        }
    }

}
