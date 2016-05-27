#include "model.hpp"

namespace preferences
{
    DEFINE_MODEL_EMIT1(history_size_change, unsigned int, new_size)
    DEFINE_MODEL_EMIT1(session_restore_change, bool, restore)
}

