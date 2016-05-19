#include <gtkmm/stock.h>

#include "gtk_edit_entry_dialog.hpp"
#include "gettext.h"

gtk_edit_entry_dialog::gtk_edit_entry_dialog(std::vector<std::pair<std::string, unsigned int>> const & es)
    : _buttons(es.size())
    , _text_views(es.size())
    , _row_boxes(es.size())
{
    auto vbox_ptr = this->get_vbox();
    for (std::size_t n = 0; n < es.size(); ++n)
    {
        auto p = es[n];

        _ids.push_back(p.second);

        auto & text_view = _text_views[n];
        auto text_buffer_ref = text_view.get_buffer();
        text_buffer_ref->set_text(p.first);
        auto & button = _buttons[n];
        text_buffer_ref->signal_changed().connect([&](){ button.set_active(); });
        
        auto & row_box = _row_boxes[n];
        row_box.pack_start(button, false, false, 10);
        // TODO horizontally wrap in ScrolledWindow
        row_box.pack_end(text_view, true, true);
        vbox_ptr->pack_end(row_box, true, true, 10);
    }
    vbox_ptr->show_all();
    this->add_button(Gtk::Stock::APPLY, Gtk::RESPONSE_APPLY);
    this->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
}

std::vector<std::pair<std::string, unsigned int>> gtk_edit_entry_dialog::get_changes()
{
    std::vector<std::pair<std::string, unsigned int>> cs;

    for (std::size_t n = 0; n < _buttons.size(); ++n)
    {
        if (_buttons[n].get_active())
        {
            cs.emplace_back(std::make_pair(_text_views[n].get_buffer()->get_text(), _ids[n]));
        }
    }

    return cs;
}

