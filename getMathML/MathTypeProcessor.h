#pragma once
class CMathTypeProcessor
{
public:
	CMathTypeProcessor(void);
	virtual ~CMathTypeProcessor(void);
	void ConvertToXml(char * fn);
	HMODULE m_hMT6Dll;
};

