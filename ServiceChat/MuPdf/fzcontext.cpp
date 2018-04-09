#include "fzcontext.h"

FzContext::~FzContext()
{
    if(ctx)
        fz_drop_context(ctx);
}

FzContext::FzContext()
{
    ctx = fz_new_context(nullptr, nullptr, FZ_STORE_UNLIMITED);
    if(!ctx)
    {
        qDebug() << QString("%1. cannot create mupdf context").arg(__func__);
    }
}

void FzContext::RegisterDocumentHandlers()
{
    fz_try(ctx)
    {
        fz_register_document_handlers(ctx);
    }
    FZ_CATCH(ctx);
}

fz_context *FzContext::getCtx() const
{
    return ctx;
}

void FzContext::setCtx(fz_context *value)
{
    ctx = value;
}
