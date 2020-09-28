static HRESULT STDMETHODCALLTYPE Storage_QueryInterface(IStorage *, REFIID, void **pp)
{
	*pp = NULL;
	return E_NOTIMPL;
}

static ULONG STDMETHODCALLTYPE Storage_AddRef(IStorage *)
{
	return 1;
}

static ULONG STDMETHODCALLTYPE Storage_Release(IStorage *)
{
	return 1;
}

static HRESULT STDMETHODCALLTYPE Storage_CreateStream(IStorage *, const OLECHAR *, DWORD, DWORD, DWORD, IStream **)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_OpenStream(IStorage *, const OLECHAR *, void *, DWORD, DWORD, IStream **)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_CreateStorage(IStorage*, const OLECHAR *, DWORD, DWORD, DWORD, IStorage **)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_OpenStorage(IStorage *, const OLECHAR *, IStorage *, DWORD, SNB, DWORD, IStorage **)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_CopyTo(IStorage *, DWORD, const IID *, SNB, IStorage *)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_MoveElementTo(IStorage *, const OLECHAR *, IStorage *, const OLECHAR *, DWORD)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_Commit(IStorage *, DWORD)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_Revert(IStorage *)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_EnumElements(IStorage *, DWORD, void *, DWORD, IEnumSTATSTG **)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_DestroyElement(IStorage *, const OLECHAR *)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_RenameElement(IStorage *, const OLECHAR *, const OLECHAR *)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_SetElementTimes(IStorage *, const OLECHAR *, const FILETIME *, const FILETIME *,
	const FILETIME *)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_SetClass(IStorage *, REFCLSID)
{
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE Storage_SetStateBits(IStorage *, DWORD, DWORD)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_Stat(IStorage *, STATSTG *, DWORD)
{
	return E_NOTIMPL;
}

IStorageVtbl StorageVtbl =
{
	Storage_QueryInterface,
	Storage_AddRef,
	Storage_Release,
	Storage_CreateStream,
	Storage_OpenStream,
	Storage_CreateStorage,
	Storage_OpenStorage,
	Storage_CopyTo,
	Storage_MoveElementTo,
	Storage_Commit,
	Storage_Revert,
	Storage_EnumElements,
	Storage_DestroyElement,
	Storage_RenameElement,
	Storage_SetElementTimes,
	Storage_SetClass,
	Storage_SetStateBits,
	Storage_Stat
};
