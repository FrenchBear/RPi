// ch1 ex02 hwserver.c
// Hello world server
// 2016-06-22	PV

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main() {
	// Socket to talk to clients
	void *context = zmq_ctx_new();
	void *responder = zmq_socket(context, ZMQ_REP);
	int rc = zmq_bind(responder, "tcp://*:5555");
	assert(rc==0);

	while(1) {
		char buffer[10];
		zmq_recv(responder, buffer, 10, 0);
		printf("Received Hello\n");
		sleep(1);
		zmq_send(responder, "World", 5, 0);
	}
	return 0;
}
