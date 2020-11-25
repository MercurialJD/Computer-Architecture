#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <limits.h>

#define UNUSED __attribute__((unused))

/* Global variables declarations */
int total_nodes_num, start_node, end_node, nodes_per_thread, total_threads, passed_nodes_num;

/* Variables indicating the current/next node and the next minimal distance */
int current_node, next_node, next_min_dist;

/* Matrix declaration */
int **matrix;

/* Helper tables */
int *visit_state_tab, *pre_node_tab, *dist_tab;

/* mutex stuff */
pthread_mutex_t mutex;

/* Handle inputs */
void get_inputs() {
    /* In case of the scanf_warning */
    int _scanf_value UNUSED;

    /* Get parameters of the graph and build the empty graph */
    _scanf_value = scanf("%d %d %d", &total_nodes_num, &start_node, &end_node);

    /* Reserve memory */
    matrix = malloc(total_nodes_num * sizeof(int *));
    visit_state_tab = calloc(total_nodes_num, sizeof(int)); /* new memory for visit_table */
    pre_node_tab = malloc(total_nodes_num * sizeof(int));    /* new memory for pre_node_table */
    dist_tab = malloc(total_nodes_num * sizeof(int));   /* new memory for dist_table */
    for(int i = 0; i < total_nodes_num; ++i) {
        matrix[i] = malloc(total_nodes_num * sizeof(int));
        dist_tab[i] = -1;   /* Initialize dists */
    }

    /* Get the graph */
    for(int i = 0; i < total_nodes_num; ++i) {
        for (int j = 0; j < total_nodes_num; ++j) {
            /* _scanf_value = scanf("%d", &matrix[i][j]); */
            int x = 0;
            char ch = getchar();
            while (ch < '0' || ch > '9') {  /* Find the next integer */
                ch = getchar();
            }
            while(ch >= '0' && ch <= '9') {
                x *= 10;
                x += (ch - '0');    /* Offset */
                ch = getchar();
            }
            matrix[i][j] = x;
        }
    }

    // for(int i = 0; i < total_nodes_num; ++i) {
    //     for (int j = 0; j < total_nodes_num; ++j) {
    //         printf("%d ", matrix[i][j]);
    //     }
    //     printf("\n");
    // }
}

/* Set all paths connected with the starting node */
void* setFollowingPath(void *thread_num) {

    /* Thread index begins with 0 */
    int thread_number = (int)thread_num;

    /* [first_node, last_node) */
    int first_node = thread_number * nodes_per_thread;
    int last_node = (thread_number < total_threads - 1) ? ((thread_number + 1) * nodes_per_thread) : (total_nodes_num);

    /* Set the direct distance and pre_node */
    pthread_mutex_lock(&mutex);
    for(int j = first_node; j < last_node; ++j) {
        if(matrix[current_node][j] > 0 && current_node != j && (dist_tab[j] == -1 || (dist_tab[j] > (matrix[current_node][j] + dist_tab[current_node])))) {
            dist_tab[j] = matrix[current_node][j] + dist_tab[current_node]; /* loosen */
            pre_node_tab[j] = current_node; /* set pre_node */
        }
    }
    /* printf("Thread: %d, settign path, first_node: %d, last_node: %d, current node is: %d\n", thread_number, first_node, last_node, current_node); */
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL); /* Exit thread */
    return NULL;
}

/* Function to find the next minimal-distance node */
void* findNextNode(void *thread_num) {

    /* Thread index begins with 0 */
    int thread_number = (int)thread_num;

    /* [first_node, last_node) */
    int first_node = thread_number * nodes_per_thread;
    int last_node = (thread_number < total_threads - 1) ? ((thread_number + 1) * nodes_per_thread) : (total_nodes_num);

    /* compare */
    for (int i = first_node; i < last_node; ++i) {
        if(!visit_state_tab[i]) {   /* Must be unvisited nodes */
            pthread_mutex_lock(&mutex);
            if(dist_tab[i] > -1 && dist_tab[i] <= next_min_dist) {
                next_min_dist = dist_tab[i];    /* Set new minimum */
                next_node = i;
            }
            pthread_mutex_unlock(&mutex);
        }
    }

    /* printf("Thread: %d, finding next node, first_node: %d, last_node: %d, current node is: %d, next node is: %d\n", thread_number, first_node, last_node, current_node, next_node); */
    pthread_exit(NULL); /* Exit thread */
    return NULL;
}

/* The final printing funct */
void print() {
    /* Reserve memory for nodes on the path */
    int *passed_nodes = malloc(passed_nodes_num * sizeof(int));
    int back_node = end_node;

    /* Get all nodes */
    for (int i = 0; i < passed_nodes_num; ++i) {
        passed_nodes[passed_nodes_num - 1 - i] = back_node;
        back_node = pre_node_tab[back_node];
    }

    /* Real place for printing */
    printf("%d\n", dist_tab[end_node]);
    for (int i = 0; i < passed_nodes_num; ++i) {
        /* printf("%d\n", passed_nodes[i]); */
        int output[32];
        int count = 0, node = passed_nodes[i];
        do {
            output[count++] = node % 10;
            node /= 10;
        } while (node != 0);
        while(count-- != 0) {
            putchar(output[count] + 48);    /* 48 indicates '0' */
        }
        putchar('\n');
    }

    /* Free memory */
    free(passed_nodes);
}

/* Some memory clean ups */
void releaseMemory() {

    /* Free global variables */
    free(visit_state_tab);  /* Free visited_table */
    free(pre_node_tab); /* Free pre_node_table */
    free(dist_tab); /* Free dist_table */
    for(int i = 0; i < total_nodes_num; ++i) {
        free(matrix[i]);    /* Don't forget the nested-mem */
    }
    free(matrix);
}

/* Calculate the shortest distance given the input. */
void dist (int max_threads)
{
    /* Thread poll and attribute */
    total_threads = max_threads;
    pthread_t *threads = malloc(total_threads * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_mutex_init(&mutex, NULL);   /* Init mutex */

    /* Get inputs */
    get_inputs();

    /* Dispatch workload to different threads */
    nodes_per_thread = total_nodes_num / total_threads;
    /* printf("total_nodes_num: %d, total_threads: %d, nodes_per_thread: %d\n", total_nodes_num, total_threads, nodes_per_thread); */

    /* Initialization for the starting node */
    visit_state_tab[start_node] = 1;
    pre_node_tab[start_node] = -1;  /* Set the pre_node for start_node as a different num */
    dist_tab[start_node] = 0;
    current_node = start_node;
    passed_nodes_num = 1;

    /* Preparation for threads */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /* Set all paths connected with the starting node */
    /* Direct passing &i will get changed is */
    for(int i = 0; i < total_threads; ++i) {
        pthread_create(&threads[i], &attr, setFollowingPath, (void *)i);
    }
    pthread_attr_destroy(&attr);    /* Wait */

    /* Join threads */
    for(int i = 0; i < total_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    /* printf("Init Joined\n"); */

    while(!visit_state_tab[end_node]) {

        /* Reset the minimal distance */
        next_min_dist = INT_MAX;

        /* Preparation for threads */
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        /* multi-threads to find the next node */
        /* Direct passing &i will get changed is */
        for(int i = 0; i < total_threads; ++i) {
            pthread_create(&threads[i], &attr, findNextNode, (void *)i);
        }
        pthread_attr_destroy(&attr);     /* Wait */

        /* Join threads */
        for(int i = 0; i < total_threads; ++i) {
            pthread_join(threads[i], NULL);
        }
        /* printf("Finding joined\n"); */

        /* Set and update info */
        pre_node_tab[next_node] = current_node;
        dist_tab[next_node] = next_min_dist;
        visit_state_tab[next_node] = 1;
        passed_nodes_num += 1;  /* Record passed nodes num */
        current_node = next_node;

        /* Preparation for threads */
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        /* multi-threads to find the next node */
        /* Direct passing &i will get changed is */
        for(int i = 0; i < total_threads; ++i) {
            pthread_create(&threads[i], &attr, setFollowingPath, (void *)i);
        }
        pthread_attr_destroy(&attr);     /* Wait */

        /* Join threads */
        for(int i = 0; i < total_threads; ++i) {
            pthread_join(threads[i], NULL);
        }
        /* printf("Setting paths joined\n"); */
    }

    /* Output */
    print();

    /* Clean up */
    pthread_mutex_destroy(&mutex);
    free(threads);
    releaseMemory();
}
