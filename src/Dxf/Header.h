#pragma once
#include "DxfReader.h" 
#include <unordered_map>
#include "Base.h"

#define SETHDRFRIENDS  friend class DxfRW; \
                       friend class DxfReader;
class  Header 
{
	SETHDRFRIENDS

public:
	Header() {};
	~Header() {};

	void AddComment(std::string c) ;
protected:
	bool ParseCode(int code, const std::unique_ptr<DxfReader>& reader);

public:
	std::unordered_map<std::string, Variant*> m_vars;
private:
	std::string  m_comments;
	std::string  m_name;
	int          m_version; // to use on read
	Variant*     m_curr{ nullptr }; 

	duint32 linetypeCtrl;
	duint32 layerCtrl;
	duint32 styleCtrl;
	duint32 dimstyleCtrl;
	duint32 appidCtrl;
	duint32 blockCtrl;
	duint32 viewCtrl;
	duint32 ucsCtrl;
	duint32 vportCtrl;
	duint32 vpEntHeaderCtrl;

	int measurement(const int unit);
};
