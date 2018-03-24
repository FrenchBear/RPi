int map2(int value, int min_in, int max_in, int min_out, int max_out) {
	return (value-min_in)/(double)(max_in-min_in)*(max_out-min_out)+min_out;
}
