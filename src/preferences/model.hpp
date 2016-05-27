#ifndef GCLIPBOARD_PREFERENCES_MODEL_HPP
#define GCLIPBOARD_PREFERENCES_MODEL_HPP

#include "../util/model_template.hpp"
#include "view.hpp"
#include "preferences.hpp"
#include "../clipboard/model.hpp"

namespace preferences
{
    struct model : util::model_template<view>
    {
        virtual void set_history_size(unsigned int new_size) = 0;
        virtual void set_session_restore(bool restore) = 0;

        virtual void save_from(clipboard::model & m) = 0;
        virtual void restore_into(clipboard::model & m) = 0;

        protected:

        void emit_history_size_change(unsigned int new_size);
        void emit_session_restore_change(bool restore);
    };
}

#endif

