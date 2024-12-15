
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 55555
#define BUFFER_SIZE 1024

// Fungsi untuk menangani disconnect dari server, mengakhiri program klien
void handle_disconnect(int sig) {
    printf("\nDisconnected from server.\n");
    exit(0);
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Menangani sinyal SIGPIPE jika server tiba-tiba mati
    signal(SIGPIPE, handle_disconnect);

    // buat socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Mengkonfigurasi alamat server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    // Menghubungkan ke server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server. Type 'exit' to disconnect.\n");

    // Loop untuk komunikasi dengan server
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            printf("Disconnected from server.\n");
            break;
        }

        // Menampilkan pesan dari server
        printf("%s", buffer);

        // Membaca input user
        memset(buffer, 0, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);

        // Mengirim pesan ke server
        if (send(sock, buffer, strlen(buffer), 0) == -1) {
            perror("Send failed");
            break;
        }

        // Jika klien mengetik "exit", keluar dari loop dan menutup koneksi
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Disconnected from server.\n");
            break;
        }
    }

    close(sock);// Menutup koneksi ke server
    return 0;
}
