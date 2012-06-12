/*!
	@file	Util/Serializer.h
	@brief	シリアライザ／デシリアライザ。

	Copyright(C) 2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author	Dila
 */
#pragma once

#include "Marda.h"

namespace Marda
{
	/*!
	 	@brief	簡易シリアライザ。
	 */
	struct Serializer
	{
		std::vector<uint8> m_Data;	///< 実データ

		Serializer()
		{
		};

		template <class T>
		Serializer& operator << (const T& value)
		{
			size_t pos = m_Data.size();
			m_Data.resize(m_Data.size() + sizeof(T));
			T* dst = reinterpret_cast<T*>(&m_Data[pos]);
			*dst = value;
			return *this;
		}

		template <class TChar>
		Serializer& operator << (const std::basic_string<TChar>& value)
		{
			// まずサイズを詰める
			uint32 size = static_cast<uint32>(sizeof(TChar) * (value.length() + 1));
			*this << size;

			// 実データ。
			size_t pos = m_Data.size();
			m_Data.resize(m_Data.size() + size);
			TChar* dst = reinterpret_cast<TChar*>(&m_Data[pos]);
			memcpy(dst, value.c_str(), size);
			return *this;
		}
	};	// end struct Serializer

	/*!
		@brief	簡易デシリアライザ。
	 */
	struct Deserializer
	{
		const std::vector<uint8>* m_pData;
		size_t m_Cursor;

		Deserializer(const std::vector<uint8>& data) :
			m_pData(&data),
			m_Cursor(0)
		{
		}

		bool IsEnd()
		{
			return m_Cursor >= m_pData->size();
		}

		template <class T>
		Deserializer& operator >> (T& value)
		{
			size_t size = sizeof(T);
			if (m_Cursor + size <= m_pData->size() && !m_pData->empty()) {
				const T* src = reinterpret_cast<const T*>(&(*m_pData)[m_Cursor]);
				value = *src;
				m_Cursor += size;
			}
			return *this;
		}
		
		template <class TChar>
		Deserializer& operator >> (std::basic_string<TChar>& value)
		{
			// まずサイズを取得
			uint32 size = 0;
			*this >> size;

			if (m_Cursor + size <= m_pData->size() && !m_pData->empty()) {
				// 実データ。
				const TChar* src = reinterpret_cast<const TChar*>(&(*m_pData)[m_Cursor]);
				value = src;
				m_Cursor += size;
			}
			return *this;
		}
	};	// end struct Deserializer

}	// end namespace Marda


