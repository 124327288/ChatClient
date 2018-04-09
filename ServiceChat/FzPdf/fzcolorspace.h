#ifndef FZCOLORSPACE_H
#define FZCOLORSPACE_H
#include "stdafx.h"
#include "mupdftool.h"
class FzContext;
class FzColorspace
{
private:
    FzColorspace();
public:
    fz_colorspace *getCs() const;
    void setCs(fz_colorspace *value);
    static FzColorspace FzDeviceRgb(const FzContext *ctx);
private:
    fz_colorspace *cs;
};

#endif // FZCOLORSPACE_H
