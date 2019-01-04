#include "uiobject.h"

UiObject::UiObject(int x, int y, int z,QVector2D base, QVector2D alt):Base2D(x,y,z,base),AltTexture(alt),useMainText(true),useAltText(true),MainTexture(base){

}

void UiObject::BindAltTexture(){
    useMainText = false;
    useAltText = true;
    renderer.SetXSpriteCoord(AltTexture.x());
    renderer.SetXSpriteCoord(AltTexture.y());
}
void UiObject::BindMainTexture(){
    useMainText = true;
    useAltText = false;
    renderer.SetXSpriteCoord(MainTexture.x());
    renderer.SetXSpriteCoord(MainTexture.y());
}

bool UiObject::MainTextBound(){
    return useMainText;
}
bool UiObject::AltTextBound(){
    return useAltText;
}
