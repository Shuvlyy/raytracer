#pragma once

#include "Parser/Parser.hpp"
#include "App/Self/Self.hpp"
#include "App/Server/Server.hpp"
#include "App/Client/Client.hpp"

#include <memory>

namespace raytracer::app
{

    static std::unique_ptr<App>
    fromAttributes
    (
        const Attributes &attributes
    )
    {
        if (attributes.programMode == Mode::SELF) {
            return std::make_unique<Self>(attributes);
        }
        if (attributes.programMode == Mode::SERVER) {
            return std::make_unique<Server>(attributes);
        }
        if (attributes.programMode == Mode::CLIENT) {
            return std::make_unique<Client>(attributes);
        }
        throw; // TODO: Throw exception
    }

}
