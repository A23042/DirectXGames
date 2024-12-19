#include "CommandManager.h"
#include "EmptyCommand.h"

void CommandManager::SetUp()
{
	commandList.emplace_back(std::make_shared<EmptyCommand>());	// �擪�Ƀ_�~�[��}��
	currentItr = commandList.begin();	// �擪�Ǝw��
}

void CommandManager::Do(std::shared_ptr<CommandBase> command)
{
	// ���X�g�̒��g���L���
	//if(commandList.size() != 0)
	{
		// currentItr��commandList�̍Ō�̗v�f���w���Ă��Ȃ����
		if (currentItr != std::prev(commandList.end()))
		{
			// currentItr�̎��̗v�f����Ō�܂ŏ���
			commandList.erase(std::next(currentItr), commandList.end());
		}
	}
	// �R�}���h�̎��s
	command->Do();
	commandList.emplace_back(command);	// ���X�g�ɒǉ�
	currentItr = std::prev(commandList.end());	// �Q�Əꏊ�̍X�V(�Ō��)
}

void CommandManager::Undo()
{
	// �Q�Əꏊ�����X�g�̐擪�łȂ����(�����߂����삪���邩)
	if (currentItr != commandList.begin())
	{
		// �Q�Əꏊ��Undo�����s����
		(*currentItr)->Undo();
		--currentItr;	// �Q�Əꏊ�̍X�V 
	}
}

void CommandManager::Redo()
{
	// �Q�Əꏊ���Ō���łȂ����(�i�߂鑀�삪���邩)
	if (currentItr != std::prev(commandList.end()))
	{
		// �Q�Əꏊ��i�߂ăR�}���h�̎��s
		++currentItr;
		(*currentItr)->Do();
	}
}
