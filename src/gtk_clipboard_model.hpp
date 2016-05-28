#ifndef GCLIPBOARD_GTK_CLIPBOARD_MODEL
#define GCLIPBOARD_GTK_CLIPBOARD_MODEL

#include <boost/circular_buffer.hpp>
#include <gtkmm/clipboard.h>
#include <mutex>

#include "clipboard/model.hpp"
#include "freezable/model.hpp"
#include "freezable/request_type.hpp"
#include "preferences/view.hpp"

struct gtk_clipboard_model : clipboard::model, freezable::model, preferences::view
{
    gtk_clipboard_model(unsigned int buffer_size);

    void disable() { _primary_con.disconnect(); _clipboard_con.disconnect(); }

    void clear();
    void select_active(unsigned int id);
    void remove(unsigned int id);
    void change(unsigned int id, std::string const & s);
    void freeze(freezable::request_type rt);
    void thaw(freezable::request_type rt);

    ~gtk_clipboard_model();

    protected:

    void init_view(clipboard::view & v);
    void init_view(freezable::view & v);

    void restore_template(std::vector<std::pair<unsigned int, std::string>> const & entries, bool active_valid, unsigned int active_id);
    void save_template(std::vector<std::pair<unsigned int, std::string>> & entries, bool & active_valid, unsigned int & active_id);

    private:

    void on_history_size_change(unsigned int new_size);
    void on_session_restore_change(bool restore);

    void setup_primary_default_owner_change_handler();
    void setup_clipboard_default_owner_change_handler();
    void update_primary_silently(std::string const & s);
    void update_clipboard_silently(std::string const & s);
    void handle_owner_change(GdkEventOwnerChange * e, Glib::RefPtr<Gtk::Clipboard> source, void (gtk_clipboard_model::*update_other_silently)(std::string const &));

    void update_active_id(unsigned int id);

    typedef boost::circular_buffer<std::pair<std::string, unsigned int>> buffer_type;
    typedef buffer_type::iterator iterator;

    iterator find_id(unsigned int id);

    // the text buffer, where the first is the newest and back is the oldest
    buffer_type _text_buffer;

    // the active text in the clipboard
    unsigned int _active_id;
    bool _active_valid;

    // clipboard used for mouse selection
    Glib::RefPtr<Gtk::Clipboard> _primary_ref;
    sigc::connection _primary_con;

    // clipboard used for keyboard based selection
    Glib::RefPtr<Gtk::Clipboard> _clipboard_ref;
    sigc::connection _clipboard_con;

    guint32 _last_time;

    std::mutex _owner_change_mutex;

    bool _frozen;

    freezable::request_type _frozen_request_type;
};


#endif

