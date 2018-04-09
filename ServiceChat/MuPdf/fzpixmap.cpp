#include "fzcolorspace.h"
#include "fzcontext.h"
#include "fzdocument.h"
#include "fzmatrix.h"
#include "fzpixmap.h"

FzPixmap::~FzPixmap()
{
    if(pixmap)
        fz_drop_pixmap(ctx, pixmap);
}

FzPixmap::FzPixmap(const FzContext *ctx, const FzDocument *doc, int number, const FzMatrix *mat, const FzColorspace &cs, int alpha)
{
    this->ctx = ctx;
    this->doc = doc;
    fz_try(ctx)
        pixmap = fz_new_pixmap_from_page_number(ctx->getCtx(), doc->getDoc(), i, mat->getCtm(), cs.getCs(), alpha);
    FZ_CATCH_DROPDOC(ctx, doc);
}
