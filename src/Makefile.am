bin_PROGRAMS = gclipboard

AM_CPPFLAGS = -DLOCALEDIR='"$(localedir)"'

gclipboard_SOURCES = clipboard/controller.hpp clipboard/model.hpp clipboard/model.cpp clipboard/view.hpp default_clipboard_controller.hpp default_clipboard_controller.cpp gtk_clipboard_model.hpp gtk_clipboard_model.cpp gtk_history_window_view.hpp gtk_history_window_view.cpp gtk_history_menu_view.hpp gtk_history_menu_view.cpp util.hpp util.cpp main.cpp gtk_edit_entry_dialog.hpp gtk_edit_entry_dialog.cpp gtk_enabled_menu_item_view.hpp gtk_enabled_menu_item_view.cpp preferences/model.hpp preferences/model.cpp preferences/view.hpp util/model.hpp freezable/model.hpp freezable/model.cpp freezable/view.hpp freezable/request_type.hpp

gclipboard_CXXFLAGS = -std=c++11 -std=gnu++11 -Wall $(gtkmm_CFLAGS) $(gdkmm_CLFAGS) $(pangomm_CFLAGS) $(BOOST_CPPFLAGS)

gclipboard_LDADD = $(gtkmm_LIBS) $(gdkmm_LIBS) $(pangomm_LIBS) $(BOOST_LIBS)

