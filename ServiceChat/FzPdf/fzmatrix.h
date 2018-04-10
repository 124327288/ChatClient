#ifndef FZMATRIX_H
#define FZMATRIX_H
#include "stdafx.h"
#include "mupdftool.h"

class FzMatrix
{
public:
    FzMatrix();
    FzMatrix(const FzMatrix &mat);
    FzMatrix &operator =(const FzMatrix &mat);
    ~FzMatrix();
    void Scale(float sx, float sy);
    void PreRotate(float degrees);
    fz_matrix &getCtm();
    const fz_matrix &getCtm() const;
    void setCtm(const fz_matrix &value);

private:
    fz_matrix ctm;
};

#endif // FZMATRIX_H
