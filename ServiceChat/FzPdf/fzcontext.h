#ifndef FZCONTEXT_H
#define FZCONTEXT_H
#include "mupdftool.h"
class FzContext
{
public:
    ~FzContext();
    FzContext();
    FzContext(const FzContext &obj) = delete;
    FzContext &operator =(const FzContext &obj) = delete;
    void RegisterDocumentHandlers();
    fz_context *getCtx() const;
    void setCtx(fz_context *value);

private:
    fz_context *ctx;
};

#endif // FZCONTEXT_H
