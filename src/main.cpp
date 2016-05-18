#include <gtkmm/statusicon.h>
#include <gtkmm/application.h>
#include <gtkmm/menu.h>
#include <gtkmm/separatormenuitem.h>

#include "default_clipboard_controller.hpp"
#include "gtk_clipboard_model.hpp"
#include "gtk_left_menu_view.hpp"
#include "gtk_history_window_view.hpp"

#include "gettext.h"

// TODO settings
// TODO about dialog
// TODO manage contents
// TODO save session
// TODO publish initial changes to view from model

int main(int argc, char ** argv)
{
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);

    auto app_ref = Gtk::Application::create(argc, argv, "org.gclipboard");
    auto status_icon_ref = Gtk::StatusIcon::create("edit-paste");

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

    // right click menu
    Gtk::Menu right_menu;
    Gtk::MenuItem clear_item(gettext("Clear"));
    Gtk::MenuItem edit_history_item(gettext("Edit history"));
    Gtk::SeparatorMenuItem sep_item;
    Gtk::MenuItem quit_item(gettext("Quit"));

    clear_item.signal_activate().connect([&](){ ctrl.clipboard_clear(); });
    edit_history_item.signal_activate().connect([&](){ history_window.show(); });
    quit_item.signal_activate().connect([app_ref](){ app_ref->release(); });

    clear_item.show();
    edit_history_item.show();
    sep_item.show();
    quit_item.show();

    right_menu.append(clear_item);
    right_menu.append(edit_history_item);
    right_menu.append(sep_item);
    right_menu.append(quit_item);

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

