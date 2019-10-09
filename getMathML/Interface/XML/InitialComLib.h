#ifndef _INITIAL_COM_LIB_H_
#define _INITIAL_COM_LIB_H_

//used to init COM lib before using COM and release COM lib 
//automatically when out of bound
#include "OBJBASE.h"

class CInitialComLib
{
public:
	CInitialComLib(){ m_bSuccess = SUCCEEDED(CoInitialize(NULL)); };
	~CInitialComLib(){ if(m_bSuccess) CoUninitialize(); };
private:
	bool m_bSuccess;
};


#endif