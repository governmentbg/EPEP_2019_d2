extern constant AppNameName;
#define MessageBoxTitle AppNameName
#define out_of_memory() { fatal("���� ���������� �����."); }
#ifdef __GNUC__
void failure() __attribute__ ((noreturn));
#endif
void failure();
