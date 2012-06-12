/*!
	@brief	Util/Any.cpp

	ƒƒ“ƒoŠÖ”‚Ì’è‹`

	Copyright(C) 2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author	Dila
 */
#include "Marda.h"
#include "Any.h"

Marda::Any::~Any()
{
	delete _pHolder;
}

Marda::Any&
Marda::Any::operator = (const Any& rhs)
{
	Reset(rhs._pHolder->Clone());
	return *this;
}

const std::type_info&
Marda::Any::GetType() const
{
	return _pHolder->GetType();
}

void
Marda::Any::Reset(HolderBase* pHolder)
{
	delete _pHolder;
	_pHolder = pHolder;
}

