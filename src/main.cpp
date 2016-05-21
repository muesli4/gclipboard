#include <gtkmm/statusicon.h>
#include <gtkmm/application.h>
#include <gtkmm/menu.h>
#include <gtkmm/imagemenuitem.h>
#include <gtkmm/checkmenuitem.h>
#include <gtkmm/separatormenuitem.h>
#include <gtkmm/aboutdialog.h>
#include <gtkmm/stock.h>

#include "default_clipboard_controller.hpp"
#include "gtk_clipboard_model.hpp"
#include "gtk_history_menu_view.hpp"
#include "gtk_history_window_view.hpp"

#include "gettext.h"

// TODO settings
// TODO save session
// TODO add unicode symbols in lines to show special whitespace characters:
//         ␣ for space
//         ↵ for newline
//         ↹ for tab

char const * const icon_name = "edit-paste";

struct enabled_menu_item_view : public Gtk::CheckMenuItem, clipboard_view
{
    enabled_menu_item_view(Glib::ustring const & label, clipboard_controller & ctrl)
        : Gtk::CheckMenuItem(label)
        , _ctrl(ctrl)
    {
        this->set_active();
        setup_default_signal_toggled_handler();
    }

    void on_move_front(unsigned int id)
    {
    }

    void on_select_active(unsigned int id)
    {
    }

    void on_unselect_active(unsigned int id)
    {
    }

    void on_clear()
    {
    }

    void on_add(std::string const & s, unsigned int id)
    {
    }

    void on_remove(unsigned int id)
    {
    }

    void on_remove_oldest()
    {
    }

    void on_change(unsigned int id, std::string const & s)
    {
    }

    void on_freeze(request_type rt)
    {
        set_active_silently(false);
    }

    void on_thaw()
    {
        set_active_silently(true);
    }

    private:

    void setup_default_signal_toggled_handler()
    {
        _c = this->signal_toggled().connect(
            [&]()
            {
                if (this->get_active())
                {
                    _ctrl.clipboard_thaw(request_type::USER);
                }
                else
                {
                    _ctrl.clipboard_freeze(request_type::USER);
                }
            }
        );
    }

    void set_active_silently(bool active)
    {
        _c.disconnect();
        _c = this->signal_toggled().connect(
            [&]()
            {
                _c.disconnect();
                setup_default_signal_toggled_handler();
            }
        );
        this->set_active(active);
    }

    clipboard_controller & _ctrl;
    sigc::connection _c;
};

int main(int argc, char ** argv)
{
    // setup localization
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);

    auto app_ref = Gtk::Application::create(argc, argv, "org.gclipboard");
    auto status_icon_ref = Gtk::StatusIcon::create(icon_name);

    // a clipboard model which uses the tools provided by Gtkmm
    gtk_clipboard_model m(10);

    default_clipboard_controller ctrl(m);

    // left click menu
    gtk_history_menu_view history_menu(ctrl);

    // history window
    gtk_history_window_view history_window(ctrl);

    // connect to model
    m.add_view(history_menu);
    m.add_view(history_window);

    Gtk::AboutDialog about_dialog;
    about_dialog.set_program_name(PACKAGE);
    about_dialog.set_version(PACKAGE_VERSION);
    about_dialog.set_logo_icon_name(icon_name);
    about_dialog.set_comments(gettext("A clean and simple clipboard manager, which has been influenced by other popular clipboard managers."));
    about_dialog.set_copyright("Copyright © 2016 Moritz Bruder");
    about_dialog.set_icon_name(icon_name);
    //about_dialog.set_transient_for(history_window);
    // TODO finish

    // right click menu
    Gtk::Menu right_menu;
    Gtk::ImageMenuItem clear_item(Gtk::Stock::CLEAR);
    Gtk::ImageMenuItem edit_history_item(Gtk::Stock::EDIT);
    enabled_menu_item_view enabled_item(gettext("Enable history"), ctrl);
    Gtk::SeparatorMenuItem sep_item;
    Gtk::ImageMenuItem settings_item(Gtk::Stock::PREFERENCES);
    Gtk::ImageMenuItem about_item(Gtk::Stock::ABOUT);
    Gtk::SeparatorMenuItem sep2_item;
    Gtk::ImageMenuItem quit_item(Gtk::Stock::QUIT);

    clear_item.signal_activate().connect([&](){ ctrl.clipboard_clear(); });
    edit_history_item.signal_activate().connect([&](){ history_window.show(); });
    m.add_view(enabled_item);
    about_item.signal_activate().connect([&](){ about_dialog.run(); about_dialog.hide(); });
    quit_item.signal_activate().connect([app_ref](){ app_ref->release(); });

    right_menu.append(clear_item);
    right_menu.append(edit_history_item);
    right_menu.append(enabled_item);
    right_menu.append(sep_item);
    right_menu.append(settings_item);
    right_menu.append(about_item);
    right_menu.append(sep2_item);
    right_menu.append(quit_item);

    right_menu.show_all();

    status_icon_ref->signal_button_press_event().connect(
        [&, app_ref, status_icon_ref](GdkEventButton * e)
        {
            if (e->button == 1)
            {
                status_icon_ref->popup_menu_at_position(history_menu, 1, e->time);
            }
            else if (e->button == 3)
            {
                status_icon_ref->popup_menu_at_position(right_menu, 3, e->time);
            }
            return true;
        }
    );

    app_ref->hold();
    return app_ref->run(argc, argv);
}

