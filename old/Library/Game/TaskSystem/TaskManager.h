/**
 *	@file TaskSystem/TaskManager.h
 *
 *	�^�X�N�}�l�[�W���N���X
 *
 *	@author	Dila
 */
/// @addtogroup TaskSystem
/// @{

#pragma once

#include <list>

/**
 *	�^�X�N�}�l�[�W���N���X
 *	@see Task
 *	@see TaskMessage
 */
class TaskManager
{
	int idGen;	// �^�X�NID������
	std::list<Task*> taskList;	// �^�X�N���X�g

public:
	TaskManager() { idGen = 0; }
	~TaskManager();

	bool Initialize();
	void Finalize();

	/**
	 *	�^�X�N����
	 *	@tparam	TaskClass	��������^�X�N�N���X
	 *	@return	�^�X�NID (0�͎��s)
	 */
	template <class TaskClass>
	int CreateTask(){
		Task* p = (Task*)new TaskClass(); 
			// new/delete�̃X�R�[�v��TaskManager�N���X�X�R�[�v�Ɏ��߂�
		if (p == NULL) return 0;
		p->id = ++idGen;
		p->pOwner = this;
		taskList.push_back(p);
		if (!p->Initialize()) {
			return 0;	// ���X�g����͂ǂ����܂��傤���˂�
		}
		return p->id;
	}

	// �^�X�N�j��
	bool DeleteTask(int id);
	bool DeleteTaskByType(int type);

	// �^�X�N�擾
	Task* FindTask(int id);

	//FindTaskByType(int type);

	// �S�^�X�N�X�V
	void Update(float fDelta);

	// �^�X�N���b�Z�[�W���M(SEND)
	void SendTaskMessage(class TaskMessage* pMsg, int id);
	
	/**
	 *	�^�X�N���b�Z�[�W���M(POST)
	 *
	 *	@tparam	TaskMessageClass	TaskMessage�N���X�̃T�u�N���X
	 *	@param	pMsg	���M���郁�b�Z�[�W
	 *	@param	id		���M����^�X�N��ID
	 */
	template <class TaskMessageClass>
	void PostTaskMessage(TaskMessageClass* pMsg, int id)
	{
		Task* pTo = FindTask(id);
		if (pTo) pTo->msgQueue.push_back(new TaskMessageClass(*pMsg));
			// new/delete�̃X�R�[�v��TaskManager�N���X�X�R�[�v�Ɏ��߂�
	}


	//void SendTaskMessageToType(class TaskMessage* pMsg, int type);
	//void PostTaskMessageToType(class TaskMessage* pMsg, int type);
};

/// @}
