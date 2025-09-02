#include"Header.h"

void Header::AddComment(std::string c)
{
}

bool Header::ParseCode(int code, const std::unique_ptr<DxfReader>& reader)
{
    if (nullptr == m_curr && 9 != code) {
     
        return false;
    } 
    switch (code)
    {
    case 9:
        m_curr = new Variant();
        m_name = reader->GetString();
        if (m_version < AC1015 && m_name == "$DIMUNIT")
            m_name = "$DIMLUNIT";
        m_vars[m_name] = m_curr;
        break;
    case 1:
        m_curr->AddString(code, reader->GetUtf8String());
        if (m_name == "$ACADVER") {
            reader->SetVersion(*m_curr->content.s, true);
            m_version = reader->GetVersion();
        }
        break;
        break;
    default:
        break;
    }
}
