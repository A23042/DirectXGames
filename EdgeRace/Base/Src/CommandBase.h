// 2024.12.19 S.Matsunaga
// オブジェクトの操作をコマンドとして保存するためのベースクラス
#pragma once
#include <iostream>
#include <list>
#include <iterator>

using namespace std;

class CommandBase
{
public:
	virtual ~CommandBase() = default;
	virtual void Do() = 0;
	virtual void Undo() = 0;
};