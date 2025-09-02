#pragma once
#include <string>
#include <memory>
#include "DxfReader.h"
#include "DxfWriter.h"  
#include "DxfInterface.h"
#include "Base.h"
#include "Header.h"

class  DxfRW
{
public:
	DxfRW(const char* fileName);
	~DxfRW();	 
	bool Read(DxfInterface* dxfInterface, bool ext);

private:
	bool ProcessDxf();
	bool ProcessHeader();
	bool ProcessTables();
	bool ProcessBlocks();
	bool ProcessEntities(bool isblock);
	bool ProcessObjects();   


	bool ProcessLine();

	 
	bool SetError(const  Error lastError);
private:
	std::unique_ptr<DxfReader> m_reader; 

	DxfInterface*   m_pDxfInterface;
	Header          m_header;
	Error           m_error;
	Version         m_version{ UNKNOWNV }; 
	std::string     m_fileName; 	 
	bool            m_applyExt;
	bool            m_binFile;
};
