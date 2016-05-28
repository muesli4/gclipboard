#ifndef GCLIPBOARD_GSETTINGS_PREFERENCES_MODEL_HPP
#define GCLIPBOARD_GSETTINGS_PREFERENCES_MODEL_HPP

#include <giomm/settings.h>

#include "preferences/model.hpp"

struct gsettings_preferences_model : preferences::model
{

    gsettings_preferences_model(Glib::ustring const & schema_id);
    ~gsettings_preferences_model();

    void set_history_size(unsigned int new_size);
    void set_session_restore(bool restore);

    void save_from(clipboard::model & m);
    void restore_into(clipboard::model & m);

    private:

    unsigned int read_history_size();
    bool read_session_restore();

    void init_view(preferences::view & v);

    Glib::RefPtr<Gio::Settings> _settings_ref;

    bool _restore_session;

    bool _reset_session_data;
};

#endif

