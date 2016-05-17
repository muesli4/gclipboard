#ifndef GTK_CLIPBOARD_MODEL
#define GTK_CLIPBOARD_MODEL

#include <boost/circular_buffer.hpp>
#include <gtkmm/clipboard.h>

#include "clipboard_model.hpp"


struct gtk_clipboard_model : clipboard_model
{
    gtk_clipboard_model(unsigned int buffer_size);

    void clear();
    void select_active(unsigned int n);

    ~gtk_clipboard_model();

    private:

    void handle_owner_change(GdkEventOwnerChange * e, bool & ignore_source, Glib::RefPtr<Gtk::Clipboard> source, bool & ignore_other, Glib::RefPtr<Gtk::Clipboard> other);

    void update_active_id(unsigned int id);

    typedef boost::circular_buffer<std::pair<std::string, unsigned int>> buffer_type;
    typedef buffer_type::iterator buffer_iterator;

    // the text buffer, where the first is the newest and back is the oldest
    buffer_type _text_buffer;


    // the active text in the clipboard
    int _active_id;
    bool _active_valid;

    // clipboard used for mouse selection
    Glib::RefPtr<Gtk::Clipboard> _primary;
    sigc::connection _primary_con;
    bool _ignore_primary;

    // clipboard used for keyboard based selection
    Glib::RefPtr<Gtk::Clipboard> _clipboard;
    sigc::connection _clipboard_con;
    bool _ignore_clipboard;
};


#endif

