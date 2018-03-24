
void main() {
	register int total = 0;
	register int i = 1024;
	while (i>0) {
		total += i%10;
		i /= 10;
	}
}
