#ifndef FZDOCUMENT_H
#define FZDOCUMENT_H
#include "stdafx.h"
#include "mupdftool.h"
class FzContext;
class FzDocument
{
public:
    ~FzDocument();
    FzDocument() = delete;
    FzDocument(const FzDocument &obj) = delete;
    FzDocument &operator =(const FzDocument &obj) = delete;
    FzDocument(FzContext *ctx, const QString &fileName);
    int CountPages();
    FzContext *getCtx() const;
    void setCtx(FzContext *value);

    fz_document *getDoc() const;
    void setDoc(fz_document *value);

private:
    FzContext *ctx;
    fz_document *doc;
};

#endif // FZDOCUMENT_H
