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
