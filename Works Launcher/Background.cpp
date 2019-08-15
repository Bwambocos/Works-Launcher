// include
#include "Background.h"

// �o�b�N�O���E���h����

// ������
void Background::init()
{
	TextureAsset::Register(U"lightIcon", Icon(0xf0eb, 38));
	AudioAsset::Register(U"cursorAudio", U"data//cursorAudio.mp3");
	AudioAsset::Register(U"dog", U"data//dog.mp3");
	FontAsset::Register(U"timerFont", 16, U"data//fontR.ttc");
	FontAsset::Register(U"messageFont", 72, U"data//fontR.ttc", FontStyle::Bold);
	m_effectStopwatch.start();
	lightIconPos = Vec2(Scene::Width() - TextureAsset(U"lightIcon").width() - 10, 10);
	const INIData configINI(U"data//config.ini");
	demoFlag = configINI.get<bool>(U"Demo", U"flag");
	if (demoFlag) demoTimer = Timer(configINI.get<double>(U"Demo", U"limit_time"), true);
}

// �X�V
bool Background::update(AppData& data)
{
	// �`�惂�[�h�ؑ�
	if (TextureAsset(U"lightIcon").region(lightIconPos).leftClicked())
	{
		AudioAsset(U"cursorAudio").stop();
		AudioAsset(U"cursorAudio").play();
		setDrawMode(data);
	}

	// �w�i�G�t�F�N�g
	if (m_effectStopwatch.elapsed() > 50ms)
	{
		const Vec2 pos = RandomVec2(Scene::Width(), Scene::Height());
		const Vec2 direction = Circular(Scene::Width() + Scene::Height(), Random(360_deg));
		m_effect << std::pair<Line, Stopwatch>(Line(pos - direction, pos + direction), Stopwatch(true));
		m_effectStopwatch.restart();
	}

	// �^�C�}�[
	if (demoFlag)
	{
		if (KeyShift.pressed() && KeyAlt.pressed() && KeyControl.pressed() && KeyR.pressed())
		{
			Window::SetFullscreen(false);
			Window::Restore();
			demoTimer.restart();
			return true;
		}
		if (demoTimer.reachedZero())
		{
			Window::SetFullscreen(true);
			FontAsset(U"messageFont")(U"���Ԑ����ɒB���܂����D\n��ϐ\���󂠂�܂��񂪁C�������肢���܂��D").drawAt(Scene::Center(), data.stringColor);
			return false;
		}
	}


	// �B���v�f
	if (KeyShift.pressed() && KeyD.pressed() && KeyO.pressed() && KeyG.pressed()) AudioAsset(U"dog").play();

	return true;
}

// �`��
void Background::draw(AppData& data)
{
	// �`�惂�[�h�ؑ�
	if (!(demoFlag && demoTimer.reachedZero())) TextureAsset(U"lightIcon").draw(lightIconPos, (TextureAsset(U"lightIcon").region(lightIconPos).mouseOver() ? data.schemeColor5 : data.schemeColor4));

	// �w�i�G�t�F�N�g
	for (auto line : m_effect)
	{
		if (line.second.sF() < 2.) line.first.draw(2, AlphaF((1.0 - Abs(line.second.sF() - 1.0)) * 0.3));
	}

	// �^�C�}�[
	if (demoFlag) FontAsset(U"timerFont")(U"�c�莞�ԁF" + demoTimer.format(U"mm:ss.xx")).draw(5, 5, data.stringColor);
}

// �_�[�N���[�h�ƃ��C�g���[�h��؂�ւ���
void Background::setDrawMode(AppData& data)
{
	(++data.drawMode) %= 2;
	if (data.drawMode == 0)
	{
		data.schemeColor1 = Color(22, 22, 22);
		data.schemeColor2 = Color(66, 66, 66);
		data.schemeColor3 = Color(110, 110, 110);
		data.schemeColor4 = Color(154, 154, 154);
		data.schemeColor5 = Color(198, 198, 198);
		data.stringColor = Palette::White;
	}
	else
	{
		data.schemeColor1 = Color(64, 138, 241);
		data.schemeColor2 = Color(90, 162, 246);
		data.schemeColor3 = Color(120, 182, 250);
		data.schemeColor4 = Color(136, 203, 250);
		data.schemeColor5 = Color(168, 215, 250);
		data.stringColor = Palette::Black;
	}
	Scene::SetBackground(data.schemeColor1);
	manager.setFadeColor(data.schemeColor1);
}
