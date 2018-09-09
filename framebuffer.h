#ifndef __FRAMEBUFFER_H
#define __FRAMEBUFFER_H

#include "types.h"

void PutPixel(UINT32 x, UINT32 y, UINT32 color);
void ClearScr(UINT32 color);
void DrawLine(int x1,int y1,int x2,int y2,int color);

#endif      // __FRAMEBUFFER_H
