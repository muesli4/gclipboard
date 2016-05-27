#ifndef GCLIPBOARD_DEFAULT_PREFERENCES_CONTROLLER_HPP
#define GCLIPBOARD_DEFAULT_PREFERENCES_CONTROLLER_HPP

#include "preferences/controller.hpp"
#include "preferences/model.hpp"

struct default_preferences_controller : preferences::controller
{
    default_preferences_controller(preferences::model & m);

    void preferences_set_history_size(unsigned int new_size);

    private:

    preferences::model & _m;
};

#endif

