#ifndef BMP_WIDGET_H_INCLUDED
#define BMP_WIDGET_H_INCLUDED
#include "widget.h"

typedef struct{
    widget* (*init)(char* filename);
}__BMPWidgetClass;

extern __BMPWidgetClass BMPWidget;

#endif