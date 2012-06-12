/*!
	@file	Util/Any.hpp

	@brief	メンバ関数テンプレートの定義

	Copyright(C) 2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author	Dila
 */
#pragma once


template <class T>
struct Marda::Any::Holder : public HolderBase
{
	T _value;

	Holder(const T& value) :
		_value(value)
	{
	}

	HolderBase* Clone() const
	{
		return new Holder(_value);
	}

	const std::type_info& GetType() const
	{
		return typeid(T);
	}
};

template <class T>
Marda::Any::Any(const T &value) :
	_pHolder(new Holder<T>(value))
{
}

template <class T> 
Marda::Any& 
Marda::Any::operator = (const T& rhs)
{
	Reset(new Holder<T>(rhs));
	return *this;
}

