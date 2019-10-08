#pragma once

#include "RecordTypes.h"
#include <vector>
#include <string>

namespace MTEF
{ 
	struct EQNOLEFILEHDR : MTEFBase{
		WORD    cbHdr;     // length of header, sizeof(EQNOLEFILEHDR) = 28 bytes
		DWORD   version;   // hiword = 2, loword = 0
		WORD    cf;        // clipboard format ("MathType EF")
		DWORD   cbObject;  // length of MTEF data following this header in bytes
		DWORD   reserved1; // not used
		DWORD   reserved2; // not used
		DWORD   reserved3; // not used
		DWORD   reserved4; // not used
		//const std::map<>
		UINT size()
		{
			return cbHdr;
		}
	};
	
	
	struct MTEFHeader : MTEFBase{
		bit MTEF_version;
		bit generating_platform;
		bit generating_product;
		bit product_version;
		bit product_subversion;
		std::string application_key;
		bit equation_options;
		UINT size()
		{
			return application_key.length() + 6;
		}
	};

	struct MTEF : MTEFBase{

	};
}