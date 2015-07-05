// Shader object information
//
// The engine can create a Shader and then attach it to a ShaderProgram
// From that moment on, ShaderProgram will have a pointer to it
// and as soon as the ShaderProgram is deleted it will decrease
// the refCount of its Shaders. When one of them reaches zero the
// Shader is deleted.
// The engine should allocate both Shader and ShaderProgram objects
// The engine should keep a pointer of the ShaderProgram and can forget the Shader pointers
// The engine should delete the ShaderProgram when no longer needed. This will automatically delete the Shaders
#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <cstdint>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "ShaderUniformBase.h"

//Prevents having to include full OpenGL header
typedef int	            GLint;
typedef unsigned int	GLuint;

using std::function;
using std::string;
using std::vector;
using std::map;

using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;

namespace Arya
{
    class File;

    enum ShaderType
    {
        VERTEX,
        FRAGMENT,
        GEOMETRY
    };

    class Shader
    {
        public:
            Shader(ShaderType type);
            ~Shader();

            // Adds a source file to the shader
            bool addSourceFile(string f);

            // Compiles the Shader and sets its handle
            bool compile();

            // Returns the handle
            GLuint getHandle() { return handle; };

        private:
            GLuint handle;
            bool compiled;
            int refCount;

            friend class ShaderProgram;

            vector<File*> sources;
            ShaderType type;
    };

    // Built-in uniforms, using flags
    // Other uniforms can be passed using the custom uniform method using callbacks
    // The built-in uniforms have fixed types and names shown after the enum
    enum UNIFORM_FLAG : std::int32_t
    {
        UNIFORM_NONE            = 0,
        UNIFORM_MOVEMATRIX      = 1,    //mat4 mMatrix
        UNIFORM_VIEWMATRIX      = 2,    //mat4 viewMatrix
        UNIFORM_VPMATRIX        = 4,    //mat4 vpMatrix
        UNIFORM_TEXTURE         = 8,    //sampler2D tex
        UNIFORM_MATERIALPARAMS  = 16,   //vec4 material
        UNIFORM_ANIM_INTERPOL   = 32,   //float interpolation
        UNIFORM_LIGHTMATRIX     = 64    //mat4 lightMatrix
    };
    //bit operators because it is not a primitive type
    using UnderType = std::underlying_type_t<UNIFORM_FLAG>;
    inline UNIFORM_FLAG operator| (UNIFORM_FLAG lhs, UNIFORM_FLAG rhs)
    {
        return (UNIFORM_FLAG)(static_cast<UnderType>(lhs) | static_cast<UnderType>(rhs));
    }

    // Custom uniforms using callbacks
    template <typename T>
        class ShaderUniform
        {
            public:
                const char* name;
                GLint handle;
                function< T (ShaderUniformBase*)> func;
        };

    class ShaderProgram
    {
        public:
            ShaderProgram();
			ShaderProgram(string vertexFile, string fragmentFile);
            ~ShaderProgram();

            void attach(Shader* shader);
            bool link();
            void use();

            GLuint getHandle() { return handle; };
			bool isValid() const { return valid; }

            // -- Manual way of setting uniforms
            GLint getUniformLocation(const char* name);
            void setUniform1i(const char* name, int val);
            void setUniform1f(const char* name, float val);
            void setUniform2fv(const char* name, const vec2& values);
            void setUniform3fv(const char* name, const vec3& values);
            void setUniform4fv(const char* name, const vec4& values);
            void setUniformMatrix4fv(const char* name, const mat4& matrix);

            // -- Built-in uniforms

            void enableUniform(UNIFORM_FLAG flag);
            bool isEnabled(UNIFORM_FLAG flag);

            //! These functions only set the uniform if the flag is enabled
            //! So the Graphics class will always call them
            void setMoveMatrix(const mat4& m);
            void setViewMatrix(const mat4& m);
            void setViewProjectionMatrix(const mat4& m);
            void setLightMatrix(const mat4& m);
            void setTexture(int t);
            void setMaterialParams(vec4 par);
            void setAnimInterpolation(float t);

            // -- Custom uniforms

            //! Add a custom uniform
            //! ShaderProgram should be valid before adding uniforms (isValid())
            //! Returns true if the uniform is present in the shader
            //! Note that contents of name is not copied and should remain valid
            bool addUniform1i(const char* name, function<int(ShaderUniformBase*)> f);
            bool addUniform1f(const char* name, function<float(ShaderUniformBase*)> f);
            bool addUniform2fv(const char* name, function<vec2(ShaderUniformBase*)> f);
            bool addUniform3fv(const char* name, function<vec3(ShaderUniformBase*)> f);
            bool addUniform4fv(const char* name, function<vec4(ShaderUniformBase*)> f);
            bool addUniformMatrix4fv(const char* name, function<mat4(ShaderUniformBase*)> f);

            //! Called by Graphics. Will perform all callbacks and set the uniforms
            void doUniforms(ShaderUniformBase* e);

        private:
            bool init();

            GLuint handle;
            bool linked;
			bool valid;

            vector<Shader*> shaders;

            std::int32_t builtinUniforms;
            vector<ShaderUniform<int> >     uniforms1i;
            vector<ShaderUniform<float> >   uniforms1f;
            vector<ShaderUniform<vec2> >    uniforms2fv;
            vector<ShaderUniform<vec3> >    uniforms3fv;
            vector<ShaderUniform<vec4> >    uniforms4fv;
            vector<ShaderUniform<mat4> >    uniformsMat4fv;

            // Caching for glGetUniformLocation
            map<string,GLint> uniforms;
    };
}
