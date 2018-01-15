#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *pfile;
	char *buffer;
	long filelen;
	pfile = fopen("test.png", "rb");
	fseek(pfile, 0, SEEK_END);
	filelen = ftell(pfile);
	rewind(pfile);

	buffer = (char *)malloc((filelen + 1) * sizeof(char));
	fread(buffer, filelen, 1, pfile);
	// printf("%s", buffer);
	fclose (pfile);
	// pfile = fopen ("test2.png", "wb");
	// fwrite (buffer , sizeof(char), sizeof(buffer), pfile);
	// fclose(pfile);

	pfile = fopen("test2.png", "wb");
	while (filelen > 0)
    {
        int written = fwrite(buffer, sizeof(char), filelen, pfile);
        if (written < 1)
        {
            printf("Can't write to file");
            return -1;
        }

        buffer += written;
        filelen -= written;
    }

	return 0;
}