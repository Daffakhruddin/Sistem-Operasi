#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

#define PORT 55555
#define BUFFER_SIZE 1024

// Fungsi ini menangani setiap klien yang terhubung ke server
void handle_client(int client_socket, struct sockaddr_in client_addr) {
    char buffer[BUFFER_SIZE];
    char username[50];

    // Meminta username
    send(client_socket, "Enter your username: ", 22, 0);
    int len = recv(client_socket, username, sizeof(username), 0);
    if (len <= 0) {
        perror("Error reading username");
        close(client_socket);
        exit(1);
    }
    username[len - 1] = '\0'; // Hapus newline

    // Notifikasi kepada server ada koneksi client baru
    printf("New connection from %s (%s:%d)\n",
           username,
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    // Tampilan Setelah Login
    char welcome_message[100];
    snprintf(welcome_message, sizeof(welcome_message), "Welcome %s! \n", username);
    send(client_socket, welcome_message, strlen(welcome_message), 0);

    // Loop untuk menerima dan mengirim pesan dari dan ke klien
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            printf("Client %s disconnected.\n", username);
            break;
        }

        buffer[bytes_received - 1] = '\0'; // Hapus newline

        // Cek client mengirim "exit"
        if (strcmp(buffer, "exit") == 0) {
            printf("Client %s requested to disconnect.\n", username);
            break;
        }

        // Menampilkan pesan yang diterima dan mengirimkan kembali ke klien
        printf("Message from %s: %s\n", username, buffer);
        send(client_socket, buffer, bytes_received, 0);
    }
    // Menutup koneksi klien setelah selesai
    close(client_socket);
    exit(0); // Mengakhiri proses anak
}

// Fungsi ini menangani sinyal SIGCHLD untuk membersihkan proses anak yang telah selesai
void handle_sigchld(int sig) {
    (void)sig; // Unused parameter
    while (waitpid(-1, NULL, WNOHANG) > 0);
}


int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Membuat socket untuk server
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Mengkonfigurasi alamat server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Binding socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listening koneksi
    if (listen(server_socket, 10) == -1) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Menangani sinyal SIGCHLD untuk membersihkan proses anak yang selesai
    signal(SIGCHLD, handle_sigchld);

    // Loop untuk menerima koneksi klien
    while (1) {
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }

        // Membuat proses anak untuk menangani klien yang terhubung
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            close(client_socket);
        } else if (pid == 0) { // Child process
            close(server_socket);
            handle_client(client_socket, client_addr);
        } else { // Parent process
            close(client_socket);
        }
    }

    close(server_socket);// Menutup socket server setelah loop selesai
    return 0;
}
