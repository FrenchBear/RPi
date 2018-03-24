// order.c
// find out order of bytes storage in memory
// 2016-06-20	PV

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

int main() {

	uint32_t i = 0x12345678;
	uint8_t *p = (uint8_t *)&i;

	printf("p[0]: %02x\n", p[0]);
	printf("p[1]: %02x\n", p[1]);
	printf("p[2]: %02x\n", p[2]);
	printf("p[3]: %02x\n", p[3]);

	FILE *f = fopen("/home/pi/temp/f1.bin", "wb");
	if (f==NULL)
		perror("Opening file failed");
	else {
		fwrite(&i, sizeof(uint32_t), 1, f);
		fclose(f);
	}

	return 0;
}
