#include "fzmatrix.h"

FzMatrix::FzMatrix() : ctm(new fz_matrix)
{

}

FzMatrix::FzMatrix(const FzMatrix &mat)
{
    ctm = new fz_matrix(*mat.getCtm());
}

FzMatrix &FzMatrix::operator =(const FzMatrix &mat)
{
    if(this == &mat)
        return *this;
    delete ctm;
    ctm = new fz_matrix(*mat.getCtm());
    return *this;
}

FzMatrix::~FzMatrix()
{
    delete ctm;
}

void FzMatrix::Scale(float sx, float sy)
{
    fz_scale(ctm, sx, sy);
}

void FzMatrix::PreRotate(float degrees)
{
    fz_pre_rotate(ctm, degrees);
}

fz_matrix *FzMatrix::getCtm() const
{
    return ctm;
}

void FzMatrix::setCtm(fz_matrix *value)
{
    ctm = value;
}
