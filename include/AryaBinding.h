#pragma once

#include <memory>

namespace Arya
{
    // Base class needed because of templates
    // We do not want to pass the templates to the user
    class BindingBase
    {
        public:
            BindingBase(){};
            virtual ~BindingBase(){};
    };

    // User receives InputBinding when binding.
    // The binding is removed when the InputBinding goes out of scope
    typedef std::unique_ptr<BindingBase> InputBinding;
}
