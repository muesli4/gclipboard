#ifndef GCLIPBOARD_PREFERENCES_MODEL_HPP
#define GCLIPBOARD_PREFERENCES_MODEL_HPP

#include "../util/model_template.hpp"
#include "view.hpp"

namespace preferences
{
    struct model : util::model_template<preferences::view>
    {
        void emit_history_size_change(unsigned int new_size);
    };
}

#endif

