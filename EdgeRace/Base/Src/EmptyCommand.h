// 2024.12.19 S.Matsunaga
// ���X�g�̐擪�ɓ����_�~�[�R�}���h
#pragma once

#include "CommandBase.h"

class EmptyCommand : public CommandBase
{
public:
	void Do() override {};
	void Undo() override {};
};