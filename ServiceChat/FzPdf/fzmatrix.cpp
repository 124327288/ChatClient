#include "fzmatrix.h"

FzMatrix::FzMatrix()
{

}

FzMatrix::FzMatrix(const FzMatrix &mat)
{
    ctm = mat.getCtm();
}

FzMatrix &FzMatrix::operator =(const FzMatrix &mat)
{
    if(this == &mat)
        return *this;
    ctm = mat.getCtm();
    return *this;
}

FzMatrix::~FzMatrix()
{

}

void FzMatrix::Scale(float sx, float sy)
{
    fz_scale(&ctm, sx, sy);
}

void FzMatrix::PreRotate(float degrees)
{
    fz_pre_rotate(&ctm, degrees);
}

fz_matrix &FzMatrix::getCtm()
{
    return ctm;
}

const fz_matrix &FzMatrix::getCtm() const
{
    return ctm;
}

void FzMatrix::setCtm(const fz_matrix &value)
{
    ctm = value;
}
