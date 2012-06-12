/*!
	@file	Util/DataStream.h

	@brief	�ȈՃf�[�^�X�g���[���B
			STL�̃X�g���[���݊���ڎw�������B

	Copyright(C) 2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author	Dila
 */
#pragma once

#include "Marda.h"

namespace Marda
{
	/*!
		@brief	�ȈՃf�[�^�X�g���[���N���X
	 */
	class DataStream
	{
	public:
		DataStream();

		DataStream(const std::vector<uint8>& src);

		DataStream(const std::vector<sint8>& src);

		DataStream(const void* src, size_t size);

		~DataStream();

		/*!
			@brief	���݂̓��̓J�[�\�����擾����B
		 */
		size_t GetCursor() const;

		/*!
			@brief	���݂̃X�g���[���T�C�Y���擾����B
		 */
		size_t GetSize() const;

		/*!
			@brief	���݂̃X�g���[���̐擪�A�h���X���擾����B
		 */
		const uint8* GetData() const;

		/*!
			@brief	�f�[�^���X�g���[���ɏo�͂���B
		 */
		template <class T>
		DataStream& operator << (const T& value);

		/*!
			@brief	������f�[�^���X�g���[���ɏo�͂���B
		 */
		template <class TChar>
		DataStream& operator << (const std::basic_string<TChar>& value);

		/*!
			@brief	�X�g���[������f�[�^����͂���B
		 */
		template <class T>
		DataStream& operator >> (T& value);

		/*!
			@brief	�X�g���[�����當����f�[�^����͂���B
		 */
		template <class TChar>
		DataStream& operator >> (std::basic_string<TChar>& value);

	private:
		std::vector<uint8> _data;	///< �f�[�^
		size_t _cursor;	///< ���̓J�[�\��
				
	};

}	// end namespace Marda

#include "DataStream.hpp"

