#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include "listen.h"

#define SOCKET_NAME "@moc_socket"
#define CHUNK_SIZE 8192
#define MAX_ALLOWED_CAPACITY 524288000 // 500 MB Cap

// Pass a pointer-to-pointer (char **) so we can modify the address in main()
int start_server_listener(char **out_buffer, size_t *out_size) {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char chunk[CHUNK_SIZE];

    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    addr.sun_path[0] = '\0';
    strncpy(addr.sun_path + 1, SOCKET_NAME, sizeof(addr.sun_path) - 2);
    socklen_t addr_len = sizeof(addr.sun_family) + 1 + strlen(SOCKET_NAME);

    if (bind(server_fd, (struct sockaddr *)&addr, addr_len) < 0) {
        perror("bind");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return -1;
    }

    if ((client_fd = accept(server_fd, NULL, NULL)) < 0) {
        perror("accept");
        close(server_fd);
        return -1;
    }

    char *dynamic_buf = NULL;
    size_t current_capacity = 0;
    size_t total_bytes_received = 0;
    int bytes_read;

    while ((bytes_read = recv(client_fd, chunk, CHUNK_SIZE, 0)) > 0) {

        // Check if we need more room (+1 for the upcoming null terminator)
        if (total_bytes_received + bytes_read + 1 > current_capacity) {
            // Double the capacity or match the incoming size, whichever is larger
            size_t new_capacity = current_capacity * 2;
            if (new_capacity < total_bytes_received + bytes_read + 1) {
                new_capacity = total_bytes_received + bytes_read + 1;
            }

            // Ask the OS for a larger block of memory
            char *temp = realloc(dynamic_buf, new_capacity);
            if (!temp) {
                perror("realloc failed");
                break; // Keep what we have so far
            }
            dynamic_buf = temp;
            current_capacity = new_capacity;
        }

        // Copy the new chunk into our expanded buffer
        memcpy(dynamic_buf + total_bytes_received, chunk, bytes_read);
        total_bytes_received += bytes_read;
    }

    if (dynamic_buf) {
        dynamic_buf[total_bytes_received] = '\0'; // Safe null termination
    }

    close(client_fd);
    close(server_fd);

    // Pass the newly allocated address and size back to main()
    *out_buffer = dynamic_buf;
    *out_size = total_bytes_received;

    return (int)total_bytes_received;
}
