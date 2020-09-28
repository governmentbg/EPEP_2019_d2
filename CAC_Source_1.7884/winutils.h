void window_show_functionals(TWindow *parent);
void window_hide_functionals(TWindow *parent);
void window_show_permanents(UINT mainSW, HWND active);
UINT window_hide_permanents();
void window_move_upright_adjusted(TWindow *window);
bool window_handle_color(TNotify far& info, uint start, uint final);

// ----- TWindowHideParent ----------------------------------------------------
class TWindowHideParent
{
public:
	TWindowHideParent(TWindow *tParent);
	~TWindowHideParent();

protected:
	TWindow *parent;
};

// ----- TWindowHideAlls ------------------------------------------------------
class TWindowHideAlls
{
public:
	TWindowHideAlls(TWindow *tParent);
	~TWindowHideAlls();

protected:
	TWindow *parent;
	UINT mainSW;
};
