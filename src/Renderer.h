
#ifndef PE_RENDERER_H
#define PE_RENDERER_H


class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    virtual bool Initialize();

    virtual bool Render();
};


#endif //PE_RENDERER_H
