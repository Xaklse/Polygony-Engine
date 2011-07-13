
#ifndef PE_RENDERER_H
#define PE_RENDERER_H


#include <windows.h> //Includes lots of other header files useful for Windows programming.


class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    bool Initialize();

    bool Render();
};


#endif //PE_RENDERER_H
