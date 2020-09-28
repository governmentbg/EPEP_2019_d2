void IHTMLDocument2_Open(IHTMLDocument2 *iDocument, BSTR url)
{
	VARIANT ve1, ve2, ve3;

	ve1.vt = VT_EMPTY;
	ve2.vt = VT_EMPTY;
	ve3.vt = VT_EMPTY;
	IHTMLDocument2_open(iDocument, url, ve1, ve2, ve3, NULL);
}

BOOL IHTMLDocument2_Write(IHTMLDocument2 *iDocument, BSTR bstr)
{
	SAFEARRAYBOUND rgsabound[1] = { { 1, 0 } } ;
	SAFEARRAY *sfArray = SafeArrayCreate(VT_VARIANT, 1, rgsabound);
	LONG rgIndices[1] = { 0 };
	VARIANT vSTR;

	if (sfArray)
	{
		vSTR.vt = VT_BSTR;
		vSTR.bstrVal = bstr;

		SafeArrayPutElement(sfArray, rgIndices, &vSTR);
		IHTMLDocument2_write(iDocument, sfArray) == S_OK;
		SafeArrayDestroy(sfArray);
		VariantClear(&vSTR);
	}

	return sfArray != NULL;
}
