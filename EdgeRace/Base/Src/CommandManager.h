// 2024.12.19 S.Matsunaga
// �R�}���h(����)��ۑ��A�Ǘ�����N���X
// CommandBase���p�������R�}���h�N���X�����X�g�z��Ɋi�[
// �C�e���[�^�Ń��X�g�̎Q�Ƃ�����Undo��Redo�ŎQ�ƃR�}���h�̎��s
#pragma once
#include "CommandBase.h"
#include "Macro.h"

class CommandManager
{
public:
	CommandManager() {};
	~CommandManager() {};

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
	void Do(shared_ptr<CommandBase> command);

	/// <summary>
	/// commandList�ɕۑ�����Ă��鑀��̊����߂�
	/// </summary>
	void Undo();

	/// <summary>
	/// commandList�ɕۑ�����Ă��鑀��̎��̎��s
	/// </summary>
	void Redo();
private:
	list<shared_ptr<CommandBase>> commandList = {};		// ���s���ꂽ�R�}���h�̊i�[
	list<shared_ptr<CommandBase>>::iterator currentItr;	// �i�[���ꂽ�R�}���h�̎Q�Əꏊ
};