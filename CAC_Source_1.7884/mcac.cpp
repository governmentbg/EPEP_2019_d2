#include "cac.h"

// ----- TXFailure -----------------------------------------------------------
class TXFailure : public TXOwl
{
public:
	TXFailure(uint resId = IDS_FAILURE, HANDLE handle = 0);

	static string Msg(uint resId, HANDLE);

#if defined(BI_NO_COVAR_RET)
	TXBase *Clone();
#else  // BI_NO_COVAR_RET
	TXFailure *Clone();
#endif  // BI_NO_COVAR_RET
	void Throw();

	static void Raise(uint resId = IDS_FAILURE, HANDLE handle = 0);
};

TXFailure::TXFailure(uint resId, HANDLE handle)
:
	TXOwl(MakeMessage(resId, uint(handle)), resId)
{
}

string TXFailure::Msg(uint resId, HANDLE handle)
{
	return MakeMessage(resId, uint(handle));
}

#if defined(BI_NO_COVAR_RET)
TXBase *TXFailure::Clone()
#else  // BI_NO_COVAR_RET
TXFailure *TXFailure::Clone()
#endif  // BI_NO_COVAR_RET
{
	return new TXFailure(*this);
}

void TXFailure::Throw()
{
	THROW(*this);
}

void TXFailure::Raise(uint resId, HANDLE handle)
{
	TXFailure(resId, handle).Throw();
}

// ----- handlers ------------------------------------------------------------
void handle_exception(const char *descript)
{
	if (!ask("%s\nЩе продължите ли работа с програмата?", descript))
		PostQuitMessage(EXIT_FAILURE);
}

bool global_shutdown()
{
	try
	{
		delete TCACDataBase::DefaultDB;
		delete ExtraDataBase;
		TODBCDataBase::Shutdown();
		TBDEDataBase::Shutdown();
	}
	catch (...)  // shoudn't happen (tm)
	{
		return false;
	}

	return true;
}

// ----- machine -------------------------------------------------------------
void failure()
{
	TXFailure::Raise();
}

void out_of_memory()
{
	fatal("Няма достатъчно памет.");
}

constant AppNameName = "САС \"СЪДЕБНО ДЕЛОВОДСТВО\"";
