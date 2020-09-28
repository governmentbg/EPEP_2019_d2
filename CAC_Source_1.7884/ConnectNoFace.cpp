//{{TConnectNoFace Implementation}}

TConnectNoFace::TConnectNoFace(TWindow* parent, int resourceId, const char *name, TCharAliasFace *face)
:
	TStringFace(parent, resourceId, name, SIZE_OF_CONNECT_NO)
{
	SetValidator(new TConnectNoValidator(face));
}
