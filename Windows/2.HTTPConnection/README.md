# 🌐 Membuat HTTP Socket Connection

## 📃 Deskripsi Projek
Proyek ini adalah implementasi sederhana **HTTP Server** menggunakan **C++** dan **Winsock2 API** di Windows.  
Server ini menerima koneksi HTTP pada port **9090**, lalu mengirim respon **HTTP/1.1 200 OK** dengan konten sederhana berupa HTML.  

## 🎯 Tujuan Projek
- Memahami cara membuat server HTTP dasar dengan **TCP socket**.
- Mempelajari cara menangani request dan memberikan response sesuai format **HTTP**.
- Menunjukkan bahwa socket programming bisa menjadi dasar dari **Web Server** dan **REST API** sederhana.

## ⚙️ Cara Kerja (server.cpp)
1. **Include header dan deklarasi variabel**
   ```cpp
   #include <iostream>
   #include <array>
   #include <winsock2.h>
   ```

* `buffer` digunakan untuk menampung request HTTP dari client (browser/curl).
* `content` menyimpan isi HTML.
* `response` menyimpan HTTP response lengkap (header + body).

2. **Membuat response HTTP sederhana**

   ```cpp
   content = "<h1>Hello World!</h1>";
   response = "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/html\r\n"
              "Content-Length: " + std::to_string(content.length()) + "\r\n"
              "\r\n" + content;
   ```

   * `Content-Type: text/html` → tipe konten HTML.
   * `Content-Length` dihitung dari panjang string HTML.

3. **Inisialisasi Winsock**

   ```cpp
   WSAStartup(MAKEWORD(2, 2), &wsa);
   ```

   * Sama seperti sebelumnya, wajib sebelum menggunakan socket di Windows.

4. **Membuat socket server**

   ```cpp
   serverSocket = socket(AF_INET, SOCK_STREAM, 0);
   ```

   * `SOCK_STREAM` → TCP digunakan karena HTTP berbasis TCP.

5. **Konfigurasi IP Address server**

   ```cpp
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port = htons(9090);
   ```

   * Server akan berjalan di semua interface (IP Address lokal) pada port **9090**.

6. **Bind dan listen**

   ```cpp
   bind(serverSocket, (sockaddr*)&server, sizeof(server));
   listen(serverSocket, SOMAXCONN);
   ```

   * Menghubungkan socket dengan port 9090 dan mulai mendengarkan request.

7. **Loop untuk menerima client**

   ```cpp
   while (true) {
       clientSocket = accept(serverSocket, (sockaddr*)&client, &clientSize);
       ...
   }
   ```

   * Server terus berjalan (infinite loop) dan menerima koneksi HTTP dari client.

8. **Menerima request HTTP dan mengirim response**

   ```cpp
   bytesReceived = recv(clientSocket, buffer.data(), buffer.size() - 1, 0);
   if (bytesReceived > 0) {
       buffer[bytesReceived] = '\0';
       send(clientSocket, response.c_str(), response.length(), 0);
       std::cout << "request HTTP:\n\n" << buffer.data() << std::endl;
   }
   ```

   * `recv()` menangkap HTTP request dari client.
   * `send()` mengirim response sederhana.
   * Server juga menampilkan request HTTP ke terminal.

9. **Menutup koneksi client**

   ```cpp
   closesocket(clientSocket);
   ```

   * Setelah mengirim response, koneksi ditutup.

## ▶️ Cara Menjalankan

### Kompilasi (MinGW / g++)

```bash
g++ server.cpp -o http_server.exe -lws2_32
```

> **Catatan:**
> Jika menggunakan MSVC (Visual Studio), tambahkan:
>
> ```cpp
> #pragma comment(lib, "ws2_32.lib")
> ```

### Jalankan Program

```bash
./http_server.exe
```

Server akan berjalan di port **9090**.

## 📝 Hasil Akhir (Output)

### Terminal Server

```bash
Server running in port 9090, Waiting connection...
Connected with client: 127.0.0.1
request HTTP:

GET / HTTP/1.1
Host: localhost:9090
User-Agent: curl/8.0.1
Accept: */*
```

### Client terminal (curl)

```bash
curl http://127.0.0.1:9090/ -i
````

Output:

```bash
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 20

<h1>Hello World!</h1>
```

### Client browser

Buka `http://127.0.0.1:9090` → akan muncul tulisan **Hello World!** di halaman web.

# 😉 Terima Kasih 😉