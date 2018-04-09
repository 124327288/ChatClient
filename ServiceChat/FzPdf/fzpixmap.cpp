#include "fzpixmap.h"
#include "fz_header.h"

FzPixmap::~FzPixmap()
{
    if(pixmap)
        fz_drop_pixmap(ctx->getCtx(), pixmap);
}

FzPixmap::FzPixmap(FzContext *ctx, FzDocument *doc, int number, const FzMatrix *mat, const FzColorspace &cs, int alpha)
{
    this->ctx = ctx;
    this->doc = doc;
    fz_try(ctx->getCtx())
        pixmap = fz_new_pixmap_from_page_number(ctx->getCtx(), doc->getDoc(), number, mat->getCtm(), cs.getCs(), alpha);
    FZ_CATCH_DROPDOC(ctx->getCtx(), doc->getDoc());
}

fz_pixmap *FzPixmap::getPixmap() const
{
    return pixmap;
}

void FzPixmap::setPixmap(fz_pixmap *value)
{
    pixmap = value;
}

FzDocument *FzPixmap::getDoc() const
{
    return doc;
}

void FzPixmap::setDoc(FzDocument *value)
{
    doc = value;
}

FzContext *FzPixmap::getCtx() const
{
    return ctx;
}

void FzPixmap::setCtx(FzContext *value)
{
    ctx = value;
}

int FzPixmap::getHeight()
{
    if(height != -1)
        return height;
    return height = fz_pixmap_height(ctx->getCtx(), pixmap);
}

int FzPixmap::getWidth()
{
    if(width != -1)
        return width;
    return width = fz_pixmap_width(ctx->getCtx(), pixmap);
}
