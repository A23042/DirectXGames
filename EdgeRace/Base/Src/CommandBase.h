// 2024.12.19 S.Matsunaga
// �I�u�W�F�N�g�̑�����R�}���h�Ƃ��ĕۑ����邽�߂̃x�[�X�N���X
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