#include <SDL2/SDL.h>

#include "misc.h"

SDL_Window          *sdlWindow;
SDL_Renderer        *sdlRenderer;
SDL_Texture         *sdlTexture, *sdlTarget;
SDL_Rect            sdlViewport;
SDL_AudioDeviceID   sdlAudio;
SDL_GameController  *sdlController;
SDL_JoystickID      sdlControllerId;

const BYTE          *keyState;

UINT                *texPixels;
int                 texPitch;

static void System_OpenController(int index)
{
    SDL_Joystick *joy = NULL;

    if (!SDL_IsGameController(index))
    {
        return;
    }

    if (sdlController)
    {
        return;
    }

    sdlController = SDL_GameControllerOpen(index);
    if (!sdlController)
    {
        return;
    }

    joy = SDL_GameControllerGetJoystick(sdlController);
    sdlControllerId = SDL_JoystickInstanceID(joy);
}

static void System_CloseController(void)
{
    if (!sdlController)
    {
        return;
    }

    SDL_GameControllerClose(sdlController);
    sdlController = NULL;
    sdlControllerId = -1;
}

static int System_ControllerActive(void)
{
    return sdlController && SDL_GameControllerGetAttached(sdlController);
}

int System_GetTime()
{
    SDL_Delay(1);

    return SDL_GetTicks() * 3;
}

void System_LockAudio()
{
    SDL_LockAudioDevice(sdlAudio);
}

void System_UnlockAudio()
{
    SDL_UnlockAudioDevice(sdlAudio);
}

void SdlCallback(void *unused, Uint8 *buffer, int length)
{
    (void)unused;

    Audio_Callback((short *)buffer, length / 2);
}

void System_UpdateKeys()
{
    SDL_PumpEvents();
}

int System_IsKeyLeft()
{
    const int deadzone = 8000;

    if (keyState[SDL_SCANCODE_LEFT])
    {
        return 1;
    }

    if (!System_ControllerActive())
    {
        return 0;
    }

    if (SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
    {
        return 1;
    }

    return SDL_GameControllerGetAxis(sdlController, SDL_CONTROLLER_AXIS_LEFTX) <= -deadzone;
}

int System_IsKeyRight()
{
    const int deadzone = 8000;

    if (keyState[SDL_SCANCODE_RIGHT])
    {
        return 1;
    }

    if (!System_ControllerActive())
    {
        return 0;
    }

    if (SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
    {
        return 1;
    }

    return SDL_GameControllerGetAxis(sdlController, SDL_CONTROLLER_AXIS_LEFTX) >= deadzone;
}

int System_IsKeyJump()
{
    if (keyState[SDL_SCANCODE_SPACE])
    {
        return 1;
    }

    if (!System_ControllerActive())
    {
        return 0;
    }

    return SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_A) ||
        SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_B);
}

int System_GetEvent(int *key)
{
    SDL_Event   event;

    *key = KEY_NONE;

    if (SDL_PollEvent(&event) == 0)
    {
        return 0;
    }

    if (event.type == SDL_QUIT)
    {
        DoQuit();
        return 1;
    }

    if (event.type == SDL_WINDOWEVENT)
    {
        if (event.window.event == SDL_WINDOWEVENT_SHOWN)
        {
            SDL_ShowCursor(SDL_DISABLE);
        }
    }

    if (event.type == SDL_CONTROLLERDEVICEADDED)
    {
        System_OpenController(event.cdevice.which);
        return 0;
    }

    if (event.type == SDL_CONTROLLERDEVICEREMOVED)
    {
        if (event.cdevice.which == sdlControllerId)
        {
            System_CloseController();
        }
        return 0;
    }

    if (event.type == SDL_CONTROLLERBUTTONDOWN)
    {
        if (event.cbutton.button == SDL_CONTROLLER_BUTTON_START ||
            event.cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
        {
            if (SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_START) &&
                SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_BACK))
            {
                DoQuit();
                return 1;
            }
        }

        switch (event.cbutton.button)
        {
          case SDL_CONTROLLER_BUTTON_A:
          case SDL_CONTROLLER_BUTTON_B:
            *key = KEY_ENTER;
            break;
          case SDL_CONTROLLER_BUTTON_START:
            *key = KEY_ENTER;
            break;
          case SDL_CONTROLLER_BUTTON_BACK:
            *key = KEY_PAUSE;
            break;
          case SDL_CONTROLLER_BUTTON_Y:
            *key = KEY_MUTE;
            break;
          default:
            *key = KEY_ELSE;
        }

        return 1;
    }

    if (event.type != SDL_KEYDOWN)
    {
        return 0;
    }

    if (event.key.repeat)
    {
        return 0;
    }

    switch (event.key.keysym.sym)
    {
      case SDLK_RETURN:
        *key = KEY_ENTER;
        break;

      case SDLK_ESCAPE:
        *key = KEY_ESCAPE;
        break;

      case SDLK_PAUSE:
      case SDLK_TAB:
        *key = KEY_PAUSE;
        break;

      case SDLK_m:
        *key = KEY_MUTE;
        break;

      case SDLK_F5:
        *key = KEY_F5;
        break;

      case SDLK_0:
      case SDLK_1:
      case SDLK_2:
      case SDLK_3:
      case SDLK_4:
      case SDLK_5:
      case SDLK_6:
      case SDLK_7:
      case SDLK_8:
      case SDLK_9:
        *key = KEY_0 + (event.key.keysym.sym - SDLK_0);
        break;

      case SDLK_e:
      case SDLK_i:
      case SDLK_p:
      case SDLK_r:
      case SDLK_t:
      case SDLK_w:
      case SDLK_y:
        *key = KEY_A + (event.key.keysym.sym - SDLK_a);
        break;

      default:
        *key = KEY_ELSE;
    }

    return 1;
}

void System_SetPixel(int point, int index)
{
    *(texPixels + point) = videoPalette[index];
}

void System_Border(int index)
{
    SDL_SetRenderDrawColor(sdlRenderer, videoPalette[index] >> 16, (videoPalette[index] >> 8) & 0xff, videoPalette[index] & 0xff, 0xff);
}

void System_LockVideo()
{
    SDL_LockTexture(sdlTexture, NULL, (void **)&texPixels, &texPitch);
}

void System_UnlockVideo()
{
    SDL_UnlockTexture(sdlTexture);
}

void System_UpdateVideo()
{
    SDL_RenderClear(sdlRenderer);
    SDL_SetRenderTarget(sdlRenderer, sdlTarget);
    SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
    SDL_SetRenderTarget(sdlRenderer, NULL);
    SDL_RenderCopy(sdlRenderer, sdlTarget, NULL, &sdlViewport);
    SDL_RenderPresent(sdlRenderer);
}

void System_Quit()
{
    SDL_CloseAudioDevice(sdlAudio);

    System_CloseController();

    SDL_DestroyTexture(sdlTexture);
    SDL_DestroyTexture(sdlTarget);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);

    SDL_Quit();
}

void System_Init()
{
    SDL_AudioSpec   want;
    SDL_DisplayMode mode;
    int             multiply;
    int             index;

    sdlController = NULL;
    sdlControllerId = -1;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);

    SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");

    SDL_GetDesktopDisplayMode(0, &mode);

    multiply = Video_Viewport(mode.w, mode.h, &sdlViewport.x, &sdlViewport.y, &sdlViewport.w, &sdlViewport.h);

    sdlWindow = SDL_CreateWindow("Manic Miner", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    sdlTarget = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, WIDTH * multiply, HEIGHT * multiply);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    want.freq = SAMPLERATE;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = 256;
    want.callback = SdlCallback;

    sdlAudio = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
    SDL_PauseAudioDevice(sdlAudio, 0);

    keyState = SDL_GetKeyboardState(NULL);

    for (index = 0; index < SDL_NumJoysticks(); index++)
    {
        if (SDL_IsGameController(index))
        {
            System_OpenController(index);
            if (sdlController)
            {
                break;
            }
        }
    }
}
