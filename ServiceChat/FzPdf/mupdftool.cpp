#include "mupdftool.h"
#include "fz_header.h"
using namespace std;
MuPdfTool::MuPdfTool()
{

}

MuPdfUtil::Document::~Document()
{
    delete document;
    delete context;
}

MuPdfUtil::Document::Document()
{
    context = new FzContext;
    context->RegisterDocumentHandlers();
}

MuPdfUtil::Document::Document(const QString &fileName) : Document()
{
    Open(fileName);
}

void MuPdfUtil::Document::Open(const QString &fileName)
{
    document = new FzDocument(context, fileName);
}

int MuPdfUtil::Document::GetPageCount()
{
    if(pageCount >= 0)
        return pageCount;
    pageCount = document->CountPages();
    return pageCount;
}

FzDocument *MuPdfUtil::Document::getDocument() const
{
    return document;
}

void MuPdfUtil::Document::setDocument(FzDocument *value)
{
    document = value;
}

FzContext *MuPdfUtil::Document::getContext() const
{
    return context;
}

void MuPdfUtil::Document::setContext(FzContext *value)
{
    context = value;
}

MuPdfUtil::RDocument::~RDocument()
{
    fz_drop_document(context, document);
    fz_drop_context(context);
}

MuPdfUtil::RDocument::RDocument()
{
    context = fz_new_context(nullptr, nullptr, FZ_STORE_UNLIMITED);
    if(!context)
    {
        qDebug() << QString("%1. cannot create mupdf context").arg(__func__);
    }
    fz_register_document_handlers(context);


}

MuPdfUtil::RDocument::RDocument(const QString &fileName) : RDocument()
{
    Open(fileName);
}

void MuPdfUtil::RDocument::Open(const QString &fileName)
{
    string name = fileName.toStdString();
    fz_try(context)
    {
        document = fz_open_document(context, name.data());
    }
    FZ_CATCH_DROPDOC(context, document);
}

int MuPdfUtil::RDocument::GetPageCount() const
{
    if(pageCount >= 0)
        return pageCount;
    fz_try(context)
    {
        pageCount = fz_count_pages(context, document);
    }
    FZ_CATCH_DROPDOC_NORETURN(context, document);
    return pageCount;
}

fz_pixmap *MuPdfUtil::RDocument::LoadPixmap(int i, fz_matrix *mat) const
{
    fz_pixmap *pixmap;
    fz_try(context)
        pixmap = fz_new_pixmap_from_page_number(context, document, i, mat, fz_device_rgb(context), 0);
    FZ_CATCH_DROPDOC_NORETURN(context, document);
    return pixmap;
}

fz_document *MuPdfUtil::RDocument::getDocument() const
{
    return document;
}

void MuPdfUtil::RDocument::setDocument(fz_document *value)
{
    document = value;
}

fz_context *MuPdfUtil::RDocument::getContext() const
{
    return context;
}

void MuPdfUtil::RDocument::setContext(fz_context *value)
{
    context = value;
}
