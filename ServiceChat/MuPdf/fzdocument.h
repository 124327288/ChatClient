#ifndef FZDOCUMENT_H
#define FZDOCUMENT_H
#include "stdafx.h"
#include "mupdftool.h"
class FzContext;
class FzDocument
{
public:
    ~FzDocument();
    FzDocument(FzContext *ctx, const QString &fileName);
    int CountPages();
    void Load(float scaleX, float scaleY, float rotateDegree, int page_from, int page_count, std::vector<fz_pixmap*> *pixmapList);
    FzContext *getCtx() const;
    void setCtx(FzContext *value);

    fz_document *getDoc() const;
    void setDoc(fz_document *value);

private:
    FzContext *ctx;
    fz_document *doc;
};

#endif // FZDOCUMENT_H
