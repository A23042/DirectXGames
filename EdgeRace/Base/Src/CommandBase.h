// 2024.12.19 S.Matsunaga
// オブジェクトの操作をコマンドとして保存するためのベースクラス
#pragma once
#include <iostream>
#include <list>
#include <iterator>

class CommandBase
{
public:
	virtual void Do() = 0;
	virtual void Undo() = 0;
};