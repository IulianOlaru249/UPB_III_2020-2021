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

using namespace std;

/**
 * Number of threads the master node will open
 */
#define MASTER_THREADS_COUNT 4

/**
 * All the diffrent types of MPI nodes 
 */
enum MPI_NODES { 
    MASTER,
    HORROR,
    COMEDY,
    FANTASY,
    SCIENCE_FICTION
};

/**
 * The diffrent types of messages that can be sent 
 */
enum MESSAGE_TYPES {
    INT_MESSAGE,
    STRING_MESSAGE
};

/**
 * Sliding window and index to keep track of packadge order
 */
extern map<int, pair<string, char*>> sliding_window;
extern int last_printed;

/**
 * Lock used for safe opperations on the sliding window
 */
extern pthread_mutex_t lock;

/** 
 * Output file
 */
extern ofstream out_file;

/**
 * Total cores of the machine
 */
extern int cores;

/**
 * Parameters required by the master's threads
 */
typedef struct _master_thread_params {
    int thread_id;
    char *file_name;
} master_thread_params;

/**
 * Parameters required by the worker's threads
 */
typedef struct _worker_thread_params {
    int thread_id;
    int worker_type;
    int number_of_chunks;
    int number_of_cores;
    vector<string>* chunks;
} worker_thread_params;

/**
 * Opens/closes the output file and starts/joins threads.
 * 
 * @param: name of output file
 */
void handle_master(char *input_file);

/**
 * Function executed by master threads.
 * Used to break text into paragraph( each 
 * thread for ine type of paragraph), send it
 * to the workers for processing, receiving the
 * processed paragraphs and printing them in the
 * correct order.
 * 
 * @param: struct _master_thread_params described above
 */
void *master_thread_function (void *arg);

/**
 * Initiate the thread responsible for receiving
 * paragraphs from master
 * 
 * @param: the type of the worker (e.g. HORROR)
 */
void handle_worker(int worker_type);

/**
 * Function executed by worker threads.
 * 
 * For the receiving thread: 
 *      gets paragraphs from master
 *      breaks paragraphs in chunks of 20 lines
 *      starts as many threads as chunks (max P - 1)
 *      sends chunks to threads to be processed
 *      puts chunks in correct order
 *      sends processed paragraph back to master
 * 
 * For the processing threads:
 *      devide chunk array into equal sizes to split work
 *      processes each chunk depending on the worker type
 *  
 * @param: struct _worker_thread_params described above 
 */
void *worker_thread_function (void *arg);

/**
 * Bundle of functions used to process the
 * paragraphs as required.
 */
void process_horror_paragraph(string &paragraph);
void process_comedy_paragraph(string &paragraph);
void process_fantasy_paragraph(string &paragraph);
void process_science_fiction_paragraph(string &paragraph);

/**
 * Sends messages between MPI nodes
 * For int messages:
 *      sends the value
 * 
 * For string messages:
 *      sends the length
 *      sends the value
 * 
 * @param 1: destination
 * @param 2: message payload
 * @param 3: length of message
 * @param 4: the type of message (int/ string)
 */
void send_message(int destination_id, void *message, int message_len, int type);

/**
 * Receivs messages between MPI nodes
 * For int and string messages:
 *      receivs the value
 * 
 * @param 1: source
 * @param 2: message payload
 * @param 3: length of message
 * @param 4: the type of message (int/ string)
 * @return: value of int message / length of string message
 */
int receive_message(int source_id, void *message, int message_len, int type);

/**
 * Reads from file untill finds an empty line or EOF and composes a paragraph
 * 
 * @param 1: reffrence to the paragraph that will be composed
 * @param 2: reffrence to the input stream
 * @param 3: reffrence to the paragrap's associated id
 * @return: the number of lines in the paragraph
 */
int get_paragraph(string &paragraph, ifstream &file, int &paragraph_id);

/**
 * Breaks paragraph into 20 lines chunks
 * 
 * @param 1: reffrence to the vector that will hold the chunks
 * @param 2: reffrence to the paragraph that will be processed
 * @return: the number of chunks
 */
int break_in_chunks(vector<string>& chunks, string &paragraph);

/**
 * Checks if character is vowel
 * 
 * @param: leter to be checked
 * @return: whether is vowel or not
 */
bool is_vowel(char letter);

#endif