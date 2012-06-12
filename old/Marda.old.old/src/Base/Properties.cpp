/*!
	@file Properties.cpp


	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */

#include "Marda.h"

using namespace Marda;

// --------------------------------------------------------------------------------------------- //
// Properties class implementation 
struct Properties::Impl
{
	#if MSVC_VER >= MSVC_9
		typedef std::unordered_map<String, Variant> Map;
	#else
		typedef std::map<String, Variant> Map;
	#endif

	Map _props;

	Impl()
	{
	}

	~Impl()
	{
	}

	bool AddProperty(const String& key, const Variant& val)
	{
		Map::iterator i = _props.find(key);
		if (i != _props.end()) return false;
		_props.insert(Map::value_type(key, val));
		return true;
	}

	void RemoveProperty(const String& key)
	{
		Map::iterator i = _props.find(key);
		if (i == _props.end()) return;
		_props.erase(i);
	}

	Variant* GetProperty(const String& key)
	{
		Map::iterator i = _props.find(key);
		if (i == _props.end()) return NULL;
		return &i->second;
	}

	const Variant* GetProperty(const String& key) const
	{
		return const_cast<Impl*>(this)->GetProperty(key);
	}

	Variant& operator [] (const String& key)
	{
		Variant* val = GetProperty(key);
		if (val) return *val;
		AddProperty(key, Variant());
		val = GetProperty(key);
		Assert(val, "Failed to add property!");
		return *val;
	}

	const Variant& operator [] (const String& key) const
	{
		const Variant* val = GetProperty(key);
		if (val) return *val;
		Assert(val, "Failed to add property!");
	}

	Impl& operator = (const Impl& rhs)
	{
		_props = rhs._props;
		return *this;
	}

};	// end struct Properties::Impl


// --------------------------------------------------------------------------------------------- //
Properties::Properties() :
	_pimpl(new Impl())
{
}

Properties::Properties(const Properties& rhs) :
	_pimpl(new Impl())
{
	*_pimpl = *rhs._pimpl;
}

Properties::~Properties()
{
}

bool Properties::AddProperty(const String& key, const Variant& val)
{
	return _pimpl->AddProperty(key, val);
}

void Properties::RemoveProperty(const String& key)
{
	_pimpl->RemoveProperty(key);
}


Variant* Properties::GetProperty(const String& key)
{
	return _pimpl->GetProperty(key);
}

const Variant* Properties::GetProperty(const Marda::String &key) const
{
	return _pimpl->GetProperty(key);
}

Variant& Properties::operator [] (const String& key)
{
	return _pimpl->operator [] (key);
}

const Variant& Properties::operator [] (const String& key) const
{
	return _pimpl->operator [] (key);
}

Properties& Properties::operator = (const Marda::Properties &rhs)
{
	*_pimpl.get() = *rhs._pimpl.get();
	return *this;
}
