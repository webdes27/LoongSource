//-----------------------------------------------------------------------------
// File: new_protect
// Auth: Lyp
// Date: 2004-2-26	
// Last: 2004-2-26
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "new_protect.h"

namespace vEngine {
//-----------------------------------------------------------------------------
// ��new�Ľ�����м��,�������ʧ��,����ʾ�û�ѡ���Ƿ� �˳�/����/����
//-----------------------------------------------------------------------------
INT NewHandle( size_t size )
{
	// ��ʾ�û�,���û�����
	TObjRef<Debug>()->ErrMsg(_T("MALLOC(or NEW) for %lu bytes failed\r\n"), size);
	return 1;
}

}