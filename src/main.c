#include "softdraw/softdraw.h"


void toU64Str(bool grid[8][8])
{
	char str[sizeof(
		"0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000"
		)] = {0};

	for (int y = 0; y < 8; y++)
	{
		str[1 + y * 9] = '\'';
		for (int x = 0; x < 8; x++)
		{
			str[2 + x + y * 9] = '0' + grid[x][y];
		}
	}

	str[0] = '0';
	str[1] = 'b';

	printf("%s\n", str);
}


int main(int argc, char** argv)
{
	sft_init();

	int scale = 50;
	sft_window* win = sft_window_open("Bitmap Char -> u64",
		400, 400, -1, -1,
		sft_flag_darkmode | sft_flag_noresize);

	bool grid[8][8] = { 0 };

	while (sft_window_update(win))
	{
		sft_input_update();

		if (sft_window_hasFocus(win))
		{
			sft_point mouse = sft_input_mousePos(win);

			// dont show hover square if out of bounds
			if (mouse.x < 0)
				mouse.x = 100000;
			if (mouse.y < 0)
				mouse.y = 100000;

			mouse.x = mouse.x / scale;
			mouse.y = mouse.y / scale;

			if (sft_input_clickPressed(sft_click_Left))
				if (mouse.x >= 0 && mouse.y >= 0 && mouse.x < 8 && mouse.y < 8)
					grid[mouse.x][mouse.y] = !grid[mouse.x][mouse.y];

			sft_window_fill(win, 0xFF000000);

			for (int i = 0; i < 8; i++)
			{
				sft_window_drawRect(win, i * scale, 0, 1, 8 * scale, 0xFFFFFFFF);
				sft_window_drawRect(win, 0, i * scale, 8 * scale + 1, 1, 0xFFFFFFFF);
			}
			sft_window_drawRect(win, 8 * scale - 1, 0, 1, 8 * scale, 0xFFFFFFFF);
			sft_window_drawRect(win, 0, 8 * scale - 1, 8 * scale + 1, 1, 0xFFFFFFFF);


			for (int x = 0; x < 8; x++)
				for (int y = 0; y < 8; y++)
					if (grid[x][y])
						sft_window_drawRect(win,
							x * scale + 2,
							y * scale + 2,
							scale - 3, scale - 3, 0xFFFFFFFF);

			if (mouse.x >= 0 && mouse.y >= 0 && mouse.x < 8 && mouse.y < 8)
				sft_window_drawRect(win,
					mouse.x * scale + 1,
					mouse.y * scale + 1,
					scale - 1, scale - 1,
					grid[mouse.x][mouse.y] ? 0xFFFF0000 : 0xFF7F0000);

			sft_window_display(win);

			if (sft_input_keyPressed(sft_key_Enter))
				toU64Str(grid);
		}

		sft_sleep(50);
	}

	sft_shutdown();


	return 0;
}