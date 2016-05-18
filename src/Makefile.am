bin_PROGRAMS = gclipboard

gclipboard_SOURCES = clipboard_controller.hpp clipboard_model.hpp clipboard_model.cpp clipboard_view.hpp default_clipboard_controller.hpp default_clipboard_controller.cpp gtk_clipboard_model.hpp gtk_clipboard_model.cpp gtk_history_window_view.hpp gtk_history_window_view.cpp gtk_left_menu_view.hpp gtk_left_menu_view.cpp util.hpp util.cpp main.cpp

gclipboard_CXXFLAGS = -std=c++11 -std=gnu++11 -Wall $(gtkmm_CFLAGS) $(gdkmm_CLFAGS) $(pangomm_CFLAGS) $(BOOST_CPPFLAGS)
#-I/usr/include/boost

gclipboard_LDADD = $(gtkmm_LIBS) $(gdkmm_LIBS) $(pangomm_LIBS)

