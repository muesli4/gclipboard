#ifndef GCLIPBOARD_FREEZEABLE_VIEW_HPP
#define GCLIPBOARD_FREEZEABLE_VIEW_HPP

#include "request_type.hpp"

namespace freezable
{
    struct view
    {
        virtual void on_freeze(request_type rt) = 0;
        virtual void on_thaw() = 0;
    };
}

#endif

