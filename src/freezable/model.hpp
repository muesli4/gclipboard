#ifndef GCLIPBOARD_FREEZABLE_MODEL_HPP
#define GCLIPBOARD_FREEZABLE_MODEL_HPP

#include "../util/model_template.hpp"
#include "view.hpp"

namespace freezable
{
    struct model : util::model_template<freezable::view>
    {
        virtual void freeze(request_type rt) = 0;
        virtual void thaw(request_type rt) = 0;

        void emit_freeze(request_type rt);
        void emit_thaw();
    };
}

#endif

