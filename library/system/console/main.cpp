#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "console.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	console console;
	{
		DWORD mode = console.get_mode(STD_INPUT_HANDLE);
		mode &= ~(ENABLE_PROCESSED_INPUT | /*ENABLE_LINE_INPUT |*/
			/*ENABLE_ECHO_INPUT |*/ ENABLE_WINDOW_INPUT |
			ENABLE_MOUSE_INPUT | /*ENABLE_INSERT_MODE |*/
			ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS |
			ENABLE_AUTO_POSITION | ENABLE_VIRTUAL_TERMINAL_INPUT);
		console.set_mode(STD_INPUT_HANDLE, mode);
	}
	{
		DWORD mode = console.get_mode(STD_OUTPUT_HANDLE);
		mode &= ~(/*ENABLE_PROCESSED_OUTPUT |*/  ENABLE_WRAP_AT_EOL_OUTPUT |
			ENABLE_VIRTUAL_TERMINAL_PROCESSING /*| ENABLE_LVB_GRID_WORLDWIDE*/);
		/*mode |= DISABLE_NEWLINE_AUTO_RETURN;*/
		console.set_mode(STD_OUTPUT_HANDLE, mode);
	}
	{
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 1;
		info.bVisible = FALSE;
		console.set_cursor(info);
	}

	return 0;
}