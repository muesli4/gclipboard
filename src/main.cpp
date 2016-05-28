#include <gtkmm/statusicon.h>
#include <gtkmm/application.h>
#include <gtkmm/menu.h>
#include <gtkmm/imagemenuitem.h>
#include <gtkmm/separatormenuitem.h>
#include <gtkmm/aboutdialog.h>
#include <gtkmm/stock.h>

#include "default_clipboard_controller.hpp"
#include "gtk_clipboard_model.hpp"
#include "gtk_history_menu_view.hpp"
#include "gtk_history_window_view.hpp"
#include "gtk_enabled_menu_item_view.hpp"
#include "gsettings_preferences_model.hpp"
#include "gtk_preferences_window.hpp"
#include "default_preferences_controller.hpp"

#include "gettext.h"

#include <iostream>

// TODO add unicode symbols in lines to show special whitespace characters (as optional feature):
//         ␣ for space
//         ↵ for newline
//         ↹ for tab

char const * const icon_name = "edit-paste";
char const * const appid = "com.github.muesli4.gclipboard";

int main(int argc, char ** argv)
{
    // setup localization
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);

    auto app_ref = Gtk::Application::create(argc, argv, appid);

    app_ref->register_application();

    // allow only one instance
    if (!app_ref->is_remote())
    {
        auto status_icon_ref = Gtk::StatusIcon::create(icon_name);

        gsettings_preferences_model gsettings_prefs(appid);

        default_preferences_controller pref_ctrl(gsettings_prefs);

        gtk_preferences_window pref_window(pref_ctrl);
        util::add_view<preferences::view>(gsettings_prefs, pref_window);

        // a clipboard model which uses the Gtk::Clipboard class provided by
        // Gtkmm to manage both clipboards
        gtk_clipboard_model clipboard_model(10);

        util::add_view<preferences::view>(gsettings_prefs, clipboard_model);

        // restore session
        gsettings_prefs.restore_into(clipboard_model);

        default_clipboard_controller ctrl(clipboard_model, clipboard_model);

        // left click menu
        gtk_history_menu_view history_menu(ctrl);

        // history window
        gtk_history_window_view history_window(ctrl, ctrl);

        // connect to model
        util::add_view<clipboard::view>(clipboard_model, history_menu);
        util::add_view<clipboard::view>(clipboard_model, history_window);
        util::add_view<freezable::view>(clipboard_model, history_window);

        Gtk::AboutDialog about_dialog;
        about_dialog.set_program_name(PACKAGE);
        about_dialog.set_version(PACKAGE_VERSION);
        about_dialog.set_logo_icon_name(icon_name);
        about_dialog.set_comments(gettext("A clean and simple clipboard manager, which has been influenced by other popular clipboard managers."));
        about_dialog.set_copyright("Copyright © 2016 Moritz Bruder");
        about_dialog.set_icon_name(icon_name);

        // right click menu
        Gtk::Menu right_menu;
        Gtk::ImageMenuItem clear_item(Gtk::Stock::CLEAR);
        Gtk::ImageMenuItem edit_history_item(Gtk::Stock::EDIT);
        gtk_enabled_menu_item_view enabled_item(gettext("Enable history"), ctrl);
        Gtk::SeparatorMenuItem sep_item;
        Gtk::ImageMenuItem settings_item(Gtk::Stock::PREFERENCES);
        Gtk::ImageMenuItem about_item(Gtk::Stock::ABOUT);
        Gtk::SeparatorMenuItem sep2_item;
        Gtk::ImageMenuItem quit_item(Gtk::Stock::QUIT);

        clear_item.signal_activate().connect([&](){ ctrl.clipboard_clear(); });
        edit_history_item.signal_activate().connect([&](){ history_window.show(); });
        util::add_view<freezable::view>(clipboard_model, enabled_item);
        about_item.signal_activate().connect(
            [&]()
            {
                app_ref->add_window(about_dialog);
                about_dialog.run();
                about_dialog.hide();
                app_ref->remove_window(about_dialog);
            }
        );
        settings_item.signal_activate().connect(
            [&]()
            {
                pref_window.show();
            }
        );
        sigc::connection quit_con = quit_item.signal_activate().connect(
            [&]()
            {
                app_ref->release();
                quit_con.disconnect();
                about_dialog.response(Gtk::RESPONSE_DELETE_EVENT);
                quit_item.set_sensitive(false);
            }
        );

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
        int result = app_ref->run(argc, argv);
        // save session
        gsettings_prefs.save_from(clipboard_model);
        return result;
    }
}

