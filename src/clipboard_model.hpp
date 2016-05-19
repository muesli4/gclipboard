#ifndef GCLIPBOARD_CLIPBOARD_MODEL_HPP
#define GCLIPBOARD_CLIPBOARD_MODEL_HPP

// TODO make interface ?

#include <vector>

#include "clipboard_view.hpp"

struct clipboard_model
{
    clipboard_model();

    // view handling
    void add_view(clipboard_view & v);
    void remove_view(clipboard_view & v);

    // clear all entries
    virtual void clear() = 0;
    // select the active entry, making it available in the system clipboard
    virtual void select_active(unsigned int id) = 0;
    virtual void remove(unsigned int id) = 0;
    virtual void change(unsigned int id, std::string const & s) = 0;
    // disallow modification of the clipboard
    virtual void freeze() = 0;
    virtual void thaw() = 0;

    virtual ~clipboard_model();

    protected:

    // signals used to message all views about changes in the model
    void emit_move_front(unsigned int id);
    void emit_add(std::string const & s, unsigned int id);
    void emit_clear();
    void emit_select_active(unsigned int id);
    void emit_unselect_active(unsigned int id);
    void emit_remove(unsigned int id);
    void emit_remove_oldest();
    void emit_change(unsigned int id, std::string const & s);

    // initialize a view with the current state of the model
    virtual void init_view(clipboard_view & v) = 0;

    unsigned int fresh_id();

    private:

    std::vector<clipboard_view *>::iterator find_ptr(clipboard_view * p);

    unsigned int _id_src;

    std::vector<clipboard_view *> _view_pointers;
};

#endif

