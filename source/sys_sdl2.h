//
// Created by Vladimir Atamanenko on 27.01.2022.
//

#ifndef SDLSAND_SYS_SDL2_H
#define SDLSAND_SYS_SDL2_H

class SDLInitializer
{
public:
    static SDLInitializer& initSDL() { return SDLI; }
protected:
    static SDLInitializer SDLI;
private:
    SDLInitializer();
};

#endif //SDLSAND_SYS_SDL2_H
