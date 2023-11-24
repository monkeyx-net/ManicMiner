#include "common.h"
#include "video.h"
#include "audio.h"
#include "game.h"

#define MINER       80 * WIDTH + 29 * 8

BYTE    titlePixels[2048] =
{
    160, 255, 255, 255, 255, 255, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0,
    224, 255, 255, 255, 255, 255, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 62, 7, 224, 124, 1, 0, 0, 0, 0, 0,
    192, 255, 255, 255, 255, 255, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 221, 250, 95, 187, 1, 0, 0, 0, 0, 0,
    128, 255, 255, 255, 255, 255, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 192, 235, 253, 191, 215, 3, 0, 0, 0, 0, 0,
    96, 255, 255, 255, 255, 255, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 192, 247, 255, 255, 239, 3, 0, 0, 0, 0, 0,
    208, 255, 255, 255, 255, 255, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 224, 255, 255, 255, 255, 7, 0, 0, 0, 0, 0,
    160, 255, 255, 255, 255, 255, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 224, 255, 255, 255, 255, 7, 0, 0, 0, 0, 0,
    84, 255, 255, 255, 255, 255, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 240, 255, 255, 255, 255, 15, 0, 0, 0, 0, 0,
    220, 255, 255, 255, 191, 255, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 240, 252, 0, 0, 15, 15, 0, 0, 0, 0, 0,
    104, 255, 243, 255, 143, 255, 19, 0, 0, 0, 0, 0, 128, 15, 0, 0, 0, 0, 0, 0, 0, 224, 255, 0, 0, 255, 7, 0, 0, 0, 0, 0,
    184, 255, 135, 255, 227, 255, 45, 0, 0, 0, 0, 0, 224, 31, 0, 0, 0, 0, 0, 0, 0, 0, 255, 126, 126, 251, 0, 0, 0, 0, 0, 0,
    248, 255, 71, 254, 228, 255, 111, 0, 0, 0, 0, 0, 240, 63, 0, 0, 0, 0, 0, 0, 0, 0, 224, 126, 126, 24, 0, 0, 0, 0, 0, 0,
    160, 255, 31, 125, 240, 243, 19, 0, 0, 0, 0, 0, 120, 220, 13, 0, 0, 0, 0, 0, 0, 0, 251, 126, 126, 223, 0, 0, 0, 0, 0, 0,
    192, 255, 63, 184, 252, 255, 11, 0, 0, 0, 0, 0, 184, 235, 27, 0, 0, 0, 0, 0, 0, 0, 251, 126, 126, 223, 0, 0, 0, 0, 0, 0,
    96, 255, 127, 0, 254, 255, 29, 0, 0, 0, 0, 0, 208, 247, 23, 0, 0, 0, 0, 0, 0, 0, 24, 126, 126, 7, 0, 0, 0, 0, 0, 0,
    192, 255, 255, 32, 255, 255, 38, 0, 0, 0, 0, 0, 224, 251, 55, 0, 0, 0, 0, 0, 0, 0, 255, 0, 0, 255, 0, 0, 0, 0, 0, 0,
    192, 159, 252, 128, 255, 255, 0, 0, 224, 7, 0, 0, 240, 251, 59, 0, 0, 0, 0, 0, 0, 0, 255, 0, 0, 255, 68, 68, 0, 12, 48, 0,
    160, 15, 243, 80, 255, 255, 0, 0, 254, 127, 0, 0, 240, 247, 30, 0, 0, 0, 24, 0, 80, 12, 128, 126, 126, 56, 238, 238, 0, 10, 48, 0,
    160, 247, 192, 212, 255, 11, 0, 0, 254, 127, 0, 0, 248, 255, 233, 1, 0, 0, 62, 28, 116, 62, 251, 126, 126, 251, 238, 238, 0, 10, 32, 192,
    232, 255, 11, 186, 255, 31, 0, 0, 254, 127, 0, 0, 248, 255, 247, 58, 14, 0, 126, 28, 117, 62, 251, 126, 126, 251, 255, 255, 0, 9, 48, 124,
    184, 255, 7, 170, 255, 3, 0, 0, 252, 63, 0, 0, 112, 254, 119, 123, 31, 0, 55, 63, 119, 119, 224, 126, 126, 1, 255, 255, 0, 5, 16, 8,
    248, 255, 31, 170, 255, 1, 128, 1, 252, 63, 128, 1, 240, 253, 119, 123, 31, 102, 127, 119, 102, 127, 255, 126, 126, 255, 238, 238, 128, 4, 24, 63,
    80, 255, 191, 170, 255, 0, 224, 1, 252, 63, 128, 7, 224, 251, 243, 246, 30, 119, 126, 126, 46, 63, 255, 0, 0, 255, 238, 238, 252, 255, 255, 255,
    224, 255, 127, 186, 255, 0, 248, 3, 252, 63, 192, 31, 192, 241, 225, 253, 15, 119, 54, 62, 36, 28, 48, 0, 0, 63, 238, 238, 255, 255, 199, 255,
    0, 255, 249, 214, 207, 0, 252, 3, 248, 31, 192, 63, 0, 0, 0, 0, 0, 34, 224, 12, 44, 104, 251, 254, 239, 255, 238, 238, 60, 243, 255, 60,
    0, 254, 240, 85, 47, 0, 254, 7, 248, 31, 224, 127, 0, 0, 0, 0, 0, 22, 24, 5, 40, 200, 251, 254, 239, 255, 238, 238, 126, 255, 255, 126,
    0, 124, 224, 85, 3, 0, 255, 7, 248, 31, 224, 255, 0, 0, 0, 0, 0, 72, 112, 7, 56, 88, 1, 4, 0, 0, 238, 238, 247, 255, 255, 247,
    0, 40, 64, 213, 3, 128, 255, 15, 248, 31, 240, 255, 1, 0, 0, 0, 0, 124, 240, 59, 24, 114, 223, 247, 125, 223, 238, 238, 223, 255, 255, 223,
    0, 0, 0, 182, 1, 128, 255, 15, 240, 15, 240, 255, 1, 0, 0, 0, 66, 22, 97, 113, 16, 54, 223, 247, 125, 223, 238, 238, 251, 243, 255, 251,
    0, 0, 0, 174, 0, 192, 255, 31, 240, 15, 248, 255, 3, 0, 0, 128, 84, 48, 5, 99, 16, 46, 192, 0, 0, 3, 238, 238, 239, 255, 255, 239,
    0, 0, 0, 170, 0, 192, 255, 31, 240, 15, 248, 255, 3, 0, 0, 0, 37, 32, 2, 3, 48, 32, 251, 191, 239, 251, 238, 238, 126, 254, 127, 126,
    0, 0, 0, 170, 0, 224, 255, 63, 240, 15, 252, 255, 7, 0, 0, 72, 255, 255, 7, 3, 255, 224, 251, 255, 239, 255, 238, 238, 60, 0, 0, 60,
    0, 0, 0, 174, 0, 224, 255, 63, 224, 7, 252, 255, 7, 0, 0, 146, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    0, 0, 0, 181, 0, 128, 255, 127, 224, 7, 254, 255, 1, 0, 0, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    0, 0, 0, 117, 0, 0, 254, 127, 0, 0, 254, 127, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    0, 0, 0, 86, 0, 0, 248, 255, 0, 0, 255, 31, 0, 0, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    0, 0, 0, 90, 0, 0, 224, 255, 192, 3, 255, 7, 0, 0, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    0, 0, 0, 106, 0, 0, 128, 127, 248, 31, 254, 1, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    0, 0, 0, 170, 0, 0, 0, 62, 254, 127, 124, 0, 0, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    0, 0, 0, 174, 0, 0, 0, 24, 255, 255, 24, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    0, 0, 0, 173, 0, 0, 0, 128, 255, 255, 1, 0, 0, 248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    0, 0, 0, 181, 0, 14, 0, 192, 255, 255, 3, 0, 112, 240, 255, 191, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 159, 255, 255, 255, 255, 255,
    0, 0, 96, 181, 12, 254, 0, 192, 255, 255, 3, 0, 127, 224, 255, 183, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 47, 255, 255, 219, 255, 255,
    0, 0, 240, 117, 31, 254, 15, 224, 255, 255, 7, 240, 127, 224, 255, 215, 255, 255, 255, 255, 255, 255, 255, 255, 247, 255, 7, 255, 255, 231, 255, 255,
    0, 0, 254, 110, 63, 254, 255, 224, 255, 255, 7, 255, 127, 192, 255, 239, 255, 255, 255, 255, 255, 255, 255, 255, 237, 255, 159, 255, 255, 239, 255, 255,
    0, 128, 255, 170, 63, 255, 255, 227, 255, 255, 199, 255, 255, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 235, 255, 207, 255, 255, 255, 255, 255,
    0, 240, 255, 162, 127, 255, 255, 243, 255, 255, 207, 255, 255, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 247, 255, 231, 255, 255, 255, 255, 255,
    0, 248, 255, 132, 255, 255, 255, 243, 255, 255, 207, 255, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 39, 232, 255, 255, 255, 255,
    0, 252, 127, 0, 254, 1, 0, 0, 224, 3, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 143, 226, 255, 255, 255, 255,
    128, 255, 31, 16, 254, 243, 0, 0, 128, 15, 0, 0, 0, 0, 255, 255, 183, 255, 255, 255, 255, 255, 255, 255, 255, 255, 95, 241, 255, 255, 255, 255,
    192, 255, 15, 227, 252, 255, 3, 0, 224, 7, 0, 0, 240, 3, 255, 255, 207, 255, 255, 252, 63, 255, 255, 255, 255, 255, 159, 224, 255, 255, 255, 255,
    240, 255, 207, 239, 255, 255, 135, 1, 192, 15, 0, 128, 255, 63, 255, 255, 239, 255, 63, 0, 0, 0, 248, 255, 255, 255, 31, 240, 255, 255, 255, 255,
    252, 255, 255, 255, 255, 255, 255, 7, 128, 1, 0, 252, 255, 255, 124, 255, 255, 255, 191, 0, 0, 187, 251, 255, 255, 255, 63, 248, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 15, 240, 3, 192, 255, 255, 255, 131, 255, 255, 255, 15, 248, 31, 0, 248, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 31, 192, 15, 248, 255, 255, 255, 255, 255, 255, 255, 239, 224, 7, 238, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 127, 128, 3, 254, 255, 255, 255, 255, 255, 255, 255, 3, 132, 33, 0, 128, 255, 255, 255, 255, 255, 255, 255, 63, 0,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 187, 92, 58, 187, 187, 255, 255, 255, 255, 255, 255, 255, 191, 187,
    224, 255, 224, 255, 224, 255, 15, 254, 255, 255, 255, 255, 255, 15, 14, 254, 255, 15, 0, 60, 61, 0, 0, 254, 15, 224, 255, 15, 254, 15, 14, 0,
    238, 255, 238, 255, 238, 255, 239, 254, 255, 255, 255, 255, 255, 239, 238, 254, 255, 239, 238, 132, 48, 238, 238, 254, 239, 238, 255, 239, 254, 239, 238, 238,
    0, 56, 0, 56, 224, 63, 8, 56, 56, 128, 3, 0, 248, 3, 0, 130, 63, 0, 0, 224, 3, 0, 0, 0, 0, 0, 128, 3, 248, 15, 0, 0,
    187, 187, 187, 187, 251, 191, 187, 187, 187, 187, 187, 187, 251, 187, 187, 187, 191, 187, 187, 248, 15, 187, 187, 187, 187, 187, 187, 187, 251, 191, 187, 187,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 0, 0, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 252, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

BYTE    titleColour[256] =
{
    0x5d, 0xcd, 0xcd, 0xcd, 0x4d, 0x4d, 0x5d, 0x50, 0x50, 0x50, 0x50, 0x50, 0x57, 0x57, 0x57, 0x57, 0x57, 0x50, 0x50, 0x50, 0x50, 0x5e, 0xfe, 0xfe, 0xfe, 0xfe, 0x5e, 0x50, 0x50, 0x50, 0x50, 0x50,
    0x5d, 0xcd, 0xfd, 0xfd, 0xfd, 0xcd, 0x5d, 0x50, 0x50, 0x57, 0x50, 0x50, 0x57, 0x57, 0x57, 0x57, 0x57, 0x50, 0x50, 0x50, 0x50, 0x5e, 0x87, 0xb9, 0xb9, 0x87, 0x5e, 0x50, 0x50, 0x52, 0x52, 0x50,
    0x5d, 0xfd, 0xfd, 0xfe, 0x4d, 0x5d, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x57, 0x57, 0x57, 0x57, 0x57, 0x53, 0x52, 0x57, 0x56, 0x51, 0x87, 0xb9, 0xb9, 0x87, 0x57, 0x57, 0x52, 0x52, 0x52, 0x52,
    0x50, 0x5d, 0x5d, 0xfe, 0x5d, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x50, 0x50, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x87, 0x87, 0x87, 0x87, 0x57, 0x57, 0x50, 0x52, 0x52, 0x50,
    0x50, 0x50, 0x50, 0xfe, 0x50, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x5c, 0x5c, 0x5c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
    0x50, 0x5c, 0x5c, 0xfe, 0x5c, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x7c, 0x0c, 0x4c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x4c, 0x0c, 0x7c, 0x7c, 0x0c, 0x4c, 0x0c, 0x0c,
    0x1c, 0x1c, 0xfc, 0xfc, 0xfc, 0x1c, 0x1c, 0x1c, 0x17, 0x17, 0x1c, 0x1c, 0x1c, 0x7c, 0x4c, 0x0c, 0x4c, 0x0c, 0x6c, 0xfc, 0xfc, 0x6c, 0x6c, 0x0c, 0x0c, 0x0c, 0x7c, 0x7c, 0x0c, 0x0c, 0x6c, 0x6c,
    0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0xfc, 0xfc, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c
};

char    textTicker[] = "\x1\x0\x2\x2" "M" "\x2\x6" "A" "\x2\x4" "N" "\x2\x5" "I" "\x2\x3" "C " "\x2\x5" "M" "\x2\x3" "I" "\x2\x2" "N" "\x2\x6" "E" "\x2\x4" "R" "\x2\x7" "   (C) Software Projects Ltd. 1983   By Matthew Smith                                " "\x2\x5" "Cursor Keys = Left & Right   " "\x2\x6" "Space = Jump   " "\x2\x3" "Pause/Tab = Pause   " "\x2\x4" "M = Tune On/Off                                " "\x2\x7" "Guide " "\x2\x5" "M" "\x2\x3" "i" "\x2\x2" "n" "\x2\x6" "e" "\x2\x4" "r" "\x2\x7" " Willy through 20 " "\x2\x2" "lethal " "\x2\x7" "caverns ...";

int     textPos, textEnd = ((int)sizeof(textTicker) - 50) * -8;

void Title_ScreenCopy()
{
    Video_CopyBytes(titlePixels);
    Video_CopyColour(titleColour, 0, 256);
}

void DoStartGame()
{
    Video_PixelFill(128 * WIDTH, 16 * WIDTH, 0x7);

    Video_Write(137 * WIDTH + 3, "\x1\x7\x2\x1" "A I R");
    Video_PixelFill(144 * WIDTH, 48 * WIDTH, 0x0);
    Video_WriteLarge(SCORE, 4, "\x1\x0\x2\x6" "High .....0        " "\x2\xe" "Score .....0");
    Game_GameReset();

    // we're calling an action from within an action
    // but that's ok ;-)
    Game_Action();
}

void DoTitleTicker()
{
    textPos -= 2;

    if (textPos < textEnd)
    {
        gameDemo = 1;
        Action = DoStartGame;
    }

    Miner_IncSeq();
}

void DoTitleDrawer()
{
    Video_WriteLarge(160 * WIDTH, textPos, textTicker);

    Video_Sprite(MINER, minerSprite[Miner_GetSeq()], 0xa, 0x7);
}

void DoTitleInit()
{
    Title_ScreenCopy();
    Video_PixelFill(64 * WIDTH, 64 * WIDTH, 0xa);
    Video_PixelFill(128 * WIDTH, 64 * WIDTH, 0x0);

    Video_Write(64 * WIDTH, "\x1\xa\x2\x6\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10");

    Video_WriteLarge(82 * WIDTH, 0, "\x2\x7\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf\x10\x11\x12\x13\x14\x15\x16");

    Video_Write(80 * WIDTH + 21 * 8, "\x2\x0" "Starring...");
    Video_Write(88 * WIDTH + 22 * 8, "\x2\x6" "Miner Willy");

    Video_WriteLarge(104 * WIDTH, 6 * 8, "\x2\x0" "PRESS " "\x2\x6" "ENTER" "\x2\x0" " TO START");

    Video_DrawPiano();

    textPos = WIDTH;

    Miner_SetSeq(4);
    Video_Sprite(MINER, minerSprite[Miner_GetSeq()], 0xa, 0x7);

    Audio_Music(MUS_TITLE, MUS_PLAY);

    Ticker = DoNothing;
}

void DoTitleResponder()
{
    if (gameInput == KEY_ENTER)
    {
        gameDemo = 0;
        Action = DoStartGame;
    }
    else if (gameInput == KEY_ESCAPE)
    {
        DoQuit();
    }
}

void DoTitleAction()
{
    if (audioMusicPlaying)
    {
        return;
    }

    Ticker = DoTitleTicker;
    Drawer = DoTitleDrawer;

    Action = DoNothing;
}

void Title_Action()
{
    Responder = DoTitleResponder;
    Ticker = DoTitleInit;
    Drawer = DoNothing;

    Action = DoTitleAction;
}
