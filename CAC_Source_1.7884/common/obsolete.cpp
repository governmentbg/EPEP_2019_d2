#if FIREBIRD_ODBC_CURSOR_CACHE
	virtual ~TFireBirdDataBase();
#endif  // FIREBIRD_ODBC_CURSOR_CACHE

#if FIREBIRD_ODBC_CURSOR_CACHE
TFireBirdDataBase::~TFireBirdDataBase()
{
	for (size_t n = 0; n < used_handles; n++)
		SQLFreeHandle(SQL_HANDLE_STMT, handles[n]);
}
#endif  // FIREBIRD_ODBC_CURSOR_CACHE
