#include "gtk_clipboard_model.hpp"

#include <utility>
#include <iterator>

#include <iostream>

gtk_clipboard_model::gtk_clipboard_model(unsigned int buffer_size)
    : _text_buffer(buffer_size)
    , _active_valid(false)
    , _primary_ref(Gtk::Clipboard::get(GDK_SELECTION_PRIMARY))
    , _ignore_primary(false)
    , _clipboard_ref(Gtk::Clipboard::get(GDK_SELECTION_CLIPBOARD))
    , _ignore_clipboard(false)
    , _last_time() // TODO sensible initial value
    , _frozen(false)
{
    _primary_con = _primary_ref->signal_owner_change().connect(
        [&](GdkEventOwnerChange * e)
        {
            handle_owner_change(e, _ignore_primary, _primary_ref, _ignore_clipboard, _clipboard_ref);
        }
    );

    _clipboard_con = _clipboard_ref->signal_owner_change().connect(
        [&](GdkEventOwnerChange * e)
        {
            handle_owner_change(e, _ignore_clipboard, _clipboard_ref, _ignore_primary, _primary_ref);
        }
    );
}

void gtk_clipboard_model::handle_owner_change(GdkEventOwnerChange * e, bool & ignore_source, Glib::RefPtr<Gtk::Clipboard> source_ref, bool & ignore_other, Glib::RefPtr<Gtk::Clipboard> other_ref)
{
    if (!_frozen)
    {
        std::unique_lock<std::mutex> lock(_owner_change_mutex, std::defer_lock);
        bool locked = lock.try_lock();
        bool remove_ignore = false;

        if (locked)
        {
            // ignore active or time between events too short
            if (e->time - _last_time < 40 || ignore_source)
            {
                remove_ignore = true;
            }
            else
            {
                auto text = source_ref->wait_for_text();

                // sync with other clipboard
                ignore_other = true;
                other_ref->set_text(text);

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
            _last_time = e->time;
        }
        // gtk somehow started both event handlers in parallel
        else
        {
            remove_ignore = true;
        }

        if (remove_ignore)
        {
            ignore_source = false;
        }
    }
}

void gtk_clipboard_model::update_active_id(unsigned int id)
{
    // TODO what if _active_id == id? it works, but update is not needed, since model does not change
    if (_active_valid)
    {
        emit_unselect_active(_active_id);
    }
    _active_id = id;
    _active_valid = true;
    emit_select_active(id);
}

void gtk_clipboard_model::clear()
{
    _ignore_primary = true;
    _primary_ref->set_text("");
    _ignore_clipboard = true;
    _clipboard_ref->set_text("");
    _text_buffer.clear();
    _active_valid = false;
    emit_clear();
}

void gtk_clipboard_model::select_active(unsigned int id)
{
    auto it = find_id(id);
    if (it != _text_buffer.end())
    {
        _ignore_primary = true;
        _primary_ref->set_text(it->first);
        _ignore_clipboard = true;
        _clipboard_ref->set_text(it->first);
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

        emit_remove(id);

        // did we delete the active entry?
        if (_active_valid && _active_id == id && !_text_buffer.empty())
        {
            // make the first entry the active one
            _active_id = _text_buffer.front().second;
            emit_select_active(_active_id);
        }
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

void gtk_clipboard_model::freeze()
{
    _frozen = true;
}

void gtk_clipboard_model::thaw()
{
    _frozen = false;
}

gtk_clipboard_model::iterator gtk_clipboard_model::find_id(unsigned int id)
{
    return find_if(_text_buffer.begin(), _text_buffer.end(), [id](std::pair<std::string, unsigned int> const & p) { return p.second == id; });
}

gtk_clipboard_model::~gtk_clipboard_model()
{
}

void gtk_clipboard_model::init_view(clipboard_view & v)
{
    for (auto p : _text_buffer)
        v.on_add(p.first, p.second);

    if (_active_valid)
        v.on_select_active(_active_id);
}

