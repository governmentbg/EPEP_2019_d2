TTypeListFace::TTypeListFace(TWindow *parent, int resourceId, const char *name, const char *tTypes, bool tWrite)
	: TCharAutoListFace(parent, resourceId, name, Type, types, SIZE_OF_TYPES), write(tWrite)
{
	build_ctypes(types, tTypes ? tTypes : TYPE_LAWSUITS, write);
}

const char *TTypeListFace::S()
{
	return write ? TCharListFace::S() : TCharAutoListFace::S();
}
