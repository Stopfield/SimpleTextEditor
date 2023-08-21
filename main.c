#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

void enable_raw_mode()
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main()
{
    char buffer[1024];
    size_t buffer_index = 0;

    enable_raw_mode();
    // setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    int escape_sequence = 0;

    char c;
    while (1)
    {
        read(STDIN_FILENO, &c, 1);

	if (c == 27)
	{
		break;
	}

        if (c == 127)
        {
            putchar('\b');
            buffer_index--;
            putchar(' ');
            buffer[buffer_index] = ' ';
            putchar('\b');
        }
        else
        {
            putchar(c);
            buffer[buffer_index] = c;
            buffer_index++;
        }
    }
    printf("\nBuffer Contents\n%s\n", buffer);
    fflush(stdout);

    /* Now we write it to a file */
    FILE* file;
    file = fopen("test.out", "w");
    fprintf(file, "%s", buffer);
    fclose(file);
    return 0;

}

