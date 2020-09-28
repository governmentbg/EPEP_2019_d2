enum { COMMAND_LINE_SIZE = 32768 };
void command_record(char *command, const char *text);
void command_record2(char *command, const char *text1, const char *text2);
bool command_launch(char *command, PROCESS_INFORMATION &procInfo);
int command_waitfor(HANDLE hProcess);
