/**
 *	@file TaskSystem/Task.h
 *	�^�X�N�x�[�X�N���X
 *
 *	@author	Dila
 */
/// @addtogroup TaskSystem
/// @{

#pragma once

#include <list>	// for msgQueue

/**
 *	�^�X�N�x�[�X�N���X
 *	
 *	@see TaskManager
 *	@see TaskMessage
 */
class Task
{
	int id;			// �^�X�NID
	bool deleteFlag;	// �폜�t���O
	bool stopFlag;		// ��~�t���O

	std::list<class TaskMessage*> msgQueue;

	friend class TaskManager;

protected:
	class TaskManager* pOwner;	/// �Ǘ������^�X�N�}�l�[�W��

public:
	Task() { id = 0; deleteFlag = false; stopFlag = false; pOwner = NULL; }
	virtual ~Task() {}

	/**
	 *	�^�X�NID���擾����
	 *	@return	�^�X�NID�B0�͖�����ID�B
	 */
	int GetId() { return id; }

	/**
	 *	�^�X�N���~����
	 */
	void Stop() { stopFlag = true; }

	/**
	 *	�^�X�N���J�n����
	 */
	void Start() { stopFlag = false; }

	/// �^�X�N���~�܂��Ă��邩�ǂ����𓾂�
	bool IsStop() { return stopFlag; }


	///	�^�X�N�^�C�v���擾����
	virtual int GetType() { return 0; }

	virtual bool Initialize() { return true; }

	virtual void Finalize() {}

	/**
	 *	�^�X�N���X�V����
	 *	@param fDelta	�O��̌Ăяo������̌o�ߎ���
	 */
	virtual void Update(float fDelta) {};

	/**
	 *	�^�X�N���b�Z�[�W�n���h��
	 *	@param pMsg	��M�����^�X�N���b�Z�[�W
	 */
	virtual void OnMessage(class TaskMessage* pMsg) {}
};

/// @}

