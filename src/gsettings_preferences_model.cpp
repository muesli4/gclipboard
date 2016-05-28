#include "gsettings_preferences_model.hpp"

char const * const history_size_name = "history-size";
char const * const session_restore_name = "session-restore";
char const * const session_data_name = "session-data";

gsettings_preferences_model::gsettings_preferences_model(Glib::ustring const & schema_id)
    : _settings_ref(Gio::Settings::create(schema_id))
    , _reset_session_data(false)
{
    // history-size
    _settings_ref->signal_changed(history_size_name).connect(
            [&](Glib::ustring const &)
            {
                emit_history_size_change(read_history_size());
            }
    );

    // session-restore
    _settings_ref->signal_changed(session_restore_name).connect(
            [&](Glib::ustring const &)
            {
                bool v = read_session_restore();

                if (v != _restore_session)
                {
                    _restore_session = v;
                    _reset_session_data = !_restore_session;
                    emit_session_restore_change(_restore_session);
                }
            }
    );

    _restore_session = read_session_restore();
}

gsettings_preferences_model::~gsettings_preferences_model()
{
    // reset all session data when we don't need it anymore
    if (_reset_session_data)
    {
        _settings_ref->reset(session_data_name);
    }
}

void gsettings_preferences_model::set_history_size(unsigned int new_size)
{
    _settings_ref->set_value(history_size_name, Glib::Variant<guint32>::create(new_size));
}

void gsettings_preferences_model::set_session_restore(bool restore)
{
    _settings_ref->set_boolean(session_restore_name, restore);
}

void gsettings_preferences_model::save_from(clipboard::model & m)
{
    if (_restore_session)
    {
        preferences::session_data sd;
        m.save(sd);

        // write to GSettings
        std::vector<Glib::VariantBase> tup;
        tup.emplace_back(Glib::Variant<guint32>::create(sd.current_id));

        std::vector<Glib::VariantBase> array;

        // there is apparently no way to build array types with non-basic types in Gtkmm
        std::vector<GVariant *> g_array;

        for (auto const & e : sd.entries)
        {
            std::vector<Glib::VariantBase> sub_tup;
            sub_tup.emplace_back(Glib::Variant<guint32>::create(e.first));
            sub_tup.emplace_back(Glib::Variant<std::string>::create(e.second));
            array.emplace_back(Glib::VariantContainerBase::create_tuple(sub_tup));
            g_array.emplace_back(array.back().gobj());
        }

        GVariant * gvarr = g_variant_new_array(g_variant_type_new("(uay)"), g_array.data(), g_array.size());

        // assumption: VariantBase takes ownerships of gvarr
        tup.emplace_back(Glib::VariantBase(gvarr));
        tup.emplace_back(
            Glib::VariantContainerBase::create_maybe(
                Glib::Variant<guint32>::variant_type(),
                sd.active_valid ? Glib::Variant<guint32>::create(sd.active_id) : Glib::Variant<guint32>()
            )
        );

        _settings_ref->set_value(session_data_name, Glib::VariantContainerBase::create_tuple(tup));
    }
}

void gsettings_preferences_model::restore_into(clipboard::model & m)
{
    using namespace Glib;
    if (_restore_session)
    {
        preferences::session_data sd;

        // read from GSettings
        VariantBase v;
        _settings_ref->get_value(session_data_name, v);
        VariantContainerBase c = VariantBase::cast_dynamic<VariantContainerBase>(v);
        sd.current_id = VariantBase::cast_dynamic<Variant<guint32>>(c.get_child(0)).get();

        {
            VariantContainerBase v_entries = VariantBase::cast_dynamic<VariantContainerBase>(c.get_child(1));

            std::size_t const entry_count = v_entries.get_n_children();
            for (unsigned int i = 0; i < entry_count; ++i)
            {
                VariantContainerBase tuple = VariantBase::cast_dynamic<VariantContainerBase>(v_entries.get_child(i));

                sd.entries.emplace_back(std::make_pair(
                        VariantBase::cast_dynamic<Variant<guint32>>(tuple.get_child(0)).get(),
                        VariantBase::cast_dynamic<Variant<std::string>>(tuple.get_child(1)).get()
                    )
                );
            }
        }
        {
            VariantContainerBase maybe_active_id = VariantBase::cast_dynamic<VariantContainerBase>(c.get_child(2));
            if (maybe_active_id.get_n_children() == 1)
            {
                sd.active_valid = true;
                sd.active_id = VariantBase::cast_dynamic<Variant<guint32>>(maybe_active_id.get_child()).get();
            }
            else
            {
                sd.active_valid = false;
            }
        }

        m.restore(sd);
    }
}

unsigned int gsettings_preferences_model::read_history_size()
{
    return _settings_ref->get_uint(history_size_name);
}

bool gsettings_preferences_model::read_session_restore()
{
    return _settings_ref->get_boolean(session_restore_name);
}

void gsettings_preferences_model::init_view(preferences::view & v)
{
    v.on_history_size_change(read_history_size());
    v.on_session_restore_change(_restore_session);
}

