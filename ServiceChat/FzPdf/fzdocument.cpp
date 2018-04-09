#include "fzdocument.h"
#include "fz_header.h"
using namespace std;
FzDocument::~FzDocument()
{
    if(doc)
        fz_drop_document(ctx->getCtx(), doc);
}

FzDocument::FzDocument(FzContext *ctx, const QString &fileName)
{
    this->ctx = ctx;
    string filename = fileName.toStdString();
    fz_try(ctx->getCtx())
    {
        doc = fz_open_document(ctx->getCtx(), filename.data());
    }
    FZ_CATCH_DROPDOC(ctx->getCtx(), doc);
}

int FzDocument::CountPages()
{
    int cnt = 0;
    fz_try(ctx->getCtx())
    {
        cnt = fz_count_pages(ctx->getCtx(), doc);
    }
    FZ_CATCH_DROPDOC_NORETURN(ctx->getCtx(), doc);
    return cnt;
}

FzContext *FzDocument::getCtx() const
{
    return ctx;
}

void FzDocument::setCtx(FzContext *value)
{
    ctx = value;
}

fz_document *FzDocument::getDoc() const
{
    return doc;
}

void FzDocument::setDoc(fz_document *value)
{
    doc = value;
}
