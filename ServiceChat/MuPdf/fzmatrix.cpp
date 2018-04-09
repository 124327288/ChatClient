#include "fzmatrix.h"

FzMatrix::FzMatrix()
{

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
