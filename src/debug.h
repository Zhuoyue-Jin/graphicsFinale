#pragma once
#include <GL/glew.h>
#include <iostream>


// namespace Debug
// {
// // Task 2: Add file name and line number parameters
// // Why? To tell exactly what file and what line the error is in
// inline void glErrorCheck(const char* fileName,int lineNumber)
// {
//     GLenum errorNumber = glGetError();
//     while (errorNumber != GL_NO_ERROR) {
//         // Task 2: Edit this print statement to be more descriptive
//         std::cout << errorNumber <<" ";
//         std::cout<<"The file : "<<fileName<<" ";
//         std::cout<<"The line : "<<lineNumber<<" ";

//         errorNumber = glGetError();
//     }
// }
// }

// // TASK 3: Add a preprocessor directive to automate the writing of this function
// #define glErrorCheck() glErrorCheck(__FILE__, __LINE__)
