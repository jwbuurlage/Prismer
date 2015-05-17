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
        init();
        valid = false;
    }

    ShaderProgram::ShaderProgram(string vertexFile, string fragmentFile)
    {
        handle = 0;
        linked = false;
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

    GLuint ShaderProgram::getUniformLocation(const char* name)
    {
        map<string,GLuint>::iterator it = uniforms.find(name);
        if(it != uniforms.end()) return it->second;
        GLuint loc = glGetUniformLocation(handle, name);
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
}
