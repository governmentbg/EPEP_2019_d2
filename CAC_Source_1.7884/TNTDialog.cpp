#include "cac.h"

//{{TNTDialog Implementation}}

TNTDialog::TNTDialog(TWindow* parent, int resId)
:
	TDialog(parent, resId), alreadyExcepted(false), exceptTerminate(false)
{
}

TNTDialog::~TNTDialog()
{
}

void TNTDialog::HandleException(const char *descript)
{
	if (!alreadyExcepted)
	{
		if (exceptTerminate)
		{
			error("%s\nРаботата с програмата ще бъде прекратена.", descript);
			global_shutdown();
			ExitProcess(EXIT_FAILURE);
		}

		handle_exception(descript);
		alreadyExcepted = true;
		Destroy(IDCANCEL);

		if (Parent)
			Parent->SetActiveWindow();
	}
}

bool TNTDialog::DialogFunction(uint message, TParam1 wParam, TParam2 lParam)
{
	try
	{
		return TDialog::DialogFunction(message, wParam, lParam);
	}
	catch (xmsg& x)
	{
		HandleException(x.why().c_str());
	}
	catch (...)
	{
		HandleException(UNHANDLED_EXCEPT);
	}

	return true;
}

TResult TNTDialog::EvCommand(uint id, THandle hWndCtl, uint notifyCode)
{
	try
	{
		return TDialog::EvCommand(id, hWndCtl, notifyCode);
	}
	catch (xmsg& x)
	{
		HandleException(x.why().c_str());
	}
	catch (...)
	{
		HandleException(UNHANDLED_EXCEPT);
	}

	return 0;
}
