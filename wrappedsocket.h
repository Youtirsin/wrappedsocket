#ifndef WRAPPED_SOCKET
#define WRAPPED_SOCKET

#include <arpa/inet.h>
#include <unistd.h>

#include <cassert>
#include <iostream>
#include <memory>

// use socket just like python
class Socket {
 private:
  // serverside client
  struct Client {
   public:
    Client(int fd_) : cfd(fd_) {}

    // send message to the client
    void send(std::string msg);

    // await message from the client
    std::string recv(int MAX_BUF_SIZE);
    
    // close fd
    void close_();
   private:
    int cfd;
  };

 public:
  // default
  Socket() = default;

  // use serverside socket
  void setAsServer();

  // use clientside socket
  void setAsClient();

  // ----------------- serverside ----------------------
  // bind address, ip and port
  void bind_(int port);

  // enable listening, max_num at most
  void listen_(int max_num);

  // await a connection, when a connection arrives,
  // make an Client object to return
  std::shared_ptr<Socket::Client> accept_();

  // ----------------- clientside ----------------------
  // use socket as a client
  // connect the target address
  void connect_(const std::string &ip, int port);

  // send message to the server
  void send(std::string msg);

  // await message from the server
  std::string recv(int MAX_BUF_SIZE);

  // close fd
  void close_();

 private:
  // socket fd
  int fd;
  // server used as server or client
  // 0 for default
  // 1 for server
  // 2 for client
  int socketType;
};

#endif