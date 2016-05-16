#ifndef CLIPBOARD_MODEL_HPP
#define CLIPBOARD_MODEL_HPP

// TODO make interface ?

#include <vector>

#include "clipboard_view.hpp"

struct clipboard_model
{
    // view handling
    void add_view(clipboard_view & v);
    void remove_view(clipboard_view & v);

    // methods
    virtual void clear_clipboard_entries() = 0;
    virtual void select_active_entry(int n) = 0;

    protected:

    // view signals
    void emit_add_clipboard_entry(std::string const & s);
    void emit_clear_clipboard_entries();
    void emit_select_active_entry(int n);

    private:

    std::vector<clipboard_view *>::iterator find_ptr(clipboard_view * p);

    std::vector<clipboard_view *> _view_pointers;
};

#endif

