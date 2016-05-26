#ifndef GCLIPBOARD_CLIPBOARD_MODEL_HPP
#define GCLIPBOARD_CLIPBOARD_MODEL_HPP

// TODO make interface ?

#include <vector>

#include "view.hpp"

namespace clipboard {
    struct model
    {
        model();

        // view handling
        void add_view(view & v);
        void remove_view(view & v);

        // clear all entries
        virtual void clear() = 0;
        // select the active entry, making it available in the system clipboard
        virtual void select_active(unsigned int id) = 0;
        virtual void remove(unsigned int id) = 0;
        virtual void change(unsigned int id, std::string const & s) = 0;
        // disallow modification of the clipboard
        virtual void freeze(request_type rt) = 0;
        virtual void thaw(request_type rt) = 0;

        virtual ~model();

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
        void emit_freeze(request_type rt);
        void emit_thaw();

        // initialize a view with the current state of the model
        virtual void init_view(view & v) = 0;

        unsigned int fresh_id();

        private:

        std::vector<view *>::iterator find_ptr(view * p);

        unsigned int _id_src;

        std::vector<view *> _view_pointers;
    };
}

#endif

