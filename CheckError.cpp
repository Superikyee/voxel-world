#include "CheckError.h"

#include <iostream>


GLenum glCheckError_(const char *file, int line)
{
    bool Error_exists = false;
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        Error_exists = true;
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << "--------------------------------------------------------\nOPENGL ERROR | " << error << " | " << file << " (" << line << ")" << "\n--------------------------------------------------------\n";
    }

    if (!Error_exists) {
      std::cout << "--------------------------------------------------------\nNo errors " << " | " << file << " (" << line << ")" << "\n--------------------------------------------------------\n";
    }
    return errorCode;
}
