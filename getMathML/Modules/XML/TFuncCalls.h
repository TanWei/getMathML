/*
	template functions used to call member functions
*/
#ifndef _TFUNCCALLS_H_
#define _TFUNCCALLS_H_

#pragma warning(push)
#pragma warning(disable:4710)
#pragma warning(disable:4702)

//call member functions without args
//typedef template<class _R, class _Ty> _R (_Ty::TFunc)();

template<class _R, class _Ty> inline
_R TCallMemberFunc0(_Ty* p, _R (_Ty::*pm)())
{
	_R ptr;
	try
	{
		ptr = (p->*pm)();
	}catch(_com_error e)
	{
		throw CXMLError(e);
	}
	return ptr; 
}




//call member function with one arg
template<class _R, class _Ty, class _ARG1, class _ARG1TYPE> inline
_R TCallMemberFunc1(_Ty* p, _R (_Ty::*pm)(_ARG1), const _ARG1TYPE& arg1)
{
	_R ptr;
	try
	{
		ptr = (p->*pm)(arg1);
	}catch(_com_error e)
	{
		throw CXMLError(e);
	}
	return ptr;
}
//call member function with one arg without return value
template<class _Ty, class _ARG1, class _ARG1TYPE> inline
void TCallVoidMemberFunc1(_Ty* p, void (_Ty::*pm)(_ARG1), const _ARG1TYPE& arg1)
{
	try
	{
		(p->*pm)(arg1);
	}catch(_com_error e)
	{
		throw CXMLError(e);
	}
}

//call member function with two args
template<class _R, class _Ty, class _ARG1, class _ARG1TYPE, class _ARG2, class _ARG2TYPE> inline
_R TCallMemberFunc2(_Ty* p, _R (_Ty::*pm)(_ARG1,_ARG2), const _ARG1TYPE& arg1, const _ARG2TYPE& arg2)
{
	_R ptr;
	try
	{
		ptr = (p->*pm)(arg1,arg2);
	}catch(_com_error e)
	{
		throw CXMLError(e);
	}
	return ptr;
}

//call member function with three args
template<class _R, class _Ty, class _ARG1, class _ARG1TYPE, class _ARG2, class _ARG2TYPE, class _ARG3, class _ARG3TYPE> inline
_R TCallMemberFunc3(_Ty* p, _R (_Ty::*pm)(_ARG1,_ARG2,_ARG3), const _ARG1TYPE& arg1, const _ARG2TYPE& arg2, const _ARG3TYPE& arg3)
{
	_R ptr;
	try
	{
		ptr = (p->*pm)(arg1,arg2,arg3);
	}catch(_com_error e)
	{
		throw CXMLError(e);
	}
	return ptr;
}

//conert CXMLNode derived _ARG to _R
template<class _R, class _ARG> inline
_R TGetPtr(const _ARG* p)
{
	if(!p->GetNodePtr().operator bool())
	{
		return NULL;
	}

	//try to convert Node to another Node
	_R ptr;
	try
	{
		ptr = (p->GetNodePtr()).GetInterfacePtr();
	}catch(_com_error e)
	{
		throw CXMLError(e);
	}
	return ptr;
}

//if bstr is NULL, conver it to wstring will cause
//error because of strlen(NULL), so we check NULL here.
inline
std::wstring ConvertBSTRTowstring(const _bstr_t& bstr)
{
	wchar_t* p = NULL;
	try
	{
		p = bstr;
	}catch(_com_error e)
	{
		throw CXMLError(e);
	}
	return std::wstring(p==NULL ? L"" : p);
}
//change a variant to _bstr_t may cause error
//because ::VariantChangeType sometimes fails,we need to
//catch the exception
inline
std::wstring ConvertVARIANTTowstring(const _variant_t& v)
{
	_bstr_t bstr;
	try
	{
		if ((v.vt != VT_EMPTY) && (v.vt != VT_NULL))
			bstr = v;
	}catch(_com_error e)
	{
		//variant can not be converted to bstr,so just return L""
		// just catch the exception is enough
	}
	return ConvertBSTRTowstring(bstr);
}

#pragma warning(pop)

#endif