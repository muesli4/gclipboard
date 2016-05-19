#include <gtkmm/statusicon.h>
#include <gtkmm/application.h>
#include <gtkmm/menu.h>
#include <gtkmm/imagemenuitem.h>
#include <gtkmm/separatormenuitem.h>
#include <gtkmm/aboutdialog.h>
#include <gtkmm/stock.h>

#include "default_clipboard_controller.hpp"
#include "gtk_clipboard_model.hpp"
#include "gtk_left_menu_view.hpp"
#include "gtk_history_window_view.hpp"

#include "gettext.h"

// TODO settings
// TODO save session
// TODO search history

char const * const icon_name = "edit-paste";

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
    gtk_left_menu_view left_menu(ctrl);

    // history window
    gtk_history_window_view history_window(ctrl);

    // connect to model
    m.add_view(left_menu);
    m.add_view(history_window);

    Gtk::AboutDialog about_dialog;
    about_dialog.set_program_name(PACKAGE);
    about_dialog.set_version(PACKAGE_VERSION);
    about_dialog.set_logo_icon_name(icon_name);
    about_dialog.set_comments(gettext("A simplistic clipboard manager."));
    about_dialog.set_copyright("Copyright © 2016 Moritz Bruder");
    about_dialog.set_icon_name(icon_name);
    //about_dialog.set_transient_for(history_window);
    // TODO finish

    // right click menu
    Gtk::Menu right_menu;
    Gtk::ImageMenuItem clear_item(Gtk::Stock::CLEAR);
    Gtk::ImageMenuItem edit_history_item(Gtk::Stock::EDIT);
    Gtk::SeparatorMenuItem sep_item;
    Gtk::ImageMenuItem settings_item(Gtk::Stock::PREFERENCES);
    Gtk::ImageMenuItem about_item(Gtk::Stock::ABOUT);
    Gtk::SeparatorMenuItem sep2_item;
    Gtk::ImageMenuItem quit_item(Gtk::Stock::QUIT);

    clear_item.signal_activate().connect([&](){ ctrl.clipboard_clear(); });
    edit_history_item.signal_activate().connect([&](){ history_window.show(); });
    about_item.signal_activate().connect([&](){ about_dialog.run(); about_dialog.hide(); });
    quit_item.signal_activate().connect([app_ref](){ app_ref->release(); });

    right_menu.append(clear_item);
    right_menu.append(edit_history_item);
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
                status_icon_ref->popup_menu_at_position(left_menu, 1, e->time);
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

