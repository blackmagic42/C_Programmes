#ifndef BOARD_WIDGET_H_INCLUDED
#define BOARD_WIDGET_H_INCLUDED
#include "widget.h"

typedef struct{
    widget* (*init)(int width,int height,color background);
    widget* (*init_surface)(int width,int height,color background);
    void (*set_brush_color)(widget* w,color fg,color bg);
    void (*wipe)(widget* w);
    void (*draw_rect)(widget* w,rect r,color c);
    void (*draw_circle)(widget* w,int ox,int oy,int radius);
    void (*draw_round)(widget* w,int ox,int oy,int radius);
    void (*insert_text)(widget* w,char* text);
}__BoardWidgetClass;

extern __BoardWidgetClass BoardWidget;

#endif