#include "cac.h"

TProcess::TProcess(TWindow *tParent, const char *what, const char *tWait)
	: fd(NULL), fdwhat(what), dccnt(0), parent(tParent)
{
	wait = tWait ? new TWaitWindow(parent, tWait) : NULL;
}

TProcess::~TProcess()
{
	delete wait;
}

void TProcess::vdprintf(const char *format, va_list ap)
{
	if (!fd)
		fd = tf.OpenTemp("log");
	else if (fd == stdout)
		fd = tf.ReopenTemp();

	if (fd)
		vfprintf(fd, format, ap);

	dccnt++;
}

void TProcess::dprintf(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vdprintf(format, ap);
	va_end(ap);
}

void TProcess::Display()
{
	if (fd)
	{
		tf.Show(wait);
		fd = stdout;
	}
}

void TProcess::Debug()
{
	delete wait;
	wait = NULL;

	if (dccnt)
	{
		Display();
		error("%d грешк%c при генерирането на %s.%s", dccnt, dccnt == 1 ? 'а' : 'и', fdwhat,
			fd ? " ѕрегледайте журнала." : "");
	}
}
