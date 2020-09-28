static HRESULT STDMETHODCALLTYPE Dispatch_QueryInterface(IDispatch *This, REFIID riid, void **ppvObject)
{
	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDispatch))
		*ppvObject = (IDispatch *) This;
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

static ULONG STDMETHODCALLTYPE Dispatch_AddRef(IDispatch *)
{
	return 1;
}

static ULONG STDMETHODCALLTYPE Dispatch_Release(IDispatch *)
{
	return 1;
}

static HRESULT STDMETHODCALLTYPE Dispatch_GetTypeInfoCount(IDispatch *, unsigned int *)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Dispatch_GetTypeInfo(IDispatch *, unsigned int, LCID, ITypeInfo **)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Dispatch_GetIDsOfNames(IDispatch *, REFIID, OLECHAR **, unsigned int, LCID, DISPID *)
{
	return E_NOTIMPL;
}

#define VT_REFTYPE
static HRESULT STDMETHODCALLTYPE Dispatch_Invoke(IDispatch *This, DISPID dispidMember, REFIID, LCID, WORD,
	DISPPARAMS *pDispParams, VARIANT *, EXCEPINFO *, UINT *)
{
	switch (dispidMember)
	{
		case DISPID_DOWNLOADBEGIN :
		case DISPID_DOCUMENTCOMPLETE :
		case DISPID_PROGRESSCHANGE : break;
		case DISPID_BEFORENAVIGATE2 :
		{
			if (pDispParams->cArgs >= 6 && pDispParams->rgvarg[5].vt == (VT_BYREF | VT_VARIANT) &&
				pDispParams->rgvarg[5].pvarVal->vt == VT_BSTR)
			{
				char s[0x200];

				if (WideCharToMultiByte(1251, 0, pDispParams->rgvarg[5].pvarVal->bstrVal, -1, s, sizeof s, NULL,
					NULL))
				{
					return ClientSitePtr(dispatch)->pHtmlDialog->pCallBack(s);
				}
			}
		}
		case DISPID_NAVIGATECOMPLETE2 : break;
		case DISPID_NEWWINDOW2 :
		{
			if (pDispParams->cArgs == 2)
			{
				if (pDispParams->rgvarg[0].vt == (VT_BYREF | VT_BOOL))
					*(pDispParams->rgvarg[0].pbool) = VARIANT_TRUE;
				if (pDispParams->rgvarg[1].vt == (VT_BYREF | VT_DISPATCH))
					*(pDispParams->rgvarg[1].ppdispVal) = This;
			}
			break;
		}
		case DISPID_NEWWINDOW3 :
		case DISPID_STATUSTEXTCHANGE :
		case DISPID_TITLECHANGE :
		case DISPID_CLIENTTOHOSTWINDOW :
		case DISPID_COMMANDSTATECHANGE :
		case DISPID_DOWNLOADCOMPLETE :
		case DISPID_FILEDOWNLOAD :
		case DISPID_NAVIGATEERROR :
		case DISPID_ONTOOLBAR :
		case DISPID_ONVISIBLE :
		case DISPID_PRIVACYIMPACTEDSTATECHANGE :
		case DISPID_PROPERTYCHANGE :
		case DISPID_SETSECURELOCKICON :
		case DISPID_TITLEICONCHANGE :
		case DISPID_WINDOWACTIVATE :
		case DISPID_WINDOWCLOSING :
		case DISPID_WINDOWMOVE :
		case DISPID_WINDOWRESIZE :
		case DISPID_WINDOWSETHEIGHT :
		case DISPID_WINDOWSETLEFT :
		case DISPID_WINDOWSETRESIZABLE :
		case DISPID_WINDOWSETTOP :
		case DISPID_WINDOWSETWIDTH : break;
		default : return DISP_E_MEMBERNOTFOUND;
	}

	return S_OK;
}

IDispatchVtbl DispatchVtbl =
{
	Dispatch_QueryInterface,
	Dispatch_AddRef,
	Dispatch_Release,
	Dispatch_GetTypeInfoCount,
	Dispatch_GetTypeInfo,
	Dispatch_GetIDsOfNames,
	Dispatch_Invoke
};
