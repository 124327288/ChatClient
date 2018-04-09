#ifndef FZPIXMAP_H
#define FZPIXMAP_H
#include "stdafx.h"
#include "mupdftool.h"
class FzContext;
class FzDocument;
class FzMatrix;
class FzColorspace;
class FzPixmap
{
public:
    ~FzPixmap();
    FzPixmap(const FzContext *ctx, const FzDocument *doc, int number, const FzMatrix &mat, const FzColorspace &cs, int alpha = 0);
private:
    FzContext *ctx;
    FzDocument *doc;
    fz_pixmap *pixmap;
};

#endif // FZPIXMAP_H
