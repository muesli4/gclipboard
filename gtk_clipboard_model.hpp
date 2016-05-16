#ifndef GTK_CLIPBOARD_MODEL
#define GTK_CLIPBOARD_MODEL

#include <gtkmm/clipboard.h>

#include "clipboard_model.hpp"

struct gtk_clipboard_model : clipboard_model
{
    gtk_clipboard_model();

    void clear_clipboard_entries();
    void select_active_entry(int n);

    private:

    std::vector<std::string> _text_entries;

    // clipboard used for mouse selection
    Glib::RefPtr<Gtk::Clipboard> _primary;
    sigc::connection _primary_con;

    // clipboard used for keyboard based selection
    Glib::RefPtr<Gtk::Clipboard> _clipboard;
    sigc::connection _clipboard_con;
};


#endif

