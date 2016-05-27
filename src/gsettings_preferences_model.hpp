#ifndef GCLIPBOARD_GSETTINGS_PREFERENCES_MODEL_HPP
#define GCLIPBOARD_GSETTINGS_PREFERENCES_MODEL_HPP

#include <giomm/settings.h>

#include "preferences/model.hpp"

struct gsettings_preferences_model : preferences::model
{

    gsettings_preferences_model(Glib::ustring const & schema_id);

    void set_history_size(unsigned int new_size);

    private:

    void init_view(preferences::view & v);

    Glib::RefPtr<Gio::Settings> _settings_ref;
};

#endif

