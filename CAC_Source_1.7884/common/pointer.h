// ----- TAutoPtr ------------------------------------------------------------
class TAutoPtr
{
public:
	TAutoPtr(void *tPtr) : ptr(tPtr) { }

protected:
	void *const ptr;
};

// ----- TVoidPtr ------------------------------------------------------------
class TVoidPtr : public TAutoPtr
{
public:
	TVoidPtr(void *ptr) : TAutoPtr(ptr) { }
	~TVoidPtr();
};

// ----- TVirtPtr ------------------------------------------------------------
class TVirtPtr : public TAutoPtr
{
public:
	TVirtPtr(TVObject *object) : TAutoPtr(object) { }
	~TVirtPtr();
};

// ----- TCharPtr ------------------------------------------------------------
class TCharPtr : public TAutoPtr
{
public:
	TCharPtr(char *string) : TAutoPtr(string) { }
	~TCharPtr();
};

// ----- TBoolPtr ------------------------------------------------------------
class TBoolPtr : public TAutoPtr
{
public:
	TBoolPtr(bool *array) : TAutoPtr(array) { }
	~TBoolPtr();
};
