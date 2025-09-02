#include"DxfReader.h"
#include <fstream>

class DxfReaderBinary:public DxfReader
{
public :
	DxfReaderBinary(std::ifstream* stream) :DxfReader(stream) { m_skip = false; };
	~DxfReaderBinary() {};
	virtual bool ReadString(std::string* text) { return false; };
	virtual bool ReadString() { return false; };
	virtual bool ReadCode(int* code) { return false; };
	virtual bool ReadDouble() { return false; }; 
};
 
 