#include "bmp-widget.h"
#include <stdlib.h>

widget* NewBMPWidget(char* filename);

__BMPWidgetClass BMPWidget={
    .init=NewBMPWidget
};

typedef struct BitMapHeader{ //bitmap header (size: 12 bytes)
    uint32_t FileSize; 
    uint16_t Reserved1;
    uint16_t Reserved2;
    uint32_t Offset;
}BitMapHeader;

typedef struct BitMapInfoHeader{//info header. It contains image properties (size: 40 bytes)
    uint32_t Size;
    uint32_t Width;
    uint32_t Height;
    uint16_t Planes;
    uint16_t BitsPerPixel;
    uint32_t Compression;
    uint32_t ImageSize;
    uint32_t HorizontalResolution;
    uint32_t VerticalResolution;
    uint32_t ColorUsed;
    uint32_t ImportantColors;
}BitMapInfoHeader;

widget* NewBMPWidget(char* filename){
    uint16_t magic,pitch,width;
    BitMapHeader header;
    BitMapInfoHeader info;
    widget* w=NULL;

    FILE* file=fopen(filename,"rb");
    if (file==NULL) return NULL;

    fread(&magic,2,1,file); //read the first 2 characters. It must be BMS
    if (magic!=0x4d42) goto end;

    fread(&header,sizeof(BitMapHeader),1,file); //read headers
    fread(&info,sizeof(BitMapInfoHeader),1,file);

    width=info.Width<<1;

    if (width>0xFF || info.Height>0xFF) goto end; //widget max dimension

    uint8_t BytePerPixel=info.BitsPerPixel/8; //support only 24 and 32 bits pixels 
    if (BytePerPixel<3) goto end;

    uint8_t current_pixel[4];
    pitch=info.ImageSize/info.Height;
    long offset=header.Offset+info.ImageSize-pitch;

    w=Widget.init(width,info.Height);
    w->pixels=AllocNodeArray(width,info.Height,NULL);
    w->nb_pixels=info.Height*info.Width;
    w->pixel_array=malloc(sizeof(pixel)*w->nb_pixels);
    w->select=Widget.standard_selection;

    pixel* p=w->pixel_array;

    for (int i=0;i<info.Height;i++,offset-=pitch){
        fseek(file,offset,SEEK_SET);
        for (int j=0;j<width;j+=2,p++){
            fread(current_pixel,BytePerPixel,1,file);
            *p=(pixel){
                .fg=WHITE,
                .bg=(color){current_pixel[2],current_pixel[1],current_pixel[0]},
                .character=' ',
                .style=0
            };
            w->pixels[i][j]->value=p;
            w->pixels[i][j+1]->value=p;
        }
    }

    end:
       fclose(file);
       return w;
}