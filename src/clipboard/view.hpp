#ifndef GCLIPBOARD_CLIPBOARD_VIEW_HPP
#define GCLIPBOARD_CLIPBOARD_VIEW_HPP

#include <string>

#include "request_type.hpp"

namespace clipboard
{
    struct view
    {
        // the entry with the given id has been moved to the first position
        virtual void on_move_front(unsigned int id) = 0;

        // the entry with the given id has been selected and is now active
        virtual void on_select_active(unsigned int id) = 0;

        // the entry with the given id is now inactive
        virtual void on_unselect_active(unsigned int id) = 0;

        // the clipboard has been completely cleared and all entries have been removed
        virtual void on_clear() = 0;

        // a new entry has been added to the clipboard
        virtual void on_add(std::string const & s, unsigned int id) = 0;

        // an entry has been removed from the clipboard
        virtual void on_remove(unsigned int id) = 0;

        // the oldest entry has been dropped from the clipboard
        virtual void on_remove_oldest() = 0;

        // an entry has been modified
        virtual void on_change(unsigned int id, std::string const & s) = 0;

        virtual void on_freeze(request_type rt) = 0;
        virtual void on_thaw() = 0;
    };
}

#endif

