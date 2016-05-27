#include "gsettings_preferences_model.hpp"

#include <iostream>

char const * const history_size_name = "history-size";
char const * const session_restore_name = "session-restore";

gsettings_preferences_model::gsettings_preferences_model(Glib::ustring const & schema_id)
    : _settings_ref(Gio::Settings::create(schema_id))
{
    // history-size
    _settings_ref->signal_changed(history_size_name).connect(
            [&](Glib::ustring const &)
            {
                emit_history_size_change(get_history_size());
            }
    );

    // session-restore
    _settings_ref->signal_changed(session_restore_name).connect(
            [&](Glib::ustring const &)
            {
                _restore_session = get_session_restore();
                emit_session_restore_change(_restore_session);
            }
    );

    _restore_session = get_session_restore();
}

void gsettings_preferences_model::set_history_size(unsigned int new_size)
{
    // FIXME typo in gtkmm?
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

        tup.emplace_back(Glib::VariantBase(gvarr));
        tup.emplace_back(
            Glib::VariantContainerBase::create_maybe(
                Glib::Variant<guint32>::variant_type(),
                sd.active_valid ? Glib::Variant<guint32>::create(sd.active_id) : Glib::Variant<guint32>()
            )
        );

        _settings_ref->set_value("session-data", Glib::VariantContainerBase::create_tuple(tup));

        Glib::VariantBase v = Glib::VariantContainerBase::create_tuple(tup);
        std::cout << (v ? "worked: " : "fail") << v.get_type_string() << " . " << v.print() << std::endl;
        std::cout.flush();

        // TODO ?
        //g_free(gvarr);
    }
}

void gsettings_preferences_model::restore_into(clipboard::model & m)
{
    using namespace Glib;
    if (_restore_session)
    {
        std::cout << "loading session data" << std::endl;
        preferences::session_data sd;

        // read from GSettings
        VariantBase v;
        _settings_ref->get_value("session-data", v);
        VariantContainerBase c = VariantBase::cast_dynamic<VariantContainerBase>(v);
        sd.current_id = VariantBase::cast_dynamic<Variant<guint32>>(c.get_child(0)).get();

        {
            VariantContainerBase v_entries = VariantBase::cast_dynamic<VariantContainerBase>(c.get_child(1));

            for (unsigned int i = 0; i < v_entries.get_n_children(); ++i)
            {
                VariantContainerBase tup = VariantBase::cast_dynamic<VariantContainerBase>(v_entries.get_child(i));

                sd.entries.emplace_back(
                    std::make_pair(
                        VariantBase::cast_dynamic<Variant<guint32>>(tup.get_child(0)).get(),
                        VariantBase::cast_dynamic<Variant<std::string>>(tup.get_child(1)).get()
                    )
                );
            }
        }
        {
            std::cout << c.get_child(2).get_type_string() << " . " << c.get_child(2).print() << ' ' << (c.get_child(2).is_container() ? "true" : "false") << std::endl;
            std::cout.flush();
            Variant<guint32> v_active_id = VariantBase::cast_dynamic<Variant<guint32>>(VariantBase::cast_dynamic<VariantContainerBase>(c.get_child(2)).get_child());
            if (v_active_id)
            {
                sd.active_valid = true;
                sd.active_id = v_active_id.get();
            }
            else
            {
                sd.active_valid = false;
            }
        }



        m.restore(sd);
    }
}

unsigned int gsettings_preferences_model::get_history_size()
{
    return _settings_ref->get_uint(history_size_name);
}

bool gsettings_preferences_model::get_session_restore()
{
    return _settings_ref->get_boolean(session_restore_name);
}

void gsettings_preferences_model::init_view(preferences::view & v)
{
    v.on_history_size_change(get_history_size());
    v.on_session_restore_change(get_session_restore());
}
