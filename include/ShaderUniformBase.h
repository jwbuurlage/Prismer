#pragma once

namespace Arya
{
    // This is castable to both Entity and View (they subclassed it)
    // because a shader can be used to draw either an Entity or View
    // It is polymorphic to make sure that std typeinfo and dynamic_cast can be used
    class ShaderUniformBase {
        public:
            virtual ~ShaderUniformBase(){};
    };
}
