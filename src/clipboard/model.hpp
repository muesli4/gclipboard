#ifndef GCLIPBOARD_CLIPBOARD_MODEL_HPP
#define GCLIPBOARD_CLIPBOARD_MODEL_HPP

#include <string>
#include <vector>

#include "view.hpp"

#include "../util/model_template.hpp"
#include "../preferences/session_data.hpp"

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

        void restore(preferences::session_data const & sd);
        void save(preferences::session_data & sd);

        protected:

        // TODO maybe refactor into a subclass like restorable_clipboard
        virtual void restore_template(std::vector<std::pair<unsigned int, std::string>> const & entries, bool active_valid, unsigned int active_id) = 0;
        virtual void save_template(std::vector<std::pair<unsigned int, std::string>> & entries, bool & active_valid, unsigned int & active_id) = 0;

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

