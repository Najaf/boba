/**
 * 'bnet' stands for 'boba networking'
 *
 * Utility functions for working with sockets
 */

int create_server_socket(char *host, char *port);
int create_client_socket(char *remote_host, char *remote_port);
int send_bytes_on_tcp_socket(int fd, void *buffer, int bytes);
int recv_bytes_on_tcp_socket(int fd, void *buffer, int bytes);
int close_tcp_socket(int fd);



