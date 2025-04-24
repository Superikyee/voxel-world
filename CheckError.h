#ifndef CHECKERROR_H_INCLUDED
#define CHECKERROR_H_INCLUDED

#include <glad/glad.h>

//Nice copy paste
GLenum glCheckError_(const char *file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__)


#endif // CHECKERROR_H_INCLUDED
