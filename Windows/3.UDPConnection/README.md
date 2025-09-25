# 📡 Membuat UDP Socket Connection

## 📃 Deskripsi Projek
Projek ini adalah implementasi sederhana komunikasi **UDP (User Datagram Protocol)** menggunakan **C++ dan Winsock2** di Windows. Berbeda dengan TCP yang bersifat *connection-oriented*, UDP adalah protokol *connectionless* sehingga tidak memerlukan proses *handshake* sebelum mengirim data. Client cukup mengirim pesan ke IP Address dan port tertentu, lalu server langsung menerima tanpa perlu membuka koneksi permanen.  

## 🎯 Tujuan Projek
- Mempelajari dasar penggunaan **UDP Socket API** dengan Winsock2.  
- Memahami perbedaan utama **TCP** dan **UDP** dalam pemrograman jaringan.  
- Mengimplementasikan komunikasi sederhana antara **UDP server** dan **UDP client**.  

## ⚙️ Cara Kerja Program

### 🖥️ server.cpp
Kode server berfungsi menerima pesan dari client lalu membalas pesan tersebut.  
1. **Include header dan deklarasi variabel**
   ```cpp
   #include <iostream>
   #include <array>
   #include <winsock2.h>
   ```
   
   ```cpp
   int clientAddrSize, byteReceived;
   std::string reply;
   std::array<char, 1024> buffer;
   WSADATA wsa;
   SOCKET udpSocket;
   sockaddr_in serverSocket, clientSocket;
   ```

* `buffer` digunakan menampung data yang diterima.
* `udpSocket` adalah socket utama untuk komunikasi.
* `serverSocket` menyimpan alamat server, `clientSocket` menyimpan alamat client yang terhubung.
* `reply` adalah pesan balasan yang akan dikirim ke client.

2. **Inisialisasi Winsock**

   ```cpp
   if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { ... }
   ```

   Wajib dilakukan sebelum menggunakan fungsi socket di Windows.

3. **Membuat socket UDP**

   ```cpp
   udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   ```

   * `AF_INET` → IPv4
   * `SOCK_DGRAM` → Datagram (UDP)
   * `IPPROTO_UDP` → Protokol UDP

4. **Konfigurasi IP Address server**

   ```cpp
   serverSocket.sin_family = AF_INET;
   serverSocket.sin_port = htons(9090);
   serverSocket.sin_addr.s_addr = INADDR_ANY;
   ```

   Server akan mendengarkan di **semua IP Address lokal** pada **port 9090**.

5. **Bind socket**

   ```cpp
   bind(udpSocket, (SOCKADDR*)&serverSocket, sizeof(serverSocket));
   ```

   Mengikat socket dengan IP Address dan port yang telah dikonfigurasi.

6. **Loop menerima data dari client**

   ```cpp
   byteReceived = recvfrom(udpSocket, buffer.data(), buffer.size() - 1, 0,
                           (SOCKADDR*)&clientSocket, &clientAddrSize);
   ```

   * `recvfrom` menerima data dari client sekaligus mengetahui IP Address pengirim.
   * Data disimpan ke `buffer`.

7. **Tampilkan pesan client**

   ```cpp
   buffer[byteReceived] = '\0';
   std::cout << "Message from client: " << buffer.data() << std::endl;
   ```

8. **Kirim balasan ke client**

   ```cpp
   sendto(udpSocket, reply.c_str(), reply.length(), 0,
          (SOCKADDR*)&clientSocket, clientAddrSize);
   ```

   Balasan dikirim langsung ke IP Address client yang didapat dari `recvfrom`.

---

### 💻 client.cpp

Kode client berfungsi mengirim pesan ke server dan menunggu balasan.

1. **Include header dan deklarasi variabel**
   ```cpp
   #include <iostream>
   #include <array>
   #include <winsock2.h>
   ```

   ```cpp
   int serverAddrSize, bytesReceived;
   std::string message;
   std::array<char, 1024> buffer;
   WSADATA wsa;
   SOCKET udpClient;
   sockaddr_in serverSocket;
   ```

   * `udpClient` adalah socket untuk mengirim/terima pesan.
   * `serverSocket` menyimpan alamat server tujuan (`127.0.0.1:9090`).

2. **Inisialisasi Winsock**

   ```cpp
   WSAStartup(MAKEWORD(2, 2), &wsa);
   ```

3. **Membuat socket UDP**

   ```cpp
   udpClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   ```

4. **Konfigurasi IP Address server tujuan**

   ```cpp
   serverSocket.sin_family = AF_INET;
   serverSocket.sin_port = htons(9090);
   serverSocket.sin_addr.s_addr = inet_addr("127.0.0.1");
   ```

   Artinya client akan mengirim pesan ke server IP Address lokal (`localhost`) di port 9090.

5. **Kirim pesan ke server**

   ```cpp
   sendto(udpClient, message.c_str(), message.length(), 0,
          (SOCKADDR*)&serverSocket, serverAddrSize);
   ```

6. **Menunggu balasan dari server**

   ```cpp
   bytesReceived = recvfrom(udpClient, buffer.data(), buffer.size() - 1, 0,
                            (SOCKADDR*)&serverSocket, &serverAddrSize);
   ```

   * Client akan menerima balasan dari server.
   * Data hasil balasan disimpan ke `buffer`.

7. **Tampilkan balasan dari server**

   ```cpp
   std::cout << "Reply from server: " << buffer.data() << std::endl;
   ```

## ▶️ Cara Menjalankan

1. Compile dengan MinGW:

   ```bash
   g++ server.cpp -o server.exe -lws2_32
   g++ client.cpp -o client.exe -lws2_32
   ```
2. Jalankan server:

   ```bash
   ./server.exe
   ```
3. Jalankan client (di terminal lain):

   ```bash
   ./client.exe
   ```

## 📝 Hasil Akhir (Output)

### Server

```bash
UDP Server active in port 9090...
Message from client: Hello from client UDP
```

### Client

```bash
Message send to server.
Reply from server: Message received!
```

# 😉 Terima Kasih 😉