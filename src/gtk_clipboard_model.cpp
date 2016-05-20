#include "gtk_clipboard_model.hpp"

#include <utility>
#include <iterator>

gtk_clipboard_model::gtk_clipboard_model(unsigned int buffer_size)
    : _text_buffer(buffer_size)
    , _active_valid(false)
    , _primary_ref(Gtk::Clipboard::get(GDK_SELECTION_PRIMARY))
    , _clipboard_ref(Gtk::Clipboard::get(GDK_SELECTION_CLIPBOARD))
    , _last_time() // TODO sensible initial value
    , _frozen(false)
{
    _primary_con = _primary_ref->signal_owner_change().connect(
        [&](GdkEventOwnerChange * e)
        {
            handle_owner_change(e, _primary_ref, _clipboard_ref);
        }
    );

    _clipboard_con = _clipboard_ref->signal_owner_change().connect(
        [&](GdkEventOwnerChange * e)
        {
            handle_owner_change(e, _clipboard_ref, _primary_ref);
        }
    );
}

void gtk_clipboard_model::handle_owner_change(GdkEventOwnerChange * e, Glib::RefPtr<Gtk::Clipboard> source_ref, Glib::RefPtr<Gtk::Clipboard> other_ref)
{
    // This handler almost completely relies on timing, because it is nearly
    // impossible to track when Gtk runs a handler and disconnecting and
    // reconnecting is impossible. Also applications like firefox will send 3
    // events at once.
    if (e->time - _last_time > 40)
    {
        // do we want the history to not change ?
        if (_frozen)
        {
            // make sure to update the active indicator, but don't touch any entries
            if (_active_valid)
            {
                // hacky way, but the interface is terrible, so this is the best to come up with
                auto text = source_ref->wait_for_text();
                auto it = std::find_if(_text_buffer.begin(), _text_buffer.end(), [&](std::pair<std::string, unsigned int> p){ return p.first == text; });
                if (it != _text_buffer.end())
                {
                    auto id = it->second;
                    if (id != _active_id)
                    {
                        emit_unselect_active(_active_id);
                        emit_select_active(id);
                        _active_id = id;
                    }
                }
                else
                {
                    emit_unselect_active(_active_id);
                    _active_valid = false;
                }
            }
        }
        else
        {
            // secure critical section
            std::unique_lock<std::mutex> lock(_owner_change_mutex, std::defer_lock);
            bool locked = lock.try_lock();

            if (locked)
            {
                // note: this call will yield and may execute other code while
                // it is suspended
                auto text = source_ref->wait_for_text();

                // Since one can't reliably distinguish between self-emitted
                // events and outside events the string serves as indicator.
                if (_text_buffer.empty() || text != _text_buffer.front().first)
                {
                    // sync with other clipboard
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
            }
        }

        _last_time = e->time;
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

void gtk_clipboard_model::clear()
{
    _primary_ref->set_text("");
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
        _primary_ref->set_text(it->first);
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
    // aquire the mutex to ensure that the last update will finish, if any
    std::lock_guard<std::mutex> lock(_owner_change_mutex);
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

