#pragma once

#include "resource.h"

class ApplicationFrame
{
public:
	ApplicationFrame();
	~ApplicationFrame();
	
	bool Create(HWND hWnd);
	void Release();
	void UpdateFrame();

private:
	struct Impl;
	std::auto_ptr<Impl> _pimpl;
	
};	// end class ApplicationCore
