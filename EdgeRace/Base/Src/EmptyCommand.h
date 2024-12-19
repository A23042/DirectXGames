// 2024.12.19 S.Matsunaga
// リストの先頭に入れるダミーコマンド
#pragma once

#include "CommandBase.h"

class EmptyCommand : public CommandBase
{
public:
	void Do() override {};
	void Undo() override {};
};