/*!
	@file	Util/Any.h

	@brief	���ł�����^�B
			TypeErasure�B

	Copyright(C) 2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author	Dila
 */
#pragma once

#include <typeinfo>

namespace Marda
{
	/**
	 *	@brief	���ł�����^�B
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
		 *	@brief	�R���X�g���N�^
		 */
		template <class T>
		Any(const T& value);

		Any(const Any& value);

		/**
		 *	@brief	�f�X�g���N�^
		 */
		~Any();

		/**
		 *	@brief	�l�̑��
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

