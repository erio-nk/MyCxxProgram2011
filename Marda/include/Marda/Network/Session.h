/*!
	@file	Network/Session.h
	
	@author	E.Nakayama
*/
#pragma once
	
#include "Marda/Types.h"

#if 0	
	�Z�b�V���������[�U���p�������N���X�ɂ�����
		�A�v���P�[�V�����ŗL�̃��[�U��������������悤�ɂ�����
	�R���X�g���N�^�ŃZ�b�V�����̊e�֐��𗘗p�o����l�ɂ�����
	boost�̎����͉B������
	
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

