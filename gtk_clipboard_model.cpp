#include "gtk_clipboard_model.hpp"

gtk_clipboard_model::gtk_clipboard_model()
    : _primary(Gtk::Clipboard::get(GDK_SELECTION_PRIMARY))
    , _clipboard(Gtk::Clipboard::get(GDK_SELECTION_CLIPBOARD))
{
    _primary_con = _primary->signal_owner_change().connect(
        [&](GdkEventOwnerChange * e)
        {
            // fetch clipboard text
            auto text = _primary->wait_for_text();

            // sync with other clipboard
            _clipboard_con.block();
            _clipboard->set_text(text);
            _clipboard_con.block(false);

            // add to internal buffer
            _text_entries.push_back(text);

            // notify views about new text
            emit_add_clipboard_entry(text);
        }
    );

    //_clipboard_con = _clipboard->signal_owner_change().connect(
    //    [&](GdkEventOwnerChange * e)
    //    {
    //        // fetch clipboard text
    //        auto text = _clipboard->wait_for_text();

    //        // sync with other clipboard
    //        _primary_con.block();
    //        _primary->set_text(text);
    //        _primary_con.block(false);

    //        // add to internal buffer
    //        _text_entries.push_back(text);

    //        // notify views about new text
    //        emit_add_clipboard_entry(text);
    //    }
    //);
}


void gtk_clipboard_model::clear_clipboard_entries()
{
}

void gtk_clipboard_model::select_active_entry(int n)
{
}
