#include "CommandManager.h"
#include "EmptyCommand.h"

void CommandManager::SetUp()
{
	commandList.emplace_back(make_shared<EmptyCommand>());	// 先頭にダミーを挿入
	currentItr = commandList.begin();	// 先頭と指す
}

void CommandManager::Do(shared_ptr<CommandBase> command)
{
	// currentItrがcommandListの最後の要素を指していなければ
	if (currentItr != prev(commandList.end()))
	{
		// currentItrの次の要素から最後まで消す
		commandList.erase(next(currentItr), commandList.end());
	}
	// コマンドの実行
	command->Do();
	commandList.emplace_back(command);	// リストに追加
	currentItr = prev(commandList.end());	// 参照場所の更新(最後尾)
}

void CommandManager::Undo()
{
	// 参照場所がリストの先頭でなければ(巻き戻す操作があるか)
	if (currentItr != commandList.begin())
	{
		// 参照場所のUndoを実行する
		(*currentItr)->Undo();
		--currentItr;	// 参照場所の更新 
	}
}

void CommandManager::Redo()
{
	// 参照場所が最後尾でなければ(進める操作があるか)
	if (currentItr != prev(commandList.end()))
	{
		// 参照場所を進めてコマンドの実行
		++currentItr;
		(*currentItr)->Do();
	}
}

void CommandManager::ReSet()
{
	currentItr = commandList.begin();
	commandList.erase(next(currentItr), commandList.end());
}
