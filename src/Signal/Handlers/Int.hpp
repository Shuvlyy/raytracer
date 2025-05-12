#pragma once

#include "../IHandler.hpp"

namespace raytracer::signal::handler
{

    class Int final
        : public IHandler
    {
    public:
        void handle(App *app) const override;

        [[nodiscard]] int getSignalNumber() const override { return SIGINT; }
        [[nodiscard]] std::string getSignalName() const override { return "SIGINT"; }
    };

}
