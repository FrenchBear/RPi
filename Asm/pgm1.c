// pgm1.c

int x, y;

int fdouble(int n) {
	return n+n;
}

void main() {
	int i, j, k;

	i = 3;
	j = 12;
	y = k = i+6-j++;
	x = fdouble(i);
}
