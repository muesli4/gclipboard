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

    // methods
    virtual void clear() = 0;
    virtual void select_active(unsigned int n) = 0;
    virtual void remove(unsigned int n) = 0;

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

    unsigned int fresh_id();

    private:

    std::vector<clipboard_view *>::iterator find_ptr(clipboard_view * p);

    unsigned int _id_src;

    std::vector<clipboard_view *> _view_pointers;
};

#endif

