/*!
	@file	Network/Message.h
	
	@author	E.Nakayama
*/
#pragma once
	
namespace Marda
{
	namespace Network
	{
		class Message
		{
		public:
			Message()
			{
				Reset();
			}
			
			void Reset()
			{
			}
			
			void OnRead(const uint8* pData, uint32 size)
			{
			}
			
		private:
			uint16 _Id;				//!< メッセージID。
			uint32 _dataSize;		//!< メッセージデータサイズ。
			std::vector<uint8> _data;	//!< データ。
			
			typedef void (Message::ReadFunc*)(const uint8*, uint32);
			ReadFunc* _pfnRead;
			
			static void ReadMessageId(const uint8* pdata, uint32 size)
			{
				size_t sz = _data.size();
				_data.resize(sz + size);
				memcpy(&_data[sz], pdata, size);
				if (_data.size() >= sizeof(uint16)) 
				{
					_Id = *reinterpret_cast<uint16*>(&_data[0]);
					_parsed += sizeof(uint16);
					// TODOOOOOOOOOOOOO:次にメッセージデータサイズ、メッセージデータ…しかし複数のメッセージ分のデータが届いた時どうする。
					
				}
			}
			
		};	// end class Message
		
	}	// end namespace Network
	
}	// end namespace Marda

