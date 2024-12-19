// 2024.12.19 S.Matsunaga
// �R�}���h(����)��ۑ��A�Ǘ�����N���X
#pragma once
#include "CommandBase.h"

class CommandManager
{
public:
	CommandManager() {};

	/// <summary>
	/// �����ݒ�
	/// �R�}���h���g�p����ۂ͂��߂Ɉ��Ă�
	/// </summary>
	void SetUp();
	
	/// <summary>
	/// ���s�R�}���h
	/// ���삵����ɌĂяo��
	/// </summary>
	/// <param name="command">���s�����R�}���h</param>
	void Do(std::shared_ptr<CommandBase> command);

	/// <summary>
	/// commandList�ɕۑ�����Ă��鑀��̊����߂�
	/// </summary>
	void Undo();

	void Redo();
private:
	std::list<std::shared_ptr<CommandBase>> commandList = {};
	std::list<std::shared_ptr<CommandBase>>::iterator currentItr;
};