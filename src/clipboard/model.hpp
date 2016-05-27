#ifndef GCLIPBOARD_CLIPBOARD_MODEL_HPP
#define GCLIPBOARD_CLIPBOARD_MODEL_HPP

#include <vector>

#include "view.hpp"

#include "../util/model_template.hpp"

namespace clipboard
{
    struct model : util::model_template<view>
    {
        model();

        // clear all entries
        virtual void clear() = 0;
        // select the active entry, making it available in the system clipboard
        virtual void select_active(unsigned int id) = 0;
        virtual void remove(unsigned int id) = 0;
        virtual void change(unsigned int id, std::string const & s) = 0;

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

        unsigned int fresh_id();

        private:

        unsigned int _id_src;
    };
}

#endif

