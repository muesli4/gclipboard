#include "gtk_clipboard_model.hpp"

#include <utility>
#include <iterator>

#include <iostream>

gtk_clipboard_model::gtk_clipboard_model(unsigned int buffer_size)
    : _text_buffer(buffer_size)
    , _active_valid(false)
    , _primary_ref(Gtk::Clipboard::get(GDK_SELECTION_PRIMARY))
    , _clipboard_ref(Gtk::Clipboard::get(GDK_SELECTION_CLIPBOARD))
    , _last_time() // TODO sensible initial value
    , _frozen(false)
{
    setup_primary_default_owner_change_handler();
    setup_clipboard_default_owner_change_handler();
}

void gtk_clipboard_model::clear()
{
    update_primary_silently("");
    update_clipboard_silently("");
    _text_buffer.clear();
    _active_valid = false;
    emit_clear();
    std::cout << "post clear " << _text_buffer.size() << std::endl;
}

void gtk_clipboard_model::select_active(unsigned int id)
{
    auto it = find_id(id);
    if (it != _text_buffer.end())
    {
        auto text = it->first;
        update_primary_silently(text);
        update_clipboard_silently(text);
        update_active_id(id);

        // additionally this model will move active entries to the front
        std::rotate( _text_buffer.begin()
                   , it
                   , it + 1
                   );

        emit_move_front(id);
    }
}

void gtk_clipboard_model::remove(unsigned int id)
{
    auto it = find_id(id);
    if (it != _text_buffer.end())
    {
        _text_buffer.erase(it);

        // did we delete the active entry?
        if (_active_valid && _active_id == id && !_text_buffer.empty())
        {
            // then first unselect it
            emit_unselect_active(_active_id);

            // make the first entry the active one
            auto const & p = _text_buffer.front();
            auto text = p.first;
            update_primary_silently(text);
            update_clipboard_silently(text);

            _active_id = p.second;
            emit_select_active(_active_id);
        }

        emit_remove(id);
    }
}

void gtk_clipboard_model::change(unsigned int id, std::string const & s)
{
    auto it = find_id(id);
    if (it != _text_buffer.end())
    {
        it->first = s;
        emit_change(id, s);
    }
}

void gtk_clipboard_model::freeze(freezable::request_type rt)
{
    // aquire the mutex to ensure that the last update will finish, if any
    std::lock_guard<std::mutex> lock(_owner_change_mutex);

    // always prefer user choices
    if (_frozen && rt != _frozen_request_type)
    {

        if (_frozen_request_type == freezable::request_type::SYSTEM)
        {
            emit_freeze(freezable::request_type::USER);
        }
        _frozen_request_type = freezable::request_type::USER;
    }
    else
    {
        _frozen = true;
        _frozen_request_type = rt;
        emit_freeze(rt);
    }


}

void gtk_clipboard_model::thaw(freezable::request_type rt)
{
    if (rt == freezable::request_type::USER || rt == _frozen_request_type)
    {
        // thaw if same request type as freeze or if user wants to
        _frozen = false;
        emit_thaw();
    }
}

gtk_clipboard_model::~gtk_clipboard_model()
{
}

void gtk_clipboard_model::init_view(clipboard::view & v)
{
    for (auto p : _text_buffer)
        v.on_add(p.first, p.second);

    if (_active_valid)
        v.on_select_active(_active_id);
}

void gtk_clipboard_model::init_view(freezable::view & v)
{
    if (_frozen)
        v.on_freeze(_frozen_request_type);
}

void gtk_clipboard_model::restore_template(std::vector<std::pair<unsigned int, std::string>> const & entries, bool active_valid, unsigned int active_id)
{
    if (_active_valid)
    {
        emit_unselect_active(_active_id);
        _active_valid = false;
    }
    _text_buffer.clear();
    emit_clear();
    
    for (auto it = entries.rbegin(); it != entries.rend(); ++it)
    {
        std::pair<unsigned int, std::string> const & p = *it;
        unsigned int id = p.first;
        std::string const & s = p.second;
        _text_buffer.push_front(std::make_pair(s, id));
        emit_add(s, id);
    }

    if (active_valid && find_id(active_id) != _text_buffer.end())
    {
        _active_valid = true;
        _active_id = active_id;
    }
}

void gtk_clipboard_model::save_template(std::vector<std::pair<unsigned int, std::string>> & entries, bool & active_valid, unsigned int & active_id)
{
    std::cout << _text_buffer.size() << std::endl;
    for (auto const & p : _text_buffer)
    {
        entries.emplace_back(p.second, p.first);
    }
    active_valid = _active_valid;
    active_id = _active_id;
}

// precondition: we can assume it is at least one
void gtk_clipboard_model::on_history_size_change(unsigned int new_size)
{
    while (_text_buffer.size() > new_size)
    {
        _text_buffer.pop_back();
        emit_remove_oldest();
    }

    _text_buffer.set_capacity(new_size);

}

void gtk_clipboard_model::on_session_restore_change(bool restore)
{
}

void gtk_clipboard_model::setup_primary_default_owner_change_handler()
{
    _primary_con = _primary_ref->signal_owner_change().connect(
        [&](GdkEventOwnerChange * e)
        {
            handle_owner_change(e, _primary_ref, &gtk_clipboard_model::update_clipboard_silently);
        }
    );
}

void gtk_clipboard_model::setup_clipboard_default_owner_change_handler()
{
    _clipboard_con = _clipboard_ref->signal_owner_change().connect(
        [&](GdkEventOwnerChange * e)
        {
            handle_owner_change(e, _clipboard_ref, &gtk_clipboard_model::update_primary_silently);
        }
    );
}

void gtk_clipboard_model::update_primary_silently(std::string const & s)
{
    _primary_con.disconnect();
    _primary_con = _primary_ref->signal_owner_change().connect(
        [&](GdkEventOwnerChange * e)
        {
            // in rare cases a value might slip in between?
            _primary_con.disconnect();
            setup_primary_default_owner_change_handler();
        }
    );
    _primary_ref->set_text(s);
}

void gtk_clipboard_model::update_clipboard_silently(std::string const & s)
{
    _clipboard_con.disconnect();
    _clipboard_con = _clipboard_ref->signal_owner_change().connect(
        [&](GdkEventOwnerChange * e)
        {
            // in rare cases a value might slip in between?
            _clipboard_con.disconnect();
            setup_clipboard_default_owner_change_handler();
        }
    );
    _clipboard_ref->set_text(s);
}

void gtk_clipboard_model::handle_owner_change(GdkEventOwnerChange * e, Glib::RefPtr<Gtk::Clipboard> source_ref, void (gtk_clipboard_model::*update_other_silently)(std::string const &))
{

    // Some applications send more than one event with the same content.
    if (e->time - _last_time > 40)
    {
        // do we want the history to not change ?
        if (_frozen)
        {
            // make sure to update the active indicator, but don't sync with clipboard
            if (_active_valid)
            {
                emit_unselect_active(_active_id);
                _active_valid = false;
            }
        }
        else
        {
            // secure critical section
            std::unique_lock<std::mutex> lock(_owner_change_mutex, std::defer_lock);
            bool locked = lock.try_lock();

            if (locked)
            {
                _last_time = e->time;

                // note: this call will yield and may execute other code while
                // it is suspended
                auto text = source_ref->wait_for_text();

                // Since one can't reliably distinguish between self-emitted
                // events and outside events the string serves as indicator.
                // Additionally some applications send the empty string when quitting.
                if (!text.empty() && (_text_buffer.empty() || text != _text_buffer.front().first))
                {
                    // sync with other clipboard
                    (this->*update_other_silently)(text);

                    // add to internal buffer
                    if (_text_buffer.full())
                    {
                        emit_remove_oldest();
                    }
                    unsigned int id = fresh_id();
                    _text_buffer.push_front(std::make_pair(text, id));

                    // notify views about new text
                    emit_add(text, id);

                    // update which entries should be selected
                    update_active_id(id);
                }
            }
        }
    }
}

void gtk_clipboard_model::update_active_id(unsigned int id)
{
    if (_active_valid)
    {
        // entry already active
        if (_active_id == id)
            return;

        emit_unselect_active(_active_id);
    }
    _active_id = id;
    _active_valid = true;
    emit_select_active(id);
}

gtk_clipboard_model::iterator gtk_clipboard_model::find_id(unsigned int id)
{
    return find_if(_text_buffer.begin(), _text_buffer.end(), [id](std::pair<std::string, unsigned int> const & p) { return p.second == id; });
}

