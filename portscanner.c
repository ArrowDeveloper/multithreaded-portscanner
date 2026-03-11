#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>
#include <netdb.h>

int next_port = 1;
int end_port;
pthread_mutex_t counter_lock;

typedef struct{
    struct addrinfo *final;
    int port;
}scan_args;

int port_scan(struct addrinfo *final, int port){
    struct timeval tv;
    struct addrinfo hints;
    struct sockaddr_storage local_addr; 

    memcpy(&local_addr, final->ai_addr, final->ai_addrlen);


    if (final->ai_family == AF_INET)
        ((struct sockaddr_in *)&local_addr)->sin_port = htons(port);
    else if (final->ai_family == AF_INET6)
        ((struct sockaddr_in6 *)&local_addr)->sin6_port = htons(port);

    int sock = socket(final->ai_family, final->ai_socktype, final->ai_protocol);
    
    if(sock<0){
        perror("sock");
        return 1;
    }


    tv.tv_sec = 0;
    tv.tv_usec= 300000;

    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));

    int result = connect(sock, (struct sockaddr *)&local_addr, final->ai_addrlen);

    if(result==0){ 	
	    printf("Port is open at: %d\n", port);
	    }
    close(sock);

    return result==0;


}

void *worker(void *arg){

    scan_args *args = (scan_args *)arg;
    while (1) {

        pthread_mutex_lock(&counter_lock);

        int port = next_port;

        if (port > end_port){  
            pthread_mutex_unlock(&counter_lock);
            break;
        }

        next_port++;
        
        pthread_mutex_unlock(&counter_lock);

        port_scan(args->final, port);
    }

    return NULL;

}


int main(int argc, char *argv[]){
    if(argc != 5) {
        printf("Usage: %s <host> <start_port> <end_port> <threads>\n", argv[0]);
        return 1;
    }

	char *host = argv[1];
	int start_port = atoi(argv[2]);
	end_port = atoi(argv[3]);
    int THREADS = atoi(argv[4]);

    if (THREADS > 500){
        printf("LOWER AMOUNT OF THREADS!");
        return 1;
    }

    if (start_port < 1 | end_port > 65535){
        printf("Invalid PORT");
        return 1;
    }

    pthread_t workers[THREADS];
    pthread_mutex_init(&counter_lock, NULL);

    struct addrinfo hints;
    struct addrinfo *final;
    

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;   
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo(host, NULL, &hints, &final);

    if(err != 0){
        printf("%s\n", gai_strerror(err));
        return 1;
    }
    
    next_port = start_port;

    scan_args args;
    args.final = final;

    for(int i=0; i<THREADS; i++){

        if (pthread_create(&workers[i], NULL, worker, &args) != 0){
            perror("pthread_create failed");
            return 1;
        }
    }

    for(int i=0; i<THREADS; i++){
        pthread_join(workers[i], NULL);
    }

	pthread_mutex_destroy(&counter_lock);
    freeaddrinfo(final);
 
	return 0;

}