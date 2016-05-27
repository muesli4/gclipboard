#ifndef GCLIPBOARD_UTIL_MODEL_HPP
#define GCLIPBOARD_UTIL_MODEL_HPP

#include <algorithm>
#include <vector>

// some macros to define the corresponding emit functions
#define DEFINE_MODEL_EMIT0(name) \
    void model::emit_##name ()\
    {\
        emit_view_method(&view::on_##name );\
    }

#define DEFINE_MODEL_EMIT1(name, t1, n1) \
    void model::emit_##name ( t1 n1 )\
    {\
        emit_view_method(&view::on_##name , n1);\
    }

#define DEFINE_MODEL_EMIT2(name, t1, n1, t2, n2) \
    void model::emit_##name ( t1 n1 , t2 n2 )\
    {\
        emit_view_method(&view::on_##name , n1 , n2);\
    }

namespace util
{
    template <typename View>
    struct model_template
    {
        void add_view(View & v)
        {
            _view_ptrs.push_back(&v);
            init_view(v);
        }

        void remove_view(View & v)
        {
            auto it = std::find(_view_ptrs.begin(), _view_ptrs.end(), &v);
            if (it != _view_ptrs.end())
            {
                _view_ptrs.erase(it);
            }
        }

        protected:

        template <typename... FunArgs, typename... GivenArgs>
        inline void emit_view_method(void (View::*method_ptr)(FunArgs...), GivenArgs... args)
        {
            for (View * v : _view_ptrs)
            {
                (v->*method_ptr)(args...);
            }
        }

        virtual void init_view(View & v) = 0;

        private:

        std::vector<View *> _view_ptrs;
    };

    // workarround for stupid c++ function lookup definition
    template <typename View>
    void add_view(model_template<View> & m, View & v)
    {
        m.add_view(v);
    }

    template <typename View>
    void remove_view(model_template<View> & m, View & v)
    {
        m.remove_view(v);
    }
}

#endif

