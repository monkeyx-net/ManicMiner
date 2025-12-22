#include "common.h"
#include "system.h"
#include "video.h"
#include "audio.h"

#include <stdio.h>
#include <string.h>

int     gameRunning = 1, gameInput;

int     videoFlash = 0;

EVENT   Action = Loader_Action;
EVENT   Responder = DoNothing;
EVENT   Ticker = DoNothing;
EVENT   Drawer = DoNothing;
EVENT   Flasher = DoNothing;

void DoNothing()
{
}

void DoQuit()
{
    gameRunning = 0;
    Drawer = DoNothing;
    Ticker = DoNothing;
}

static void PrintHelp(void)
{
    printf("Manic Miner - Controls\n");
    printf("\n");
    printf("Keyboard:\n");
    printf("  Left/Right Arrows : Move\n");
    printf("  Space             : Jump\n");
    printf("  Enter             : Start\n");
    printf("  Esc               : Quit to title\n");
    printf("  Tab/Pause         : Pause\n");
    printf("  M                 : Mute music\n");
    printf("  F5                : Reload levels.txt\n");
    printf("\n");
    printf("Controller (SDL2):\n");
    printf("  D-Pad / Left Stick : Move\n");
    printf("  A or B             : Jump\n");
    printf("  Start              : Start\n");
    printf("  Select (Back)      : Pause\n");
    printf("  Start + Select     : Quit\n");
    printf("  Y                  : Mute music\n");
}

int main(int argc, char **argv)
{
    int time;
    int flash = 0;
    int i;

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 ||
            strcmp(argv[i], "--help") == 0 ||
            strcmp(argv[i], "help") == 0)
        {
            PrintHelp();
            return 0;
        }
    }

    Audio_Init();
    System_Init();

    time = System_GetTime();

    while (gameRunning)
    {
        if (time > System_GetTime())
        {
            continue;
        }
        time += 50;

        Action();

        while (System_GetEvent(&gameInput))
        {
            Responder();
        }

        System_LockVideo();
        Ticker();
        Drawer();
        System_UnlockVideo();
        System_UpdateVideo();

        flash++;
        if (flash == 20)
        {
            flash = 0;
            videoFlash = 1 - videoFlash;
        }
    }

    System_Quit();

    return 0;
}
