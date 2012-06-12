/*!
	@file	Util/Any.h

	@brief	何でも入る型。
			TypeErasure。

	Copyright(C) 2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author	Dila
 */
#pragma once

#include <typeinfo>

namespace Marda
{
	/**
	 *	@brief	何でも入る型。
	 */
	class Any
	{
		struct HolderBase
		{
			virtual ~HolderBase() {}
			virtual HolderBase* Clone() const = 0;
			virtual const std::type_info& GetType() const = 0;
		};
		template <class T> struct Holder;

	public:
		/**
		 *	@brief	コンストラクタ
		 */
		template <class T>
		Any(const T& value);

		Any(const Any& value);

		/**
		 *	@brief	デストラクタ
		 */
		~Any();

		/**
		 *	@brief	値の代入
		 */
		template <class T>
		Any& operator = (const T& rhs);

		Any& operator = (const Any& rhs);

		const std::type_info& GetType() const;
		
	private:
		void Reset(HolderBase* pHolder);

		HolderBase* _pHolder;

	};	// end class Any

}	// end namespace Marda

#include "Any.hpp"

