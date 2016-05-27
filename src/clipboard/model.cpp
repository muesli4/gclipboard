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

    void model::restore(preferences::session_data const & sd)
    {
        _id_src = sd.current_id;
        restore_template(sd.entries, sd.active_valid, sd.active_id);
    }

    void model::save(preferences::session_data & sd)
    {
        sd.current_id = _id_src;
        save_template(sd.entries, sd.active_valid, sd.active_id);
    }

    DEFINE_MODEL_EMIT1(move_front, unsigned int, id)
    DEFINE_MODEL_EMIT2(add, std::string const &, s, unsigned int, id)
    DEFINE_MODEL_EMIT0(clear)
    DEFINE_MODEL_EMIT1(select_active, unsigned int, id)
    DEFINE_MODEL_EMIT1(unselect_active, unsigned int, id)
    DEFINE_MODEL_EMIT1(remove, unsigned int, id)
    DEFINE_MODEL_EMIT0(remove_oldest)
    DEFINE_MODEL_EMIT2(change, unsigned int, id, std::string const &, s)

    unsigned int model::fresh_id()
    {
        auto id = _id_src;
        _id_src += 1;
        return id;
    }
}

