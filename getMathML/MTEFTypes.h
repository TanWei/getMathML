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
		UINT size()
		{
			return cbHdr;
		}
	};
	struct AppsMFCCHeader : MTEFBase{
		char  id[8];		// Must be "AppsMFCC"
		WORD  version;		// Must be 1
		DWORD totalLen;		// Total of data bytes in all comments
		DWORD dataLen;		// # of data bytes in this comment (see 'data' below) 
		std::string signature;	// Variable length, null terminated string (see below for full details)
		// dataLen bytes of data follows (aka the chunk data)
	};	
	
	struct MTEFHeader : MTEFBase{
		BYTE MTEF_version;
		BYTE generating_platform;
		BYTE generating_product;
		BYTE product_version;
		BYTE product_subversion;
		std::string application_key;
		BYTE equation_options;
	};

	struct MTEFData : MTEFBase{
		//Consists of:
		//	MTEF header
		//	equation preferences (size, style, spacing), preceded by the definition records to which they refer
		//	initial SIZE record
		//	PILE or LINE record
		//	contents of PILE or LINE
		//	END record
		std::shared_ptr<MTEFHeader> mtef_header;
		std::vector<RecordBasePtr> contents;
	};
}