# 🔌 Membuat TCP Socket Connection

## 📃 Deskripsi Projek
Proyek ini adalah implementasi sederhana **TCP Socket Connection** menggunakan bahasa **C++** dan **Winsock2 API** pada sistem operasi Windows.  
Program terdiri dari dua bagian utama:
- **server.cpp** → bertindak sebagai server yang menunggu koneksi client.
- **client.cpp** → bertindak sebagai client yang terhubung ke server dan mengirim pesan.

## 🎯 Tujuan Projek
- Memahami dasar-dasar pemrograman socket dengan **C++**.
- Mempelajari cara kerja **server** dan **client** pada komunikasi berbasis **TCP**.
- Melatih penggunaan **Winsock API** (Windows Sockets).

## ⚙️ Cara Kerja

### 🔹 server.cpp
1. **Include header dan deklarasi variabel**
   ```cpp
   #include <iostream>
   #include <array>
   #include <winsock2.h>
    ```

* `winsock2.h` adalah header utama untuk socket di Windows.
* `std::array<char, 1024> buffer;` → buffer untuk menampung data dari client.

2. **Inisialisasi Winsock**

   ```cpp
   if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
       std::cerr << "WSAStartup failed. Error: " << WSAGetLastError() << std::endl;
       return 1;
   }
   ```

   * `WSAStartup()` wajib dipanggil sebelum menggunakan socket di Windows.
   * Versi `2.2` dari Winsock digunakan di sini.

3. **Membuat Socket Server**

   ```cpp
   serverSocket = socket(AF_INET, SOCK_STREAM, 0);
   ```

   * `AF_INET` → IPv4.
   * `SOCK_STREAM` → TCP (reliable, connection-based).

4. **Konfigurasi IP address server**

   ```cpp
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port = htons(9090);
   ```

   * `INADDR_ANY` → menerima koneksi dari semua IP address lokal.
   * `htons(9090)` → port yang dipakai adalah **9090**.

5. **Bind socket ke IP Address/port**

   ```cpp
   bind(serverSocket, (sockaddr*)&server, sizeof(server));
   ```

   * Menghubungkan socket ke alamat IP & port yang sudah dikonfigurasi.

6. **Listen (menunggu koneksi)**

   ```cpp
   listen(serverSocket, SOMAXCONN);
   ```

   * `SOMAXCONN` → batas maksimal antrian koneksi yang bisa ditangani.

7. **Menerima koneksi client**

   ```cpp
   clientSocket = accept(serverSocket, (sockaddr*)&client, &clientSize);
   ```

   * `accept()` akan blocking sampai client mencoba terhubung.
   * Setelah berhasil, terbentuk socket baru khusus komunikasi dengan client.

8. **Menerima pesan dari client**

   ```cpp
   bytesReceived = recv(clientSocket, buffer.data(), sizeof(buffer), 0);
   ```

   * `recv()` membaca data yang dikirim client.
   * Hasil pesan disimpan ke `buffer`.

9. **Menutup koneksi**

   ```cpp
   closesocket(clientSocket);
   closesocket(serverSocket);
   WSACleanup();
   ```

   * Socket ditutup dan resource Winsock dibersihkan.

---

### 🔹 client.cpp

1. **Include header dan deklarasi variabel**

   ```cpp
   #include <iostream>
   #include <winsock2.h>
   ```

   * `std::string message = "Hello, World, from client.";` → pesan yang akan dikirim.

2. **Inisialisasi Winsock**

   ```cpp
   if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
       std::cerr << "WSAStartup failed. Error: " << WSAGetLastError() << std::endl;
       return -1;
   }
   ```

   * Sama seperti server, wajib sebelum menggunakan socket.

3. **Membuat socket client**

   ```cpp
   clientSocket = socket(AF_INET, SOCK_STREAM, 0);
   ```

   * `SOCK_STREAM` → koneksi TCP.

4. **Konfigurasi alamat server**

   ```cpp
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = inet_addr("127.0.0.1");
   server.sin_port = htons(9090);
   ```

   * Client akan mencoba terhubung ke **localhost (127.0.0.1)** port **9090**.

5. **Menghubungkan ke server**

   ```cpp
   connect(clientSocket, (sockaddr*)&server, sizeof(server));
   ```

   * Jika berhasil, koneksi TCP terbentuk.

6. **Mengirim data ke server**

   ```cpp
   send(clientSocket, message.c_str(), message.length(), 0);
   ```

   * Mengirim isi pesan string ke server.

7. **Menutup koneksi**

   ```cpp
   closesocket(clientSocket);
   WSACleanup();
   ```

   * Koneksi ditutup dan resource dibersihkan.

## ▶️ Cara Menjalankan

### Kompilasi (MinGW / g++)

```bash
g++ server.cpp -o server.exe -lws2_32
g++ client.cpp -o client.exe -lws2_32
```

> **Catatan:**
> Jika menggunakan **MSVC (Visual Studio)**, tambahkan preprocessor:
>
> ```cpp
> #pragma comment(lib, "ws2_32.lib")
> ```

### Jalankan Program

1. Jalankan server di terminal pertama:

   ```bash
   ./server.exe
   ```
2. Jalankan client di terminal kedua:

   ```bash
   ./client.exe
   ```

## 📝 Hasil Akhir (Output)

### Server

```
Server ready. Wainting connection from port port 9090...
Connected with client: 127.0.0.1
Message from client: Hello, World, from client.
```

### Client

```
Successfull connect to server
Message successfull sended to server
```

# 😉 Terima Kasih 😉