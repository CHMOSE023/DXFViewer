#include "libdxfrw.h"
#include <Vector>

static std::string ToUpper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

static Quaternion NormalFromExtPoint(Vector extPoint) {
    // DXF arbitrary axis algorithm for transforming a Z-vector into a rotated
    // coordinate system
    Vector ax, ay;
    Vector az = extPoint.WithMagnitude(1.0);

    if ((fabs(az.x) < 1 / 64.) && (fabs(az.y) < 1 / 64.)) {
        ax = Vector::From(0, 1, 0).Cross(az).WithMagnitude(1.0);
    }
    else {
        ax = Vector::From(0, 0, 1).Cross(az).WithMagnitude(1.0);
    }
    ay = az.Cross(ax).WithMagnitude(1.0);
    return Quaternion::From(ax, ay);
}

class DxfImport : public DRW_Interface
{
 

};

 