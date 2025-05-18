#pragma once

#include "Signal/IHandler.hpp"

namespace raytracer::signal::handler
{

    class Pipe final
        : public IHandler
    {
    public:
        void handle(App *app) const override;

        [[nodiscard]] int getSignalNumber() const override { return SIGPIPE; }
        [[nodiscard]] std::string getSignalName() const override { return "SIGPIPE"; }
    };

}
