/*!
	@file	Network/Session.h
	
	@author	E.Nakayama
*/
#pragma once
	
#include "Marda/Types.h"

#if 0	
	セッションをユーザが継承したクラスにしたい
		アプリケーション固有のユーザ情報をもたせられるようにしたい
	コンストラクタでセッションの各関数を利用出来る様にしたい
	boostの実装は隠したい
	
	class Session
	{
	public: 
		struct Core;
		Session(Core& core);
		virtual void OnMessage() {};
		void SendMessage(const Message& message);
	};
	
	class Session::Core
	{
	};
	
	class MySession : public Session
	{
	public:
		MySession(Session::Core core)
			: Session(core)
		{
		}
	};
	
	public Session* Factory(SessionCore& core)
	{
		return new MySession(core);
	}
	
#endif

namespace Marda
{
	namespace Network
	{
		class Session
		{
		public:
			typedef std::shared_ptr<Session> Ptr;
			typedef unsigned int Id;
		
			Session() {}
			virtual ~Session() {}
			
			virtual Id GetId() const = 0;
			
		
		};	// end class Session
		
	}	// end namespace Network
	
}	// end namespace Marda

