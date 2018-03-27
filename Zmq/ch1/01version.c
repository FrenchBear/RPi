// Show zmq version
// 2017-06-22	PV

#include <zmq.h>

int main() {
	int M,m,p;
	zmq_version(&M, &m, &p);
	printf("Current ZMQ version: %d.%d.%d\n", M, m, p);
	return 0;
}
