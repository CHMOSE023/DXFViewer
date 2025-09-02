#include "DxfReaderAscii.h"
#include <string>

bool DxfReaderAscii::ReadCode(int* code)
{
	std::string text;
	std::getline(*m_filestr, text);
	*code = atoi(text.c_str());
	return  (m_filestr->good());
}

bool DxfReaderAscii::ReadDouble()
{

	return  (m_filestr->good());
}

bool DxfReaderAscii::ReadString(std::string* text)
{
	 

	return  (m_filestr->good());
}

bool DxfReaderAscii::ReadString(){

	m_type = STRING;
	std::getline(*m_filestr, m_strData);
	// 移除可能存在的\r
	if (!m_strData.empty() && m_strData.at(m_strData.size() - 1) == '\r')
		m_strData.erase(m_strData.size() - 1);

	return  (m_filestr->good());
}

 