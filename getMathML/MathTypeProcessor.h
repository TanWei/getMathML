#pragma once


struct MTAPI_RECT
{
	long _left;
	long _top;
	long _right;
	long _bottom;
};

struct MTAPI_DIMS
{
	int baseline;
	MTAPI_RECT bounds;
};

class CMathTypeProcessor
{
public:
	CMathTypeProcessor(void);
	virtual ~CMathTypeProcessor(void);
	void ConvertToXml(char * fn);
	HMODULE m_hMT6Dll;
};

