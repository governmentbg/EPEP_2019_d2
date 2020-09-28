#if TESTVER
void test_rtl();
void testfunc(TWindow *parent);
void write_all_bytes(const char *filename, const char *data, size_t size);
#else  // TESTVER
#define test_rtl()
#define testfunc(parent)
#define write_all_bytes(filename, data, size)
#endif  // TESTVER
