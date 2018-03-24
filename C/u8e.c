unsigned char *utf8encode(int c) {
	static unsigned char buf[7];

	if (c<=0x7f) {
		buf[0] = c;
		buf[1] = 0;
	}
	else if (c<=0x7ff) {
		buf[0] = 0xC0+c/0x40;
		buf[1] = 0x80+c%0x40;
		buf[2] = 0;
	}
	else {
		buf[0] = 0xE0+(c/0x40)/0x40;
		buf[1] = 0x80+(c/0x40)%0x40;
		buf[2] = 0x80+c%0x40;
		buf[3] = 0;
	}

	return buf;
}

