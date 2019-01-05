#ifndef UIOBJECT_H
#define UIOBJECT_H

#include "base2d.h"

class UiObject : public Base2D
{
protected:
    QVector2D MainTexture;
    QVector2D AltTexture;
    bool useMainText;
    bool useAltText;
public:
    UiObject(const UiObject&) = delete;
    UiObject(int x, int y, int z,QVector2D base, QVector2D alt);
    void BindAltTexture();
    void BindMainTexture();
    bool MainTextBound();
    bool AltTextBound();
};

#endif // UIOBJECT_H
