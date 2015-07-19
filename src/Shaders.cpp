#include "common/Logger.h"
#include "Shaders.h"
#include "Files.h"
#include "Locator.h"
#include <GL/glew.h>

namespace Arya
{
    //---------------------------------------------------------
    // SHADER
    //---------------------------------------------------------

    Shader::Shader(ShaderType type)
    {
        this->type = type;
        handle = 0;
        compiled = false;
        refCount = 0;
    }

    Shader::~Shader()
    {
        if(handle)
            glDeleteShader(handle);
    }

    bool Shader::addSourceFile(string f)
    {
        File* source = Locator::getFileSystem().getFile(f);
        if(!source) return false;
        sources.push_back(source);
        return true;
    }

    bool Shader::compile()
    {
        if(sources.empty()) 
        {
            LogWarning << "Can not compile shader, no sources set." << endLog;
            return false;
        }
        else 
        {
            switch(type)
            {
                case VERTEX:    handle = glCreateShader(GL_VERTEX_SHADER); break;
                case FRAGMENT:  handle = glCreateShader(GL_FRAGMENT_SHADER); break;
                case GEOMETRY:  handle = glCreateShader(GL_GEOMETRY_SHADER); break;
                default: LogError << "Error compiling shader: Invalid type." << endLog; return false;
            }

            const GLchar** gl_sources = new const GLchar*[sources.size()];
            GLint* lengths = new GLint[sources.size()];
            for(unsigned int i = 0; i < sources.size(); ++i) {
                gl_sources[i] = sources[i]->getData();
                lengths[i] = sources[i]->getSize();
            }

            glShaderSource(handle, sources.size(), gl_sources, lengths);

            delete[] gl_sources;
            delete[] lengths;

            glCompileShader(handle);

            GLint result;
            glGetShaderiv(handle, GL_COMPILE_STATUS, &result);
            if(result == GL_FALSE)
            {
                LogError << "Error compiling shader, log:\n";
                GLint logLength;
                glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
                if(logLength > 0)
                {
                    char* log = new char[logLength];
                    GLsizei written;
                    glGetShaderInfoLog(handle, logLength, &written, log);
                    LogError << log;
                    delete[] log;
                }
                else 
                {
                    LogError << "no log available";
                }
                LogError << endLog;

                glDeleteShader(handle);
                compiled = false;
                handle = 0;

                return false;
            }

            compiled = true;
        }

        return true;
    }

    //---------------------------------------------------------
    // SHADERPROGRAM
    //---------------------------------------------------------

    ShaderProgram::ShaderProgram()
    {
        handle = 0;
        linked = false;
        builtinUniforms = UNIFORM_NONE;
        init();
        valid = false;
    }

    ShaderProgram::ShaderProgram(string vertexFile, string fragmentFile)
    {
        handle = 0;
        linked = false;
        builtinUniforms = UNIFORM_NONE;
        init();

        valid = false;

        Shader* vertex = new Shader(Arya::VERTEX);
        Shader* fragment = new Shader(Arya::FRAGMENT);
        if( !vertex->addSourceFile(vertexFile)
                || !vertex->compile() 
                || !fragment->addSourceFile(fragmentFile)
                || !fragment->compile() ) {
            delete vertex;
            delete fragment;
            return;
        }
        attach(vertex);
        attach(fragment);
        if(!link()) return;

        valid = true;
    }

    ShaderProgram::~ShaderProgram()
    {
        for(unsigned int i = 0; i < shaders.size(); ++i)
        {
            shaders[i]->refCount--;
            if( shaders[i]->refCount == 0 )
            {
                delete shaders[i];
            }
        }
        if(handle)
            glDeleteProgram(handle);
    }

    bool ShaderProgram::init()
    {
        handle = glCreateProgram();
        if(handle == 0) { 
            LogError << "Error at glCreateProgram" << endLog;
            return false;
        }
        return true;
    }

    void ShaderProgram::attach(Shader* shader)
    {
        glAttachShader(handle, shader->getHandle());
        shader->refCount++;
        shaders.push_back(shader);
    }

    bool ShaderProgram::link()
    {
        glLinkProgram(handle);

        GLint result;
        glGetProgramiv(handle, GL_LINK_STATUS, &result);
        if(result == GL_FALSE)
        {
            LogError << "Error linking shader program. Log:\n";
            GLint logLength;
            glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);
            if(logLength > 0)
            {
                char* log = new char[logLength];
                GLsizei written;
                glGetProgramInfoLog(handle, logLength, &written, log);
                LogError << log;
                delete[] log;
            }
            LogError << endLog;
            return false;
        }

        valid = true;
        return true;
    }

    void ShaderProgram::use()
    {
        glUseProgram(handle);
    }

    //---------------------------
    // Uniforms
    //---------------------------

    GLint ShaderProgram::getUniformLocation(const char* name)
    {
        auto it = uniforms.find(name);
        if(it != uniforms.end()) return it->second;
        GLint loc = glGetUniformLocation(handle, name);
        if (loc == -1)
        {
            LogWarning << "ShaderProgram::getUniformLocation : Uniform " << name << " not found." << endLog;
            return -1;
        }
        uniforms.insert(std::pair<string,GLuint>(name, loc));
        return loc;
    }

    void ShaderProgram::setUniform1i(const char* name, int val)
    {
        glUniform1i(getUniformLocation(name), val);
    }

    void ShaderProgram::setUniform1f(const char* name, float val)
    {
        glUniform1f(getUniformLocation(name), val);
    }

    void ShaderProgram::setUniform2fv(const char* name, const vec2& values)
    {
        glUniform2fv(getUniformLocation(name), 1, &values[0]);
    }

    void ShaderProgram::setUniform3fv(const char* name, const vec3& values)
    {
        glUniform3fv(getUniformLocation(name), 1, &values[0]);
    }

    void ShaderProgram::setUniform4fv(const char* name, const vec4& values)
    {
        glUniform4fv(getUniformLocation(name), 1, &values[0]);
    }

    void ShaderProgram::setUniformMatrix4fv(const char* name, const mat4& matrix)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, false, &matrix[0][0]);
    }

    //---------------------------
    // Built-in uniforms
    //---------------------------

    void ShaderProgram::enableUniform(UNIFORM_FLAG flag)
    {
        builtinUniforms |= flag;
    }

    bool ShaderProgram::isEnabled(UNIFORM_FLAG flag)
    {
        return builtinUniforms & flag;
    }

    void ShaderProgram::setMoveMatrix(const mat4& m)
    {
        if (builtinUniforms & UNIFORM_MOVEMATRIX)
            setUniformMatrix4fv("mMatrix", m);
    }

    void ShaderProgram::setViewMatrix(const mat4& m)
    {
        if (builtinUniforms & UNIFORM_VIEWMATRIX)
            setUniformMatrix4fv("viewMatrix", m);
    }

    void ShaderProgram::setViewProjectionMatrix(const mat4& m)
    {
        if (builtinUniforms & UNIFORM_VPMATRIX)
            setUniformMatrix4fv("vpMatrix", m);
    }

    void ShaderProgram::setLightMatrix(const mat4& m)
    {
        if (builtinUniforms & UNIFORM_LIGHTMATRIX)
            setUniformMatrix4fv("lightMatrix", m);
    }

    void ShaderProgram::setTexture(int t)
    {
        if (builtinUniforms & UNIFORM_TEXTURE)
            setUniform1i("tex", t);
    }

    void ShaderProgram::setShadowTexture(int t)
    {
        if (builtinUniforms & UNIFORM_SHADOWTEXTURE)
            setUniform1i("shadowMap", t);
    }

    void ShaderProgram::setMaterialParams(vec4 par)
    {
        if (builtinUniforms & UNIFORM_MATERIALPARAMS)
            setUniform4fv("material", par);
    }

    void ShaderProgram::setAnimInterpolation(float t)
    {
        if (builtinUniforms & UNIFORM_ANIM_INTERPOL)
            setUniform1f("interpolation", t);
    }

    bool ShaderProgram::addUniform1i(const char* name, function<int(ShaderUniformBase*)> f)
    {
        GLint loc = glGetUniformLocation(handle, name);
        if (loc == -1)
        {
            LogError << "ShaderProgram::addUniform : " << name << " not found." << endLog;
            return false;
        }
        uniforms1i.push_back( ShaderUniform<int>{name, loc, f} );
        return true;
    }

    bool ShaderProgram::addUniform1f(const char* name, function<float(ShaderUniformBase*)> f)
    {
        GLint loc = glGetUniformLocation(handle, name);
        if (loc == -1)
        {
            LogError << "ShaderProgram::addUniform : " << name << " not found." << endLog;
            return false;
        }
        uniforms1f.push_back( ShaderUniform<float>{name, loc, f} );
        return true;
    }

    bool ShaderProgram::addUniform2fv(const char* name, function<vec2(ShaderUniformBase*)> f)
    {
        GLint loc = glGetUniformLocation(handle, name);
        if (loc == -1)
        {
            LogError << "ShaderProgram::addUniform : " << name << " not found." << endLog;
            return false;
        }
        uniforms2fv.push_back( ShaderUniform<vec2>{name, loc, f} );
        return true;
    }

    bool ShaderProgram::addUniform3fv(const char* name, function<vec3(ShaderUniformBase*)> f)
    {
        GLint loc = glGetUniformLocation(handle, name);
        if (loc == -1)
        {
            LogError << "ShaderProgram::addUniform : " << name << " not found." << endLog;
            return false;
        }
        uniforms3fv.push_back( ShaderUniform<vec3>{name, loc, f} );
        return true;
    }

    bool ShaderProgram::addUniform4fv(const char* name, function<vec4(ShaderUniformBase*)> f)
    {
        GLint loc = glGetUniformLocation(handle, name);
        if (loc == -1)
        {
            LogError << "ShaderProgram::addUniform : " << name << " not found." << endLog;
            return false;
        }
        uniforms4fv.push_back( ShaderUniform<vec4>{name, loc, f} );
        return true;
    }

    bool ShaderProgram::addUniformMatrix4fv(const char* name, function<mat4(ShaderUniformBase*)> f)
    {
        GLint loc = glGetUniformLocation(handle, name);
        if (loc == -1)
        {
            LogError << "ShaderProgram::addUniform : " << name << " not found." << endLog;
            return false;
        }
        uniformsMat4fv.push_back( ShaderUniform<mat4>{name, loc, f} );
        return true;
    }

    void ShaderProgram::doUniforms(ShaderUniformBase* e)
    {
        for (auto a : uniforms1i) glUniform1i(a.handle, a.func(e));
        for (auto a : uniforms1f) glUniform1f(a.handle, a.func(e));
        for (auto a : uniforms2fv) glUniform2fv(a.handle, 1, &(a.func(e))[0]);
        for (auto a : uniforms3fv) glUniform3fv(a.handle, 1, &(a.func(e))[0]);
        for (auto a : uniforms4fv) glUniform4fv(a.handle, 1, &(a.func(e))[0]);
        for (auto a : uniformsMat4fv) glUniformMatrix4fv(a.handle, 1, false, &(a.func(e))[0][0]);
    }

}
