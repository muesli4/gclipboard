#include "gsettings_preferences_model.hpp"

char const * const history_size_name = "history-size";

gsettings_preferences_model::gsettings_preferences_model(Glib::ustring const & schema_id)
    : _settings_ref(Gio::Settings::create(schema_id, "/tmp/gclipboard/share/glib-2.0/schemas/"))
{
    _settings_ref->signal_changed(history_size_name).connect(
            [&](Glib::ustring const &)
            {
                emit_history_size_change(_settings_ref->get_int(history_size_name));
            }
    );
}

void gsettings_preferences_model::set_history_size(unsigned int new_size)
{
    _settings_ref->set_int(history_size_name, new_size);
}

void gsettings_preferences_model::init_view(preferences::view & v)
{
    v.on_history_size_change(_settings_ref->get_int(history_size_name));
}

