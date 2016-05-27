#include <algorithm>

#include "model.hpp"

namespace clipboard
{
    model::model()
        : _id_src(0)
    {
    }

    model::~model()
    {
    }

    DEFINE_MODEL_EMIT1(move_front, unsigned int, id)
    DEFINE_MODEL_EMIT2(add, std::string const &, s, unsigned int, id)
    DEFINE_MODEL_EMIT0(clear)
    DEFINE_MODEL_EMIT1(select_active, unsigned int, id)
    DEFINE_MODEL_EMIT1(unselect_active, unsigned int, id)
    DEFINE_MODEL_EMIT1(remove, unsigned int, id)
    DEFINE_MODEL_EMIT0(remove_oldest)
    DEFINE_MODEL_EMIT2(change, unsigned int, id, std::string const &, s)
    DEFINE_MODEL_EMIT1(freeze, request_type, rt)
    DEFINE_MODEL_EMIT0(thaw)

    unsigned int model::fresh_id()
    {
        auto id = _id_src;
        _id_src += 1;
        return id;
    }
}

