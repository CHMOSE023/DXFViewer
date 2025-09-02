#pragma once
#include "DxfRW.h" 
#include <fstream>
#include "DxfReaderBinary.h"
#include "DxfReaderAscii.h"
#include "Base.h"

DxfRW::DxfRW(const char* fileName)
{
	m_fileName = fileName;
	m_pDxfInterface = nullptr;
	m_binFile = false;
	m_applyExt = false;
}

DxfRW::~DxfRW()
{

}

bool DxfRW::Read(DxfInterface* dxfInterface, bool ext)
{
	m_applyExt = ext;
	std::ifstream filestr;
	if (dxfInterface == nullptr) {
		return false;
	}

	filestr.open(m_fileName.c_str(), std::ios_base::in | std::ios::binary);

	if (!filestr.is_open() || !filestr.good()) {
		return false;
	}

	char line[22];
	char line2[22] = "AutoCAD Binary DXF\r\n";

	line2[20] = (char)26;
	line2[21] = '\0';

	filestr.read(line, 22);
	filestr.close();

	m_pDxfInterface = dxfInterface;

	if (strcmp(line, line2) == 0) // strcmp比较字符串
	{
		filestr.open(m_fileName.c_str(), std::ios_base::in | std::ios::binary);
		m_binFile = true;

		filestr.seekg(22, std::ios::beg);
		m_reader = std::make_unique<DxfReaderBinary>(&filestr);
	}
	else
	{
		m_binFile = false;
		filestr.open(m_fileName.c_str(), std::ios_base::in);
		m_reader = std::make_unique<DxfReaderAscii>(&filestr);
	}

	bool isOk{ ProcessDxf() };
	filestr.close();

	m_version = (Version)m_reader->GetVersion();
	m_reader.reset();
	return isOk;

}

bool DxfRW::ProcessDxf()
{
	int code{ -1 };
	bool inSection{ false };
	m_reader->SetIgnoreComments(false);

	while (m_reader->ReadRec(&code)) {

		switch (code)
		{
		case 999:
			m_header.AddComment(m_reader->GetString());
			continue;

		case 0:
			// 忽略comments dxf 不支持
			m_reader->SetIgnoreComments(true); 

			if (!inSection) {
				std::string sectionstr{ m_reader->GetString() };

				if ("SECTION" == sectionstr)
				{
					inSection = true;
					continue;
				}

				if ("EOF" == sectionstr) {

					return true;
				}
			}
			else
			{
				if ("ENDSEC" == m_reader->GetString())
				{
					inSection = false;
				}
			} 

			break; 
		case 2:

			if (inSection)
			{
				bool processed{ false };
				std::string sectionname{ m_reader->GetString() };

				if ("HEADER" == sectionname) {
					processed = ProcessHeader();
				}
				else if ("TABLES" == sectionname) {
					processed = ProcessTables();
				}
				else if ("BLOCKS" == sectionname) {
					processed = ProcessBlocks();
				}
				else if ("ENTITIES" == sectionname) {
					processed = ProcessEntities(false);
				}
				else if ("OBJECTS" == sectionname) {
					processed = ProcessObjects();
				}
				else {
					//TODO handle CLASSES   
					continue;
				}  
				inSection = false; 
			} 
			break;
		default: 
			inSection = false;
			break;
		}
	}
	
	if (0 == code && "EOF" == m_reader->GetString()) {
		// in case the final EOF has no newline we end up here!
		// this is caused by filestr->good() which is false for missing newline on EOF
		return true;
	}

	return SetError(BAD_UNKNOWN);
	 
}

bool DxfRW::ProcessHeader()
{
	int code;
	std::string sectionstr;

	while (m_reader->ReadRec(&code))
	{
		if (code == 0)
		{
			sectionstr = m_reader->GetString();

			if (sectionstr == "ENDSEC")
			{
				//m_pDxfInterface->AddHeader(); 
				return true; 
			}

			return SetError(BAD_READ_HEADER);
		}

		if (!m_header.ParseCode(code, m_reader)) {
			return SetError(BAD_READ_HEADER);
		}


	}
	return SetError(BAD_READ_HEADER);
}

bool DxfRW::ProcessTables()
{
	return false;
}

bool DxfRW::ProcessBlocks()
{
	return false;
}

bool DxfRW::ProcessEntities(bool isblock)
{
	return false;
}

bool DxfRW::ProcessObjects()
{
	return false;
}

bool DxfRW::ProcessLine()
{
	return true;
}

bool DxfRW::SetError(const Error lastError)
{
	m_error = lastError;
	return (BAD_NONE == m_error);
}


