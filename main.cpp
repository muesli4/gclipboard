#include <gtkmm/statusicon.h>
#include <gtkmm/application.h>
#include <gtkmm/menu.h>
#include <gtkmm/separatormenuitem.h>

#include "default_clipboard_controller.hpp"
#include "gtk_clipboard_model.hpp"
#include "gtk_left_menu_view.hpp"

// TODO settings
// TODO about dialog
// TODO manage contents
// TODO save session

int main(int argc, char ** argv)
{
    auto app_ref = Gtk::Application::create(argc, argv, "org.gclipboard");
    auto status_icon_ref = Gtk::StatusIcon::create("edit-paste");

    // a clipboard model which uses the tools provided by Gtkmm
    gtk_clipboard_model m(10);

    default_clipboard_controller ctrl(m);

    // left click menu
    gtk_left_menu_view left_menu(ctrl);

    // connect to model
    m.add_view(left_menu);

    // right click menu
    Gtk::Menu right_menu;
    Gtk::MenuItem clear_item("Clear");
    Gtk::SeparatorMenuItem sep_item;
    Gtk::MenuItem quit_item("Quit");

    clear_item.signal_activate().connect([&](){ ctrl.clipboard_clear(); });
    quit_item.signal_activate().connect([app_ref](){ app_ref->release(); });

    clear_item.show();
    sep_item.show();
    quit_item.show();

    right_menu.append(clear_item);
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

