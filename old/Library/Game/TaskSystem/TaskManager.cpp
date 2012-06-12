/*
 *	TaskSystem/TaskManager.cpp
 *
 *	@author	Dila
 */

#include "Game/TaskSystem.h"


/**
 *	�f�X�g���N�^
 */
TaskManager::~TaskManager()
{
	Finalize();
}

/**
 *	����������
 */
bool TaskManager::Initialize()
{
	return true;
}


/**
 *	�I������
 */
void TaskManager::Finalize()
{
	std::list<Task*>::iterator i = taskList.begin();
	while (i != taskList.end()) {
		delete *i;
		i = taskList.erase(i);
	}
}


/**
 *	�w��ID�̃^�X�N���폜����
 *	@param	id	�^�X�NID
 */
bool TaskManager::DeleteTask(int id)
{
	Task* pTask = FindTask(id);
	if (pTask) {
		pTask->deleteFlag = true;
		return true;
	}

	return false;
}


/**
 *	�w��^�C�v�̃^�X�N���폜����
 *	@param	type	�^�X�N�^�C�v
 */
bool TaskManager::DeleteTaskByType(int type)
{
	// ���̂�������܂��B
	return true;
}

/**
 *	�w��ID�̃^�X�N����������
 *	@param	id	�^�X�NID
 *	@return	�^�X�N�B������Ȃ������ꍇ��NULL
 */
Task* TaskManager::FindTask(int id)
{
	std::list<Task*>::iterator i = taskList.begin();
	for (; i != taskList.end(); ++i) {
		if ((*i)->GetId() == id) {
			break;
		}
	}

	if (i != taskList.end()) {
		return *i;
	}
	return NULL;
}

/**
 *	�S�^�X�N�X�V
 *	@param fDelta	�o�ߎ���
 */
void TaskManager::Update(float fDelta)
{
	std::list<Task*>::iterator i;

	// �폜�t���O�����Ă�^�X�N�͏���
	i = taskList.begin();
	while (i != taskList.end()) {
		if ((*i)->deleteFlag) {
			delete *i;
			i = taskList.erase(i);
			continue;
		}
		++i;
	}

	// �S�^�X�NUpdate
	i = taskList.begin();
	std::list<TaskMessage*>::iterator k;
	for (; i != taskList.end(); ++i) {
		if (!(*i)->IsStop()) {
			// Post���b�Z�[�W����
			k = (*i)->msgQueue.begin();
			while (k != (*i)->msgQueue.end()) {
				(*i)->OnMessage(*k);
				delete *k;
				k = (*i)->msgQueue.erase(k);
			}

			// Update
			(*i)->Update(0.0f);	// �Ƃ肠����0
		}
	}
}

/**
 *	�^�X�N���b�Z�[�W���M(SEND)
 *	@param	pMsg	���M���郁�b�Z�[�W
 *	@param	id		���M����^�X�N��ID
 */
void TaskManager::SendTaskMessage(TaskMessage* pMsg, int id)
{
	Task* pTo = FindTask(id);
	if (pTo) {
		pTo->OnMessage(pMsg);
	}
}

