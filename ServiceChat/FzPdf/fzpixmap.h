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
    FzPixmap(FzContext *ctx, FzDocument *doc, int number, const FzMatrix *mat, const FzColorspace &cs, int alpha = 0);
    fz_pixmap *getPixmap() const;
    void setPixmap(fz_pixmap *value);

    FzDocument *getDoc() const;
    void setDoc(FzDocument *value);

    FzContext *getCtx() const;
    void setCtx(FzContext *value);

    int getHeight();

    int getWidth();

private:
    FzContext *ctx;
    FzDocument *doc;
    fz_pixmap *pixmap;
    int height = -1;
    int width = -1;
};

#endif // FZPIXMAP_H
