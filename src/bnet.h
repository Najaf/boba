/**
 * 'bnet' stands for 'boba networking'
 *
 * Utility functions for working with sockets
 */

int create_client_socket(char *remote_host, char *remote_port);
int create_server_socket(char *host, char *port);
int accept_connection_on_socket(int fd);
int send_bytes_on_tcp_socket(int fd, void *buffer, int bytes);
int recv_bytes_on_tcp_socket(int fd, void *buffer, int bytes);
void close_tcp_socket(int fd);



