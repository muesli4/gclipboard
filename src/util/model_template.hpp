#ifndef GCLIPBOARD_UTIL_MODEL_HPP
#define GCLIPBOARD_UTIL_MODEL_HPP

#include <algorithm>

namespace util
{
    template <typename View>
    struct model_template
    {
        void add_view(View & v)
        {
            _view_ptrs.push_back(&v);
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
        void emit_view_method(void (View::*method_ptr)(FunArgs...), GivenArgs... args)
        {
            for (View * v : _view_ptrs)
            {
                (v->*method_ptr)(args...);
            }
        }

        private:

        std::vector<View *> _view_ptrs;
    };
}

#endif

