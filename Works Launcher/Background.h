// include
#pragma once
#include "Main.h"

// �o�b�N�O���E���h����

class Background
{
private:

	Array<std::pair<Line, Stopwatch>>m_effect;
	Stopwatch m_effectStopwatch;
	Vec2 lightIconPos;

public:

	// ������
	void init();

	// �X�V
	bool update(AppData& data);

	// �`��
	void draw(AppData& data);

	// �_�[�N���[�h�ƃ��C�g���[�h��؂�ւ���
	void setDrawMode(AppData& data);
};
