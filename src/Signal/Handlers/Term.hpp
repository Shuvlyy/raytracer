#pragma once

#include "../IHandler.hpp"

namespace raytracer::signal::handler
{

    class Term final
        : public IHandler
    {
    public:
        void handle(App *app) const override;

        [[nodiscard]] int getSignalNumber() const override { return SIGTERM; }
        [[nodiscard]] std::string getSignalName() const override { return "SIGTERM"; }
    };

}
