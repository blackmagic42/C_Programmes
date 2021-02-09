#include "board-widget.h"
#include <stdlib.h>
#include <string.h>

widget* NewSurfaceWidget(int width,int height,color background);
widget* NewBoardWidget(int width,int height,color background);
void WidgetBoardSetBrushColor(widget* w,color fg,color bg);
void WidgetBoardWipe(widget* w);
void WidgetBoardWrite(widget* w,char* text);

__BoardWidgetClass BoardWidget={
    .init=NewBoardWidget,
    .init_surface=NewSurfaceWidget,
    .set_brush_color=WidgetBoardSetBrushColor,
    .wipe=WidgetBoardWipe,
    .insert_text=WidgetBoardWrite
};

typedef struct{
    pixel brush,*current;
}widget_board;


widget* NewSurfaceWidget(int width,int height,color background){
    widget* w=Widget.init(width,height);
    w->nb_pixels=width*height;
    w->pixel_array=malloc(sizeof(pixel)*w->nb_pixels);
    w->pixels=AllocNodeArray(width,height,NULL);
    w->select=Widget.standard_selection;
    
    pixel p={
        .fg=BLACK,
        .bg=background,
        .character=' ',
        .style=0
    };
    pixel* tb=w->pixel_array;

    for (int i=0;i<height;i++){
        for (int j=0;j<width;j++,tb++){
            *tb=p;
            w->pixels[i][j]->value=tb;
        }
    }
    return w;
}

widget* NewBoardWidget(int width,int height,color background){
    widget* w=NewSurfaceWidget(width,height,background);
    w->free_reserved=free;
    widget_board* wb=malloc(sizeof(widget_board));
    *wb=(widget_board){
        .brush=*(w->pixel_array),
        .current=w->pixel_array
    };
    w->reserved=wb;
    return w;
}

void WidgetBoardSetBrushColor(widget* w,color fg,color bg){
    widget_board* wb=w->reserved;
    if (wb){
        wb->brush.fg=fg;
        wb->brush.fg=bg;
    }
}

void WidgetBoardWipe(widget* w){
    widget_board* wb=w->reserved;
    if (wb==NULL) return;
    wb->brush.character=' ';
    wb->brush.style=0;
    wb->current=w->pixel_array;
    for (pixel *p=w->pixel_array,*end=p+w->nb_pixels;p<end;p++) *p=wb->brush;
}

void WidgetBoardWrite(widget* w,char* text){
    widget_board* wb=w->reserved;
    pixel* end=w->pixel_array+w->nb_pixels;
    for (;(*text);text++,wb->current++){
        wb->brush.character=*text;
        *(wb->current)=wb->brush;
        if (wb->current==end) break;
    }
}

bool IsSurfaceWidget(widget* w){
    return (w->nb_pixels==w->surface.width*w->surface.height);
}


