#ifndef HELPERS_H
#define HELPERS_H

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <sstream>
#include <pthread.h>
#include <math.h>

using namespace std;

#define NUM_WORKERS 4

enum MPI_NODES {
    MASTER,
};

enum MESSAGE_TYPES {
    INT_MESSAGE,
    STRING_MESSAGE
};

extern pthread_mutex_t lock;
extern pthread_barrier_t barrier;
extern int cores;

void send_message(int destination_id, void *message, int message_len, int type);
int receive_message(int source_id, void *message, int message_len, int type);

void handle_master(char *input_file);
void handle_worker(int worker_type);

#endif