/**
 * @file sub.cpp
 *
 * @breif Test client that connects to pub server.
 */
#include <czmq.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    const char name[] = "pub";
    zctx_t* ctx = zctx_new();
    void* sub = zsocket_new(ctx, ZMQ_SUB);
    zsocket_set_subscribe(sub, "");
    //zsocket_connect(sub, "ipc://%s-server.ipc", name);
    zsocket_connect(sub, "ipc://pub-server.ipc");
    zmq_pollitem_t items[] = {
        { sub, 0, ZMQ_POLLIN, 0 }
    };
    while (true) {
        printf("I: polling for pub messages...\n");
        int rc = zmq_poll(items, 1, 1000 * ZMQ_POLL_MSEC);
        if (rc == -1)
            break;
        if (items[0].revents & ZMQ_POLLIN) {
            char* peer = zstr_recv(sub);
            char* load = zstr_recv(sub);
            printf("%s - load %s\n", peer, load);
            free(peer);
            free(load);
        }
        fflush(stdout);
    }
    zctx_destroy(&ctx);
    return EXIT_SUCCESS;
}