#ifndef CLIPBOARD_VIEW_HPP
#define CLIPBOARD_VIEW_HPP

#include <string>

struct clipboard_view
{
    virtual void on_select_active_entry(int n) = 0;
    virtual void on_clear_clipboard_entries() = 0;
    virtual void on_add_clipboard_entry(std::string const & s) = 0;
    virtual void on_remove_oldest_entry() = 0;
};

#endif

