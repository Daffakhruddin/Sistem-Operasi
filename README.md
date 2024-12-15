# Chat Server-Client dengan WebSocket (C)

Chat Server-Client dengan WebSocket (C)
Program ini adalah implementasi server-client berbasis TCP menggunakan bahasa C. Server menggunakan model berbasis fork() untuk menangani beberapa klien secara bersamaan. Klien dapat mengirimkan pesan ke server, dan server akan mengirimkan kembali pesan yang sama (echo). Program ini juga mendukung fitur tambahan seperti username, koneksi aman, dan penanganan input untuk menjaga stabilitas.

## Fitur
1. Pembuatan Socket: Server dan klien menggunakan socket TCP untuk komunikasi.
2. Binding dan Listening: Server dapat mengikat socket ke port tertentu dan mendengarkan koneksi klien.
3. Menerima Koneksi Multi-Client: Server mendukung banyak klien secara bersamaan menggunakan fork().
4. Echo Service: Server menerima pesan dari klien dan mengirimkan kembali pesan yang sama.
5. Username Klien: Klien diminta memasukkan username saat terhubung, dan username ini ditampilkan di log server.
6. Keluar dengan 'exit': Klien dapat mengetik exit untuk memutuskan koneksi dengan server.
7. Deteksi Server Mati: Jika server mati tiba-tiba, klien akan langsung keluar tanpa hang.
8. Keamanan: Input dari klien diproses dengan aman untuk mencegah buffer overflow.
9. Efisiensi: Server dapat menangani lebih dari 6 klien secara bersamaan tanpa penurunan performa.
10. Keandalan: Server tetap melayani klien meskipun salah satu koneksi gagal.

## Cara Menjalankan Program
1. Kompilasi Program
Kompilasi file server.c dan client.c menggunakan GCC atau compiler C lainnya.
`gcc -o server server.c`
`gcc -o client client.c`

2. Menjalankan Server
Jalankan server terlebih dahulu pada terminal.
`./server`

4. Menjalankan Client
Pada terminal lain, jalankan klien untuk terhubung ke server.
`./client`

## Struktur Program
- server.c:
1. Menerima koneksi dari klien.
2. Menangani pesan klien dengan proses child (menggunakan fork()).
3. Memberikan layanan echo.
- client.c:
1. Menghubungkan ke server.
2. Mengirim dan menerima pesan dari server.
3. Mendukung input pengguna dan deteksi server mati.


