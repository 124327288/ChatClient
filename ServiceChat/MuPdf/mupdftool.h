#ifndef MUPDFTOOL_H
#define MUPDFTOOL_H
#include "stdafx.h"
#include <mupdf/fitz.h>
#include <mupdf/pdf.h>

#define FZ_CATCH(ctx)                                                          \
    fz_catch(ctx)                                                                   \
    {                                                                               \
        qDebug() << QString("%2. Error : %1").arg(fz_caught_message(ctx)).arg(__func__); \
        fz_drop_context(ctx);                                                       \
        return;                                                                     \
    }

#define FZ_CATCH_DROPDOC(ctx, doc)                                                          \
    fz_catch(ctx)                                                                   \
    {                                                                               \
        qDebug() << QString("%2. Error : %1").arg(fz_caught_message(ctx)).arg(__func__); \
        fz_drop_document(ctx, doc);                                                 \
        fz_drop_context(ctx);                                                       \
        return;                                                                     \
    }
class MuPdfTool
{
public:
    MuPdfTool();

    static fz_context *NewContext();
    static void RegisterDocumentHandlers(fz_context *&context);
    static void OpenDocument(fz_context *ctx, fz_document *&doc, const QString &fileName);
    static int PagesCount(fz_context *ctx, fz_document *doc);
    static fz_pixmap *NewPixmapFromPageNumber(fz_context *ctx, fz_document *doc, int number, const fz_matrix *ctm, fz_colorspace *cs, int alpha);
private:

};
#endif // MUPDFTOOL_H
