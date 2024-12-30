// 2024.12.19 S.Matsunaga
// コマンド(操作)を保存、管理するクラス
// CommandBaseを継承したコマンドクラスをリスト配列に格納
// イテレータでリストの参照をしてUndoとRedoで参照コマンドの実行
#pragma once
#include "CommandBase.h"
#include "Macro.h"

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

	/// <summary>
	/// commandListに保存されている操作の次の実行
	/// </summary>
	void Redo();
private:
	list<shared_ptr<CommandBase>> commandList = {};		// 実行されたコマンドの格納
	list<shared_ptr<CommandBase>>::iterator currentItr;	// 格納されたコマンドの参照場所
};