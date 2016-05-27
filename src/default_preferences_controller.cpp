#include "default_preferences_controller.hpp"

default_preferences_controller::default_preferences_controller(preferences::model & m)
    : _m(m)
{
}

void default_preferences_controller::preferences_set_history_size(unsigned int new_size)
{
    _m.set_history_size(new_size);
}
