#include "DxfReader.h"

bool DxfReader::ReadRec(int* codeData)
{
	int code{-1};
	if (!ReadCode(&code)) // 调用子类的ReadCode
		return false;  
	*codeData = code;
	if (code<10)
	{
		ReadString();
	}
	else if (code < 60) {
		ReadDouble();
	}

	return m_filestr->good();
};

 