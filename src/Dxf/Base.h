#pragma once
#include <unordered_map>

#ifndef M_PI
#define M_PI       3.141592653589793238462643
#endif
#ifndef M_PI_2
#define M_PI_2       1.57079632679489661923
#endif
#define M_PIx2      6.283185307179586 // 2*PI
#define ARAD 57.29577951308232

#define UTF8STRING std::string
#define DRW_UNUSED(x) (void)x

typedef signed char dint8;              /* 8 bit signed */
typedef signed short dint16;            /* 16 bit signed */
typedef signed int dint32;              /* 32 bit signed */
typedef long long int dint64;           /* 64 bit signed */

typedef unsigned char duint8;           /* 8 bit unsigned */
typedef unsigned short duint16;         /* 16 bit unsigned */
typedef unsigned int duint32;           /* 32 bit unsigned */
typedef unsigned long long int duint64; /* 64 bit unsigned */

typedef float dfloat32;                 /* 32 bit floating point */
typedef double ddouble64;               /* 64 bit floating point */
typedef long double ddouble80;          /* 80 bit floating point */



enum Version {
    UNKNOWNV,     //!< UNKNOWN VERSION.
    MC00,         //!< DWG Release 1.1
    AC12,         //!< DWG Release 1.2
    AC14,         //!< DWG Release 1.4
    AC150,        //!< DWG Release 2.0
    AC210,        //!< DWG Release 2.10
    AC1002,       //!< DWG Release 2.5
    AC1003,       //!< DWG Release 2.6
    AC1004,       //!< DWG Relase 9
    AC1006,       //!< DWG Release 10
    AC1009,       //!< DWG Release 11/12 (LT R1/R2)
    AC1012,       //!< DWG Release 13 (LT95)
    AC1014,       //!< DWG Release 14/14.01 (LT97/LT98)
    AC1015,       //!< AutoCAD 2000/2000i/2002
    AC1018,       //!< AutoCAD 2004/2005/2006
    AC1021,       //!< AutoCAD 2007/2008/2009
    AC1024,       //!< AutoCAD 2010/2011/2012
    AC1027,       //!< AutoCAD 2013/2014/2015/2016/2017
    AC1032,       //!< AutoCAD 2018/2019/2020
};

enum Error {
    BAD_NONE,             /*!< No error. */
    BAD_UNKNOWN,          /*!< UNKNOWN. */
    BAD_OPEN,             /*!< error opening file. */
    BAD_VERSION,          /*!< unsupported version. */
    BAD_READ_METADATA,    /*!< error reading metadata. */
    BAD_READ_FILE_HEADER, /*!< error in file header read process. */
    BAD_READ_HEADER,      /*!< error in header vars read process. */
    BAD_READ_HANDLES,     /*!< error in object map read process. */
    BAD_READ_CLASSES,     /*!< error in classes read process. */
    BAD_READ_TABLES,      /*!< error in tables read process. */
    BAD_READ_BLOCKS,      /*!< error in block read process. */
    BAD_READ_ENTITIES,    /*!< error in entities read process. */
    BAD_READ_OBJECTS,     /*!< error in objects read process. */
    BAD_READ_SECTION,     /*!< error in sections read process. */
    BAD_CODE_PARSED,      /*!< error in any parseCodes() method. */
};

class Coord {
public:
    Coord() = default;
    Coord(double ix, double iy, double iz) : x(ix), y(iy), z(iz) {}

    /*!< convert to unitary vector */
    void unitize() {
        double dist;
        dist = hypot(hypot(x, y), z);
        if (dist > 0.0) {
            x = x / dist;
            y = y / dist;
            z = z / dist;
        }
    }

public:
    double x{ 0 };
    double y{ 0 };
    double z{ 0 };
};



/// <summary>
/// 处理头部
/// </summary>
class Variant
{
    enum TYPE {
        STRING,
        INTEGER,
        DOUBLE,
        COORD,
        INVALID
    };
public:
    //TODO: add INT64 support
    Variant() : sdata(std::string()), vdata(), content(0), vType(INVALID), vCode(0) {}

    Variant(int c, dint32 i) : sdata(std::string()), vdata(), content(i), vType(INTEGER), vCode(c) {}

    Variant(int c, duint32 i) : sdata(std::string()), vdata(), content(static_cast<dint32>(i)), vType(INTEGER), vCode(c) {}

    Variant(int c, double d) : sdata(std::string()), vdata(), content(d), vType(DOUBLE), vCode(c) {}

    Variant(int c, UTF8STRING s) : sdata(s), vdata(), content(&sdata), vType(STRING), vCode(c) {}

    Variant(int c, Coord crd) : sdata(std::string()), vdata(crd), content(&vdata), vType(COORD), vCode(c) {}

    Variant(const Variant& d) : sdata(d.sdata), vdata(d.vdata), content(d.content), vType(d.vType), vCode(d.vCode) {
        if (d.vType == COORD)
            content.v = &vdata;
        if (d.vType == STRING)
            content.s = &sdata;
    }

    ~Variant() {
    }

    void AddString(int c, UTF8STRING s) { vType = STRING; sdata = s; content.s = &sdata; vCode = c; }
    void AddInt(int c, int i) { vType = INTEGER; content.i = i; vCode = c; }
    void AddDouble(int c, double d) { vType = DOUBLE; content.d = d; vCode = c; }
    void AddCoord(int c, Coord v) { vType = COORD; vdata = v; content.v = &vdata; vCode = c; }
    void SetCoordX(double d) { if (vType == COORD) vdata.x = d; }
    void SetCoordY(double d) { if (vType == COORD) vdata.y = d; }
    void SetCoordZ(double d) { if (vType == COORD) vdata.z = d; }
    enum TYPE type() const { return vType; }
    int code() { return vCode; }            /*!< returns dxf code of this value*/



private:
    std::string sdata;
    Coord vdata;

private:
    union  VarContent {
        UTF8STRING* s;
        dint32 i;
        double d;
        Coord* v;

        VarContent(UTF8STRING* sd) :s(sd) {}
        VarContent(dint32 id) :i(id) {}
        VarContent(double dd) :d(dd) {}
        VarContent(Coord* vd) :v(vd) {}
    };

public:
    VarContent content;
private:
    enum TYPE vType;
    int vCode;            /*!< dxf code of this value*/

};

const std::unordered_map< const char*, Version > DwgVersionStrings{
    { "MC0.0",  MC00 },
    { "AC1.2",  AC12 },
    { "AC1.4",  AC14 },
    { "AC1.50", AC150 },
    { "AC2.10", AC210 },
    { "AC1002", AC1002 },
    { "AC1003", AC1003 },
    { "AC1004", AC1004 },
    { "AC1006", AC1006 },
    { "AC1009", AC1009 },
    { "AC1012", AC1012 },
    { "AC1014", AC1014 },
    { "AC1015", AC1015 },
    { "AC1018", AC1018 },
    { "AC1021", AC1021 },
    { "AC1024", AC1024 },
    { "AC1027", AC1027 },
    { "AC1032", AC1032 },
};