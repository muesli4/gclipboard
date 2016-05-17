#ifndef CLIPBOARD_VIEW_HPP
#define CLIPBOARD_VIEW_HPP

#include <string>

struct clipboard_view
{
    virtual void on_select_active(unsigned int id) = 0;
    virtual void on_unselect_active(unsigned int id) = 0;
    virtual void on_clear() = 0;
    virtual void on_add(std::string const & s, unsigned int id) = 0;
    virtual void on_remove_oldest() = 0;
};

#endif

