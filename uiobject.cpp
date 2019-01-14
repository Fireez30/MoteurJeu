#include "uiobject.h"

UiObject::UiObject(int x, int y, int z,QVector2D base, QVector2D alt):Base2D(x,y,z,base),AltTexture(alt),useMainText(true),useAltText(true),MainTexture(base){

}

void UiObject::BindAltTexture(){
    useMainText = false;
    useAltText = true;
    renderer.SetXSpriteCoord(AltTexture.x());
    renderer.SetYSpriteCoord(AltTexture.y());
    renderer.CreateGeometry();
}
void UiObject::BindMainTexture(){
    useMainText = true;
    useAltText = false;
    renderer.SetXSpriteCoord(MainTexture.x());
    renderer.SetYSpriteCoord(MainTexture.y());
    renderer.CreateGeometry();
}

bool UiObject::MainTextBound(){
    return useMainText;
}
bool UiObject::AltTextBound(){
    return useAltText;
}
