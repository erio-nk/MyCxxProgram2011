/**
 *	@file TaskSystem/TaskManager.h
 *
 *	タスクマネージャクラス
 *
 *	@author	Dila
 */
/// @addtogroup TaskSystem
/// @{

#pragma once

#include <list>

/**
 *	タスクマネージャクラス
 *	@see Task
 *	@see TaskMessage
 */
class TaskManager
{
	int idGen;	// タスクID生成器
	std::list<Task*> taskList;	// タスクリスト

public:
	TaskManager() { idGen = 0; }
	~TaskManager();

	bool Initialize();
	void Finalize();

	/**
	 *	タスク生成
	 *	@tparam	TaskClass	生成するタスククラス
	 *	@return	タスクID (0は失敗)
	 */
	template <class TaskClass>
	int CreateTask(){
		Task* p = (Task*)new TaskClass(); 
			// new/deleteのスコープをTaskManagerクラススコープに収める
		if (p == NULL) return 0;
		p->id = ++idGen;
		p->pOwner = this;
		taskList.push_back(p);
		if (!p->Initialize()) {
			return 0;	// リストからはどうしましょうかねぇ
		}
		return p->id;
	}

	// タスク破棄
	bool DeleteTask(int id);
	bool DeleteTaskByType(int type);

	// タスク取得
	Task* FindTask(int id);

	//FindTaskByType(int type);

	// 全タスク更新
	void Update(float fDelta);

	// タスクメッセージ送信(SEND)
	void SendTaskMessage(class TaskMessage* pMsg, int id);
	
	/**
	 *	タスクメッセージ送信(POST)
	 *
	 *	@tparam	TaskMessageClass	TaskMessageクラスのサブクラス
	 *	@param	pMsg	送信するメッセージ
	 *	@param	id		送信するタスクのID
	 */
	template <class TaskMessageClass>
	void PostTaskMessage(TaskMessageClass* pMsg, int id)
	{
		Task* pTo = FindTask(id);
		if (pTo) pTo->msgQueue.push_back(new TaskMessageClass(*pMsg));
			// new/deleteのスコープをTaskManagerクラススコープに収める
	}


	//void SendTaskMessageToType(class TaskMessage* pMsg, int type);
	//void PostTaskMessageToType(class TaskMessage* pMsg, int type);
};

/// @}
