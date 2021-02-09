#include "widget.h"
#include <unistd.h>

void PrintPointer(void* value){
    printf("%p\n",value);
}

void ForEachValueInside(node* root,void (*func)(void*)){
    node* n1=root->last_insertion;
    while (n1!=root){
        func(n1->value);
        n1=n1->previous_insertion;
    }
}

int main(){
    widget* root=BMPWidget.init("default.bmp");
    widget* container=ContainerWidget.init(20,10);
    widget* other=BMPWidget.init("right.bmp");
    for (int i=0;i<10;i++){
      widget* w=BoardWidget.init(20,1,WHITE);
        BoardWidget.insert_text(w,"test");
        Widget.insert(container,w,0,i);
    }
    Window.init(root);
    Window.insert_widget(root,container,0,0);
    Window.insert_widget(root,other,20,0);
    Widget.select(container);
    Window.destroy();
    return 0;
}