#include "mupdftool.h"
using namespace std;
MuPdfTool::MuPdfTool()
{

}

fz_context *MuPdfTool::NewContext()
{
    fz_context *context = fz_new_context(nullptr, nullptr, FZ_STORE_UNLIMITED);
    if(!context)
    {
        qDebug() << QString("%1. cannot create mupdf context").arg(__func__);
    }
    return context;
}

void MuPdfTool::RegisterDocumentHandlers(fz_context *&context)
{
    fz_try(context)
    {
        fz_register_document_handlers(context);
    }
    FZ_CATCH(context);
}

void MuPdfTool::OpenDocument(fz_context *ctx, fz_document *&doc, const QString &fileName)
{
    fz_try(ctx)
    {
        string filename = fileName.toStdString();
        doc = fz_open_document(ctx, filename.data());
    }
    FZ_CATCH_DROPDOC(ctx, doc);
}

int MuPdfTool::PagesCount(fz_context *ctx, fz_document *doc)
{
    int cnt = 0;
    fz_try(ctx)
    {
        cnt = fz_count_pages(ctx, doc);
    }
    FZ_CATCH_DROPDOC(ctx, doc);
    return cnt;
}

fz_pixmap *MuPdfTool::NewPixmapFromPageNumber(fz_context *ctx, fz_document *doc, int number, const fz_matrix *ctm, fz_colorspace *cs, int alpha)
{
    fz_pixmap *pixmap;
    fz_try(ctx)
    {
        pixmap = fz_new_pixmap_from_page_number(ctx, doc, number, ctm, cs, alpha);
    }
    FZ_CATCH_DROPDOC(ctx, doc);
    return pixmap;
}
