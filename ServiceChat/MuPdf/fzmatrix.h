#ifndef FZMATRIX_H
#define FZMATRIX_H
#include "stdafx.h"
#include "mupdftool.h"

class FzMatrix
{
public:
    FzMatrix();
    ~FzMatrix();
    void Scale(float sx, float sy);
    void PreRotate(float degrees);
    fz_matrix *getCtm() const;
    void setCtm(fz_matrix *value);

private:
    fz_matrix *ctm;
};

#endif // FZMATRIX_H
