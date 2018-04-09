#include "fzdocument.h"
#include "mupdftool.h"
using namespace std;
FzDocument::~FzDocument()
{
    fz_drop_document(ctx, doc);
}

FzDocument::FzDocument(FzContext *ctx, const QString &fileName)
{
    this->ctx = ctx;
    string filename = fileName.toStdString();
    fz_try(ctx)
    {
        doc = fz_open_document(ctx, filename.data());
    }
    FZ_CATCH_DROPDOC(ctx, doc);
}

int FzDocument::CountPages()
{
    int cnt = 0;
    fz_try(ctx)
    {
        cnt = fz_count_pages(ctx, doc);
    }
    FZ_CATCH_DROPDOC(ctx, doc);
    return cnt;
}

void FzDocument::Load(float scaleX, float scaleY, float rotateDegree, int page_from, int page_count, std::vector<fz_pixmap *> *pixmapList)
{
    if(!pixmapList)
    {
        DEBUG_VAR(pixmapList);
        return;;
    }
    fz_matrix ctm;
    fz_scale(&ctm, scaleX, scaleY);
    fz_pre_rotate(&ctm, rotateDegree);
    for(int i = 0; i < page_count; ++i)
    {
        fz_pixmap *pixmap = MuPdfTool::NewPixmapFromPageNumber(ctx, doc, page_from + i, &ctm, fz_device_rgb(ctx), 0);
        pixmapList->push_back(pixmap);
    }
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
