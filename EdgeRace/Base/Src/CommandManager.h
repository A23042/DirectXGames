// 2024.12.19 S.Matsunaga
// コマンド(操作)を保存、管理するクラス
#pragma once
#include "CommandBase.h"

class CommandManager
{
public:
	CommandManager() {};
	~CommandManager() {};

	/// <summary>
	/// 初期設定
	/// コマンドを使用する際はじめに一回呼ぶ
	/// </summary>
	void SetUp();
	
	/// <summary>
	/// 実行コマンド
	/// 操作した後に呼び出す
	/// </summary>
	/// <param name="command">実行したコマンド</param>
	void Do(shared_ptr<CommandBase> command);

	/// <summary>
	/// commandListに保存されている操作の巻き戻し
	/// </summary>
	void Undo();

	void Redo();
private:
	list<shared_ptr<CommandBase>> commandList = {};
	list<shared_ptr<CommandBase>>::iterator currentItr;
};