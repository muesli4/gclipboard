#ifndef GCLIPBOARD_PREFERENCES_MODEL_HPP
#define GCLIPBOARD_PREFERENCES_MODEL_HPP

#include "../util/model_template.hpp"
#include "view.hpp"
#include "preferences.hpp"

namespace preferences
{
    struct model : util::model_template<view>
    {
        virtual void set_history_size(unsigned int new_size) = 0;

        protected:

        void emit_history_size_change(unsigned int new_size);
    };
}

#endif

