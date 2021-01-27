#include "helpers.h"

void send_message(int destination_id, void *message, int message_len, int type)
{
    /* Handle integer messaes */
    if (type == INT_MESSAGE) {
        int *payload = (int*) message;
        MPI_Send(payload, 1, MPI_INT, destination_id, 0, MPI_COMM_WORLD);
    }

    /* Handle string messsages */
    if (type == STRING_MESSAGE) {
        char *payload = (char *)message;

        /* Send payload length then payload */
        MPI_Send(&message_len, 1, MPI_INT, destination_id, 0, MPI_COMM_WORLD);
        MPI_Send(payload, message_len, MPI_CHAR, destination_id, 0, MPI_COMM_WORLD);
    }
}

int receive_message(int source_id, void *message, int message_len, int type)
{
    MPI_Status status;

    /* Handle int messages */
    if (type == INT_MESSAGE) {
        int payload_len;
        MPI_Recv(&payload_len, 1, MPI_INT, source_id, 0, MPI_COMM_WORLD, &status);
        message = NULL;
        return payload_len;
    }

    /* Handle string messages */
    if (type = STRING_MESSAGE) {
        MPI_Recv(message, message_len, MPI_CHAR, source_id, 0, MPI_COMM_WORLD, &status);
        return message_len;
    }
}

void handle_master(char *input_file)
{
    string in_file_name(input_file);
    fstream in_file;
    in_file.open(in_file_name);

    string line = "";
    getline (in_file, line);
    int N = stoi(line);

    /* Stabilim cat ar avea de prelucrat fiecare nod daca s-ar imparti egal*/
    int worker_load = N / (NUM_WORKERS + 1);
    /* Verificam cate din puncte mai trebuie prelucrate dac anu se impart egal */
    int overflow =  N % (NUM_WORKERS + 1);
    /* Asignam lui master numarul de puncte suplimentare */
    int master_load = worker_load + overflow;
    cout << "load master: " << master_load << endl;

    int recv_count;
    int total_count = 0;
    int master_count = 0;

    /* Trimite work Load-ul catre fiecare worker */
    for (int i = 1; i <= NUM_WORKERS; i++) {
        send_message(i, &worker_load, 1, INT_MESSAGE);
    }

    /* Ocupa-te de work lead-ul master-ului */
    srand(time(NULL));
    double x, y, z;
    int count = 0;
    for (int i = 0; i < master_load; i++) {
        x = (double)random() / RAND_MAX;
        y = (double)random() / RAND_MAX;
        z = sqrt((x * x) + (y * y));
        if (z <= 1)
            master_count++;
    }

    /* Primim raspunsurile de la fiecare worker */
    for (int i = 1; i <= NUM_WORKERS; i++) {
        recv_count = receive_message(i, NULL, 1, INT_MESSAGE);
        total_count += recv_count;
    }

    total_count += master_count;
    cout << "Final count: " << total_count << " Points: " << N << endl;
    double pi = ((double)total_count / (double)(N)) * 4.0;
    cout << "PI ~~ " << pi << endl;

    in_file.close();
}

void handle_worker(int worker_type)
{
        int work_load = receive_message(MASTER, NULL, 1, INT_MESSAGE);
        srand(time(NULL));
        double x, y, z;
        int count = 0;
        for (int i = 0; i < work_load; i++) {
            x = (double)random() / RAND_MAX;
            y = (double)random() / RAND_MAX;
            z = sqrt((x * x) + (y * y));
            if (z <= 1)
                count++;
        }
        cout << "load " << worker_type << ": " << work_load << "; Count: "<< count <<endl;
        send_message(MASTER, &count, 1, INT_MESSAGE);
}