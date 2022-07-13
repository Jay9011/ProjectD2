#pragma once
class TweakBar
{
public:
	static TweakBar* Get();
	static void Create();
	static void Delete();

	void Start();
	void InputProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void Render();

	void GetDraw(bool* isDraw)
	{
		*isDraw = this->isDraw;
	}

	void SetDraw(bool isDraw)
	{
		this->isDraw = isDraw;

		if(isDraw == true)
			TwDefine("TweakBar visible=true");
		else
			TwDefine("TweakBar visible=false");
	}

	TwBar* GetBar()
	{
		return bar;
	}

private:
	TweakBar();
	~TweakBar();

	static TweakBar* instance;

	bool isDraw;
	TwBar* bar;
};

