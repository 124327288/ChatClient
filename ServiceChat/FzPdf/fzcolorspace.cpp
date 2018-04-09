#include "fzcolorspace.h"
#include "fzcontext.h"

FzColorspace::FzColorspace()
{

}

fz_colorspace *FzColorspace::getCs() const
{
    return cs;
}

void FzColorspace::setCs(fz_colorspace *value)
{
    cs = value;
}

FzColorspace FzColorspace::FzDeviceRgb(const FzContext *ctx)
{
    FzColorspace cs;
    cs.setCs(fz_device_rgb(ctx->getCtx()));
    return cs;
}
