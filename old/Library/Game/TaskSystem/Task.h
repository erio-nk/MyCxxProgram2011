/**
 *	@file TaskSystem/Task.h
 *	タスクベースクラス
 *
 *	@author	Dila
 */
/// @addtogroup TaskSystem
/// @{

#pragma once

#include <list>	// for msgQueue

/**
 *	タスクベースクラス
 *	
 *	@see TaskManager
 *	@see TaskMessage
 */
class Task
{
	int id;			// タスクID
	bool deleteFlag;	// 削除フラグ
	bool stopFlag;		// 停止フラグ

	std::list<class TaskMessage*> msgQueue;

	friend class TaskManager;

protected:
	class TaskManager* pOwner;	/// 管理されるタスクマネージャ

public:
	Task() { id = 0; deleteFlag = false; stopFlag = false; pOwner = NULL; }
	virtual ~Task() {}

	/**
	 *	タスクIDを取得する
	 *	@return	タスクID。0は無効なID。
	 */
	int GetId() { return id; }

	/**
	 *	タスクを停止する
	 */
	void Stop() { stopFlag = true; }

	/**
	 *	タスクを開始する
	 */
	void Start() { stopFlag = false; }

	/// タスクが止まっているかどうかを得る
	bool IsStop() { return stopFlag; }


	///	タスクタイプを取得する
	virtual int GetType() { return 0; }

	virtual bool Initialize() { return true; }

	virtual void Finalize() {}

	/**
	 *	タスクを更新する
	 *	@param fDelta	前回の呼び出しからの経過時間
	 */
	virtual void Update(float fDelta) {};

	/**
	 *	タスクメッセージハンドラ
	 *	@param pMsg	受信したタスクメッセージ
	 */
	virtual void OnMessage(class TaskMessage* pMsg) {}
};

/// @}

