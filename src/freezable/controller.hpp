#ifndef GCLIPBOARD_FREEZABLE_CONTROLLER_HPP
#define GCLIPBOARD_FREEZABLE_CONTROLLER_HPP

#include <string>

#include "request_type.hpp"

namespace freezable
{
    struct controller
    {
        virtual void freezable_freeze(request_type rt) = 0;
        virtual void freezable_thaw(request_type rt) = 0;
    };
}

#endif

