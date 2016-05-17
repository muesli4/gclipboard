#include <vector>
#include <deque>
#include <iostream>

#include <gtkmm/statusicon.h>
#include <gtkmm/application.h>
#include <gtkmm/menu.h>
#include <gtkmm/separatormenuitem.h>
#include <gtkmm/clipboard.h>
#include <gdkmm.h>
#include <glibmm.h>

#include "default_clipboard_controller.hpp"
#include "gtk_clipboard_model.hpp"

// TODO maybe remove public inheritance of Menu
struct gtk_left_menu : public Gtk::Menu, clipboard_view
{
    gtk_left_menu(clipboard_controller & ctrl)
        : _empty_indicator_menu_item("No items")
        , _ctrl(ctrl)
    {
        _empty_indicator_menu_item.set_sensitive(false);
        _empty_indicator_menu_item.show();
        this->append(_empty_indicator_menu_item);
    }

    // TODO change signature to contain last active
    void on_select_active(unsigned int id)
    {
        std::cout << "view should select entry " << id << std::endl;

        auto it = find_id(id);

        if (it != _menu_items.end())
        {
            auto & mi = it->first;
            mi.set_label("<b>" + mi.get_label() + "</b>");
        }
    }

    void on_unselect_active(unsigned int id)
    {
        std::cout << "view should unselect entry " << id << std::endl;
        auto it = find_id(id);

        if (it != _menu_items.end())
        {
            auto & mi = it->first;
            auto const & text = mi.get_label();
            mi.set_label(text.substr(3, text.size() - 7)); // TODO check if wrong
        }
    }

    void on_clear()
    {
        for (auto & p : _menu_items)
        {
            this->remove(p.first);
        }
        int size = _menu_items.size();
        _menu_items.clear();
        if (size != 0)
            show_empty_indicator();
    }

    void on_add(std::string const & s, unsigned int id);

    void on_remove_oldest()
    {
        this->remove(_menu_items.back().first);
        _menu_items.pop_back();

        if (_menu_items.empty())
        {
            show_empty_indicator();
        }
    }

    private:

    static void enable_pango_markup(Gtk::MenuItem & mi)
    {
        auto l = dynamic_cast<Gtk::Label *>(mi.get_child());
        if (l != 0)
        {
            l->set_use_markup();
        }
    }

    Gtk::MenuItem _empty_indicator_menu_item;

    void hide_empty_indicator()
    {
        this->remove(_empty_indicator_menu_item);
    }

    void show_empty_indicator()
    {
        this->append(_empty_indicator_menu_item);
    }

    typedef std::deque<std::pair<Gtk::MenuItem, unsigned int>> buffer_type;
    typedef buffer_type::iterator buffer_iterator;

    buffer_iterator find_id(unsigned int id)
    {
        return std::find_if(_menu_items.begin(), _menu_items.end(), [id](std::pair<Gtk::MenuItem, unsigned int> const & p){ return p.second == id; });
    }

    clipboard_controller & _ctrl;

    // new items are inserted into the back
    buffer_type _menu_items;
};

void gtk_left_menu::on_add(std::string const & s, unsigned int id)
{
    hide_empty_indicator();
    // TODO trim string to length and replace newlines
    _menu_items.emplace_front(std::make_pair(Glib::Markup::escape_text(s.substr(0, 20)), id));
    Gtk::MenuItem & mi = _menu_items.front().first;

    enable_pango_markup(mi);

    mi.signal_activate().connect(
        [this, id]()
        {
            this->_ctrl.clipboard_select_active(id);
        }
    );

    mi.show();
    // add the new item on top
    this->prepend(mi);
}

int main(int argc, char ** argv)
{
    auto app = Gtk::Application::create(argc, argv, "org.gclipboard");
    auto status_icon = Gtk::StatusIcon::create("gtk-about");

    gtk_clipboard_model m(10);

    default_clipboard_controller ctrl(m);

    gtk_left_menu left_menu(ctrl);

    m.add_view(left_menu);

    Gtk::Menu right_menu;

    Gtk::MenuItem clear_item("Clear");
    clear_item.signal_activate().connect([&](){ ctrl.clipboard_clear(); });
    clear_item.show();
    Gtk::SeparatorMenuItem sep_item;
    sep_item.show();
    Gtk::MenuItem quit_item("Quit");
    quit_item.signal_activate().connect([app](){ app->release(); });
    quit_item.show();

    right_menu.append(clear_item);
    right_menu.append(sep_item);
    right_menu.append(quit_item);


    status_icon->signal_button_release_event().connect(
        [&, app, status_icon](GdkEventButton * e)
        {
            if (e->button == 1)
            {
                status_icon->popup_menu_at_position(left_menu, 1, e->time);
            }
            else if (e->button == 3)
            {
                status_icon->popup_menu_at_position(right_menu, 3, e->time);
            }
            return true;
        }
    );

    app->hold();
    return app->run(argc, argv);
}

