#include "gtk_clipboard_model.hpp"

#include <utility>

gtk_clipboard_model::gtk_clipboard_model(unsigned int buffer_size)
    : _text_buffer(buffer_size)
    , _active_valid(false)
    , _primary(Gtk::Clipboard::get(GDK_SELECTION_PRIMARY))
    , _ignore_primary(false)
    , _clipboard(Gtk::Clipboard::get(GDK_SELECTION_CLIPBOARD))
    , _ignore_clipboard(false)
{
    _primary_con = _primary->signal_owner_change().connect(
        [&](GdkEventOwnerChange * e)
        {
            handle_owner_change(e, _ignore_primary, _primary, _ignore_clipboard, _clipboard);
        }
    );

    _clipboard_con = _clipboard->signal_owner_change().connect(
        [&](GdkEventOwnerChange * e)
        {
            handle_owner_change(e, _ignore_clipboard, _clipboard, _ignore_primary, _primary);
        }
    );
}

void gtk_clipboard_model::handle_owner_change(GdkEventOwnerChange * e, bool & ignore_source, Glib::RefPtr<Gtk::Clipboard> source, bool & ignore_other, Glib::RefPtr<Gtk::Clipboard> other)
{
    if (ignore_source)
    {
        ignore_source = false;
    }
    else
    {
        auto text = source->wait_for_text();

        // sync with other clipboard
        ignore_other = true;
        other->set_text(text);

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

void gtk_clipboard_model::update_active_id(unsigned int id)
{
    // TODO what if _active_id == id?
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
    _primary->set_text("");
    _ignore_clipboard = true;
    _clipboard->set_text("");
    _text_buffer.clear();
    _active_valid = false;
    emit_clear();
}

void gtk_clipboard_model::select_active(unsigned int id)
{
    auto const end_it = _text_buffer.end();
    auto it = find_if(_text_buffer.begin(), end_it, [id](std::pair<std::string, unsigned int> const & p) { return p.second == id; });
    if (it != end_it)
    {
        _ignore_primary = true;
        _primary->set_text(it->first);
        _ignore_clipboard = true;
        _clipboard->set_text(it->first);
        update_active_id(id);
    }
}

gtk_clipboard_model::~gtk_clipboard_model()
{
}

