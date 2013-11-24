#pragma once
class CPubFun
{
public:
	CPubFun(void);
	~CPubFun(void);

	//获得最后出错信息描述
	static CString GetLastErrorMessage();
};

