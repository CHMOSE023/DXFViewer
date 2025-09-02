#include"TextCodec.h"
#include "Tables.h"
#include "Table936.h"
#include "Table949.h"
#include "Table950.h" 

#include <algorithm>  // std::transform
#include <cctype>     // std::toupper
#include <string>       // std::string
#include <sstream>      // std::istringstream
#include <cstdio>       // sscanf
#include <memory>       // std::unique_ptr
#include <iomanip>      // std::uppercase, std::setfill, std::setw, std::hex 
#include "Table932.h"

TextCodec::TextCodec()  : m_version{ AC1021 } , m_conv(new Converter(nullptr, 0))
{
}

TextCodec::~TextCodec() = default;


std::string TextCodec::FromUtf8(const std::string& s) {
    return m_conv->FromUtf8(s);
}

std::string TextCodec::ToUtf8(const std::string& s) {
    return m_conv->ToUtf8(s);
    
}  

void TextCodec::SetVersion(const std::string& v, bool dxfFormat)
{
    m_version = UNKNOWNV;
    for ( auto it = DwgVersionStrings.begin(); it != DwgVersionStrings.end(); ++it )
    {
        if ( std::strcmp( v.c_str(), it->first ) == 0 ) {
            m_version = it->second;
            SetVersion( it->second, dxfFormat);
            break;
        }
    }
}

void TextCodec::SetVersion(Version v, bool dxfFormat)
{
    switch (v)
    {
    case UNKNOWNV:
    case MC00:
    case AC12:
    case AC14:
    case AC150:
    case AC210:
    case AC1002:
    case AC1003:
    case AC1004:
        // unhandled?
        break;

    case AC1006:
    case AC1009:
    {
        m_version = AC1009;
        m_cp = "ANSI_1252";
        SetCodePage(m_cp, dxfFormat);
        break;
    }

    case AC1012:
    case AC1014:
    case AC1015:
    case AC1018:
    {
        m_version = AC1015;
        //            if (cp.empty()) { //codepage not set, initialize ??
        m_cp = "ANSI_1252";
        SetCodePage(m_cp, dxfFormat);
        //          }
        break;
    }

    case AC1021:
    case AC1024:
    case AC1027:
    case AC1032:
    {
        m_version = AC1021;
        if (dxfFormat)
            m_cp = "UTF-8";//RLZ: can be UCS2 or UTF-16 16bits per char
        else
            m_cp = "UTF-16";//RLZ: can be UCS2 or UTF-16 16bits per char
        SetCodePage(m_cp, dxfFormat);
        break;
    }
    }
}


void TextCodec::SetCodePage(const std::string& c, bool dxfFormat)
{
    m_cp = CorrectCodePage(c);
    m_conv.reset();
    if (m_version ==  AC1009 || m_version ==  AC1015) {
        if (m_cp == "ANSI_874")
            m_conv.reset(new ConvTable(Table874, CPLENGTHCOMMON));
        else if (m_cp == "ANSI_932")
            m_conv.reset(new Conv932Table());
        else if (m_cp == "ANSI_936")
            m_conv.reset(new ConvDBCSTable(Table936, LeadTable936, DoubleTable936, CPLENGTH936));
        else if (m_cp == "ANSI_949")
            m_conv.reset(new ConvDBCSTable(Table949, LeadTable949, DoubleTable949, CPLENGTH949));
        else if (m_cp == "ANSI_950")
            m_conv.reset(new ConvDBCSTable(Table950, LeadTable950, DoubleTable950, CPLENGTH950));
        else if (m_cp == "ANSI_1250")
            m_conv.reset(new ConvTable(Table1250, CPLENGTHCOMMON));
        else if (m_cp == "ANSI_1251")
            m_conv.reset(new ConvTable(Table1251, CPLENGTHCOMMON));
        else if (m_cp == "ANSI_1253")
            m_conv.reset(new ConvTable(Table1253, CPLENGTHCOMMON));
        else if (m_cp == "ANSI_1254")
            m_conv.reset(new ConvTable(Table1254, CPLENGTHCOMMON));
        else if (m_cp == "ANSI_1255")
            m_conv.reset(new ConvTable(Table1255, CPLENGTHCOMMON));
        else if (m_cp == "ANSI_1256")
            m_conv.reset(new ConvTable(Table1256, CPLENGTHCOMMON));
        else if (m_cp == "ANSI_1257")
            m_conv.reset(new ConvTable(Table1257, CPLENGTHCOMMON));
        else if (m_cp == "ANSI_1258")
            m_conv.reset(new ConvTable(Table1258, CPLENGTHCOMMON));
        else if (m_cp == "UTF-8") { //DXF older than 2007 are write in win codepages
            m_cp = "ANSI_1252";
            m_conv.reset(new Converter(nullptr, 0));
        }
        else
            m_conv.reset(new ConvTable(Table1252, CPLENGTHCOMMON));
    }
    else {
        if (dxfFormat)
            m_conv.reset(new Converter(nullptr, 0));//utf16 to utf8
        else
            m_conv.reset(new ConvUTF16());//utf16 to utf8
    }
}

std::string TextCodec::CorrectCodePage(const std::string& s)
{
    //stringstream cause crash in OS/X, bug#3597944
    std::string cp = s;
    std::transform(cp.begin(), cp.end(), cp.begin(), toupper);
    //Latin/Thai
    if (cp == "ANSI_874" || cp == "CP874" || cp == "ISO8859-11" || cp == "TIS-620") {
        return "ANSI_874";
        //Central Europe and Eastern Europe
    }
    else if (cp == "ANSI_1250" || cp == "CP1250" || cp == "ISO8859-2") {
        return "ANSI_1250";
        //Cyrillic script
    }
    else if (cp == "ANSI_1251" || cp == "CP1251" || cp == "ISO8859-5" || cp == "KOI8-R" ||
        cp == "KOI8-U" || cp == "IBM 866") {
        return "ANSI_1251";
        //Western Europe
    }
    else if (cp == "ANSI_1252" || cp == "CP1252" || cp == "LATIN1" || cp == "ISO-8859-1" ||
        cp == "CP819" || cp == "CSISO" || cp == "IBM819" || cp == "ISO_8859-1" || cp == "APPLE ROMAN" ||
        cp == "ISO8859-1" || cp == "ISO8859-15" || cp == "ISO-IR-100" || cp == "L1" || cp == "IBM 850") {
        return "ANSI_1252";
        //Greek
    }
    else if (cp == "ANSI_1253" || cp == "CP1253" || cp == "ISO8859-7") {
        return "ANSI_1253";
        //Turkish
    }
    else if (cp == "ANSI_1254" || cp == "CP1254" || cp == "ISO8859-9" || cp == "ISO8859-3") {
        return "ANSI_1254";
        //Hebrew
    }
    else if (cp == "ANSI_1255" || cp == "CP1255" || cp == "ISO8859-8") {
        return "ANSI_1255";
        //Arabic
    }
    else if (cp == "ANSI_1256" || cp == "CP1256" || cp == "ISO8859-6") {
        return "ANSI_1256";
        //Baltic
    }
    else if (cp == "ANSI_1257" || cp == "CP1257" || cp == "ISO8859-4" || cp == "ISO8859-10" || cp == "ISO8859-13") {
        return "ANSI_1257";
        //Vietnamese
    }
    else if (cp == "ANSI_1258" || cp == "CP1258") {
        return "ANSI_1258";

        //Japanese
    }
    else if (cp == "ANSI_932" || cp == "SHIFT-JIS" || cp == "SHIFT_JIS" || cp == "CSSHIFTJIS" ||
        cp == "CSWINDOWS31J" || cp == "MS_KANJI" || cp == "X-MS-CP932" || cp == "X-SJIS" ||
        cp == "EUCJP" || cp == "EUC-JP" || cp == "CSEUCPKDFMTJAPANESE" || cp == "X-EUC" ||
        cp == "X-EUC-JP" || cp == "JIS7") {
        return "ANSI_932";
        //Chinese PRC GBK (XGB) simplified
    }
    else if (cp == "ANSI_936" || cp == "GBK" || cp == "GB2312" || cp == "CHINESE" || cp == "CN-GB" ||
        cp == "CSGB2312" || cp == "CSGB231280" || cp == "CSISO58BG231280" ||
        cp == "GB_2312-80" || cp == "GB231280" || cp == "GB2312-80" ||
        cp == "ISO-IR-58" || cp == "GB18030") {
        return "ANSI_936";
        //Korean
    }
    else if (cp == "ANSI_949" || cp == "EUCKR") {
        return "ANSI_949";
        //Chinese Big5 (Taiwan, Hong Kong SAR)
    }
    else if (cp == "ANSI_950" || cp == "BIG5" || cp == "CN-BIG5" || cp == "CSBIG5" ||
        cp == "X-X-BIG5" || cp == "BIG5-HKSCS") {
        return "ANSI_950";

        //celtic
        /*    } else if (cp=="ISO8859-14") {
               return "ISO8859-14";
            } else if (cp=="TSCII") {
                return "TSCII"; //tamil
            }*/

    }
    else if (cp == "UTF-8" || cp == "UTF8" || cp == "UTF8-BIT") {
        return "UTF-8";
    }
    else if (cp == "UTF-16" || cp == "UTF16" || cp == "UTF16-BIT") {
        return "UTF-16";
    }

    return "ANSI_1252";
}


std::string Converter::EncodeText(const std::string& stmp)
{
    int code;
#if defined(__APPLE__)
    int Succeeded = sscanf(&(stmp.substr(3, 4)[0]), "%x", &code);
    if (!Succeeded || Succeeded == EOF)
        code = 0;
#else
    std::istringstream sd(stmp.substr(3, 4));
    sd >> std::hex >> code;
#endif
    return EncodeNum(code);
}

std::string Converter::DecodeText(int c) {
    std::string res = "\\U+";
    std::string num;
#if defined(__APPLE__)
    std::string str(16, '\0');
    snprintf(&(str[0]), 16, "%04X", c);
    num = str;
#else
    std::stringstream ss;
    ss << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << c;
    ss >> num;
#endif
    res += num;
    return res;
}


std::string Converter::EncodeNum(int c) {
    unsigned char ret[5];
    if (c < 128) { // 0-7F US-ASCII 7 bits
        ret[0] = c;
        ret[1] = 0;
    }
    else if (c < 0x800) { //80-07FF 2 bytes
        ret[0] = 0xC0 | (c >> 6);
        ret[1] = 0x80 | (c & 0x3f);
        ret[2] = 0;
    }
    else if (c < 0x10000) { //800-FFFF 3 bytes
        ret[0] = 0xe0 | (c >> 12);
        ret[1] = 0x80 | ((c >> 6) & 0x3f);
        ret[2] = 0x80 | (c & 0x3f);
        ret[3] = 0;
    }
    else { //10000-10FFFF 4 bytes
        ret[0] = 0xf0 | (c >> 18);
        ret[1] = 0x80 | ((c >> 12) & 0x3f);
        ret[2] = 0x80 | ((c >> 6) & 0x3f);
        ret[3] = 0x80 | (c & 0x3f);
        ret[4] = 0;
    }
    return std::string(reinterpret_cast<char*>(ret));
}

std::string Converter::ToUtf8(const std::string& s) {
    std::string result;
    int j = 0;
    unsigned int i = 0;
    for (i = 0; i < s.length(); i++) {
        unsigned char c = s.at(i);
        if (c < 0x80) { //ascii check for /U+????
            if (c == '\\' && i + 6 < s.length() && s.at(i + 1) == 'U' && s.at(i + 2) == '+') {
                result += s.substr(j, i - j);
                result += EncodeText(s.substr(i, 7));
                i += 6;
                j = i + 1;
            }
        }
        else if (c < 0xE0) {//2 bits
            i++;
        }
        else if (c < 0xF0) {//3 bits
            i += 2;
        }
        else if (c < 0xF8) {//4 bits
            i += 3;
        }
    }
    result += s.substr(j);

    return result;
}



/** 's' is a string with at least 4 bytes length
** returned 'b' is byte length of encoded char: 2,3 or 4
**/
int Converter::DecodeNum(const std::string& s, int* b) {
    int code = 0;
    unsigned char c = s.at(0);
    if ((c & 0xE0) == 0xC0) { //2 bytes
        code = (c & 0x1F) << 6;
        code = (s.at(1) & 0x3F) | code;
        *b = 2;
    }
    else if ((c & 0xF0) == 0xE0) { //3 bytes
        code = (c & 0x0F) << 12;
        code = ((s.at(1) & 0x3F) << 6) | code;
        code = (s.at(2) & 0x3F) | code;
        *b = 3;
    }
    else if ((c & 0xF8) == 0xF0) { //4 bytes
        code = (c & 0x07) << 18;
        code = ((s.at(1) & 0x3F) << 12) | code;
        code = ((s.at(2) & 0x3F) << 6) | code;
        code = (s.at(3) & 0x3F) | code;
        *b = 4;
    }

    return code;
}
  
std::string ConvUTF16::FromUtf8(const std::string& s) {
    DRW_UNUSED(s);
    //RLZ: to be written (only needed for write dwg 2007+)
    return std::string();
}

std::string ConvUTF16::ToUtf8(const std::string& s) {//RLZ: pending to write
    std::string res;
    for (auto it = s.begin(); it < s.end(); ++it) {
        unsigned char c1 = *it;
        unsigned char c2 = *(++it);
        duint16 ch = (c2 << 8) | c1;
        res += EncodeNum(ch);
    } //end for

    return res;
}


std::string ConvTable::FromUtf8(const std::string& s) {
    std::string result;
    bool notFound;
    int code;

    int j = 0;
    for (unsigned int i = 0; i < s.length(); i++) {
        unsigned char c = s.at(i);
        if (c > 0x7F) { //need to decode
            result += s.substr(j, i - j);
            std::string part1 = s.substr(i, 4);
            int l;
            code = DecodeNum(part1, &l);
            j = i + l;
            i = j - 1;
            notFound = true;
            for (int k = 0; k < m_cpLength; k++) {
                if (m_table[k] == code) {
                    result += CPOFFSET + k; //translate from table
                    notFound = false;
                    break;
                }
            }
            if (notFound)
                result += DecodeText(code);
        }
    }
    result += s.substr(j);

    return result;
}

std::string  ConvTable::ToUtf8(const std::string& s) {
    std::string res;
    for (auto it = s.begin(); it < s.end(); ++it) {
        unsigned char c = *it;
        if (c < 0x80) {
            //check for \U+ encoded text
            if (c == '\\') {
                if (s.end() - it > 6 && *(it + 1) == 'U' && *(it + 2) == '+') {
                    res += EncodeText(std::string(it, it + 7));
                    it += 6;
                }
                else {
                    res += c; //no \U+ encoded text write
                }
            }
            else
                res += c; //c!='\' ascii char write
        }
        else {//end c < 0x80
            res += EncodeNum(m_table[c - 0x80]); //translate from table
        }
    } //end for

    return res;
}



std::string ConvDBCSTable::FromUtf8(const std::string& s) {
    std::string result;
    bool notFound;
    int code;

    int j = 0;
    for (unsigned int i = 0; i < s.length(); i++) {
        unsigned char c = s.at(i);
        if (c > 0x7F) { //need to decode
            result += s.substr(j, i - j);
            std::string part1 = s.substr(i, 4);
            int l;
            code = DecodeNum(part1, &l);
            j = i + l;
            i = j - 1;
            notFound = true;
            for (int k = 0; k < m_cpLength; k++) {
                if (m_doubleTable[k][1] == code) {
                    int data = m_doubleTable[k][0];
                    char d[3];
                    d[0] = data >> 8;
                    d[1] = data & 0xFF;
                    d[2] = '\0';
                    result += d; //translate from table
                    notFound = false;
                    break;
                }
            }
            if (notFound)
                result += DecodeText(code);
        } //direct conversion
    }
    result += s.substr(j);

    return result;
}

std::string ConvDBCSTable::ToUtf8(const std::string& s) {
    std::string res;
    for (auto it = s.begin(); it < s.end(); ++it) {
        bool notFound = true;
        unsigned char c = *it;
        if (c < 0x80) {
            notFound = false;
            //check for \U+ encoded text
            if (c == '\\') {
                if (s.end() - it > 6 && *(it + 1) == 'U' && *(it + 2) == '+') {
                    res += EncodeText(std::string(it, it + 7));
                    it += 6;
                }
                else {
                    res += c; //no \U+ encoded text write
                }
            }
            else
                res += c; //c!='\' ascii char write
        }
        else if (c == 0x80) {//1 byte table
            notFound = false;
            res += EncodeNum(0x20AC);//euro sign
        }
        else {//2 bytes
            ++it;
            int code = (c << 8) | static_cast<unsigned char>(*it);
            int sta = m_leadTable[c - 0x81];
            int end = m_leadTable[c - 0x80];
            for (int k = sta; k < end; k++) {
                if (m_doubleTable[k][0] == code) {
                    res += EncodeNum(m_doubleTable[k][1]); //translate from table
                    notFound = false;
                    break;
                }
            }
        }
        //not found
        if (notFound) res += EncodeNum(NOTFOUND936);
    } //end for

    return res;
}

Conv932Table::Conv932Table() :Converter(Table932, CPLENGTH932) {
}

std::string Conv932Table::FromUtf8(const std::string& s) {
    std::string result;
    bool notFound;
    int code;

    int j = 0;
    for (unsigned int i = 0; i < s.length(); i++) {
        unsigned char c = s.at(i);
        if (c > 0x7F) { //need to decode
            result += s.substr(j, i - j);
            std::string part1 = s.substr(i, 4);
            int l;
            code = DecodeNum(part1, &l);
            j = i + l;
            i = j - 1;
            notFound = true;
            // 1 byte table
            if (code > 0xff60 && code < 0xFFA0) {
                result += code - CPOFFSET932; //translate from table
                notFound = false;
            }
            if (notFound && (code < 0xF8 || (code > 0x390 && code < 0x542) ||
                (code > 0x200F && code < 0x9FA1) || code>0xF928)) {
                for (int k = 0; k < m_cpLength; k++) {
                    if (DoubleTable932[k][1] == code) {
                        int data = DoubleTable932[k][0];
                        char d[3];
                        d[0] = data >> 8;
                        d[1] = data & 0xFF;
                        d[2] = '\0';
                        result += d; //translate from table
                        notFound = false;
                        break;
                    }
                }
            }
            if (notFound)
                result += DecodeText(code);
        } //direct conversion
    }
    result += s.substr(j);

    return result;
}

std::string Conv932Table::ToUtf8(const std::string& s) {
    std::string res;
    for (auto it = s.begin(); it < s.end(); ++it) {
        bool notFound = true;
        unsigned char c = *it;
        if (c < 0x80) {
            notFound = false;
            //check for \U+ encoded text
            if (c == '\\') {
                if (s.end() - it > 6 && *(it + 1) == 'U' && *(it + 2) == '+') {
                    res += EncodeText(std::string(it, it + 7));
                    it += 6;
                }
                else {
                    res += c; //no \U+ encoded text write
                }
            }
            else
                res += c; //c!='\' ascii char write
        }
        else if (c > 0xA0 && c < 0xE0) {//1 byte table
            notFound = false;
            res += EncodeNum(c + CPOFFSET932); //translate from table
        }
        else {//2 bytes
            ++it;
            int code = (c << 8) | static_cast<unsigned char>(*it);
            int sta = 0;
            int end = 0;
            if (c > 0x80 && c < 0xA0) {
                sta = LeadTable932[c - 0x81];
                end = LeadTable932[c - 0x80];
            }
            else if (c > 0xDF && c < 0xFD) {
                sta = LeadTable932[c - 0xC1];
                end = LeadTable932[c - 0xC0];
            }
            if (end > 0) {
                for (int k = sta; k < end; k++) {
                    if (DoubleTable932[k][0] == code) {
                        res += EncodeNum(DoubleTable932[k][1]); //translate from table
                        notFound = false;
                        break;
                    }
                }
            }
        }
        //not found
        if (notFound) res += EncodeNum(NOTFOUND932);
    } //end for

    return res;
}
