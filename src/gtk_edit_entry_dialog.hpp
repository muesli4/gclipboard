#ifndef GCLIPBOARD_GTK_EDIT_ENTRY_DIALOG_HPP
#define GCLIPBOARD_GTK_EDIT_ENTRY_DIALOG_HPP

#include <gtkmm/dialog.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/textview.h>
#include <gtkmm/hvbox.h>
#include <vector>
#include <string>


struct gtk_edit_entry_dialog : public Gtk::Dialog
{
    gtk_edit_entry_dialog(std::vector<std::pair<std::string, unsigned int>> const & es);

    std::vector<std::pair<std::string, unsigned int>> get_changes();

    private:
    
    std::vector<unsigned int> _ids;

    std::vector<Gtk::CheckButton> _buttons;
    std::vector<Gtk::TextView> _text_views;

    std::vector<Gtk::HBox> _row_boxes;
};


#endif

