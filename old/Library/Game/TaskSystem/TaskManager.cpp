/*
 *	TaskSystem/TaskManager.cpp
 *
 *	@author	Dila
 */

#include "Game/TaskSystem.h"


/**
 *	デストラクタ
 */
TaskManager::~TaskManager()
{
	Finalize();
}

/**
 *	初期化処理
 */
bool TaskManager::Initialize()
{
	return true;
}


/**
 *	終了処理
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
 *	指定IDのタスクを削除する
 *	@param	id	タスクID
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
 *	指定タイプのタスクを削除する
 *	@param	type	タスクタイプ
 */
bool TaskManager::DeleteTaskByType(int type)
{
	// そのうちつくります。
	return true;
}

/**
 *	指定IDのタスクを検索する
 *	@param	id	タスクID
 *	@return	タスク。見つからなかった場合はNULL
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
 *	全タスク更新
 *	@param fDelta	経過時間
 */
void TaskManager::Update(float fDelta)
{
	std::list<Task*>::iterator i;

	// 削除フラグ立ってるタスクは消す
	i = taskList.begin();
	while (i != taskList.end()) {
		if ((*i)->deleteFlag) {
			delete *i;
			i = taskList.erase(i);
			continue;
		}
		++i;
	}

	// 全タスクUpdate
	i = taskList.begin();
	std::list<TaskMessage*>::iterator k;
	for (; i != taskList.end(); ++i) {
		if (!(*i)->IsStop()) {
			// Postメッセージ処理
			k = (*i)->msgQueue.begin();
			while (k != (*i)->msgQueue.end()) {
				(*i)->OnMessage(*k);
				delete *k;
				k = (*i)->msgQueue.erase(k);
			}

			// Update
			(*i)->Update(0.0f);	// とりあえず0
		}
	}
}

/**
 *	タスクメッセージ送信(SEND)
 *	@param	pMsg	送信するメッセージ
 *	@param	id		送信するタスクのID
 */
void TaskManager::SendTaskMessage(TaskMessage* pMsg, int id)
{
	Task* pTo = FindTask(id);
	if (pTo) {
		pTo->OnMessage(pMsg);
	}
}

