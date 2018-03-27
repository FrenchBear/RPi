// ch1 ex02 hwclient.c
// Hello world client
// 2016-06-22	PV

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main() {
	printf("Connecting to hello world server\n");
	void *context = zmq_ctx_new();
	void *requester = zmq_socket(context, ZMQ_REQ);
	int rc = zmq_connect(requester, "tcp://localhost:5555");
	assert(rc==0);

	int nbreq;
	for (nbreq=0 ; nbreq<10 ; nbreq++) {
		char buffer[10];
		printf("Sending Hello %d\n", nbreq);
		zmq_send(requester, "Hello", 5, 0);
		zmq_recv(requester, buffer, 10, 0);
		printf("Received World\n");
	}
	zmq_close(requester);
	zmq_ctx_destroy(context);
	return 0;
}
