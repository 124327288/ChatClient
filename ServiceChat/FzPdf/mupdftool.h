#ifndef MUPDFTOOL_H
#define MUPDFTOOL_H
#include "stdafx.h"
#include <mupdf/fitz.h>
#include <mupdf/pdf.h>

#define FZ_CATCH(ctx)                                                                       \
    fz_catch(ctx)                                                                           \
    {                                                                                       \
        qDebug() << QString("%2. Error : %1").arg(fz_caught_message(ctx)).arg(__func__);    \
        fz_drop_context(ctx);                                                               \
    }

#define FZ_CATCH_DROPDOC(ctx, doc)                                                          \
    fz_catch(ctx)                                                                           \
    {                                                                                       \
        qDebug() << QString("%2. Error : %1").arg(fz_caught_message(ctx)).arg(__func__);    \
        fz_drop_document(ctx, doc);                                                         \
        fz_drop_context(ctx);                                                               \
        return;                                                                             \
    }
#define FZ_CATCH_DROPDOC_NORETURN(ctx, doc)                                                 \
    fz_catch(ctx)                                                                           \
    {                                                                                       \
        qDebug() << QString("%2. Error : %1").arg(fz_caught_message(ctx)).arg(__func__);    \
        fz_drop_document(ctx, doc);                                                         \
        fz_drop_context(ctx);                                                               \
    }
class FzContext;
class FzDocument;
class FzMatrix;
namespace MuPdfUtil
{
    class Document
    {
    public:
        ~Document();
        Document();
        Document(const QString &fileName);
        void Open(const QString &fileName);
        int GetPageCount();
        FzContext *getContext() const;
        void setContext(FzContext *value);

        FzDocument *getDocument() const;
        void setDocument(FzDocument *value);

    private:
        FzContext *context = nullptr;
        FzDocument *document = nullptr;
        int pageCount = -1;
    };
}
class MuPdfTool
{
public:
    MuPdfTool();

};
#endif // MUPDFTOOL_H
