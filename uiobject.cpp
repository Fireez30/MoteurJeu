#include "uiobject.h"

UiObject::UiObject(int x, int y, int z,QVector2D base, QVector2D alt):Base2D(x,y,z,base),posFromCam(x,y),AltTexture(alt),useMainText(true),MainTexture(base){
    renderer.CreateGeometry();
}
//au cas ou l'objet possède 2 textures, passer de l'une a l'autre
void UiObject::BindAltTexture(){
    useMainText = false;
    renderer.SetXSpriteCoord(AltTexture.x());
    renderer.SetYSpriteCoord(AltTexture.y());
    renderer.CreateGeometry();
}

//au cas ou l'objet possède 2 textures, passer de l'une a l'autre
void UiObject::BindMainTexture(){
    useMainText = true;
    renderer.SetXSpriteCoord(MainTexture.x());
    renderer.SetYSpriteCoord(MainTexture.y());
    renderer.CreateGeometry();
}

bool UiObject::MainTextBound(){
    return useMainText;
}
bool UiObject::AltTextBound(){
    return !useMainText;
}
