#include "wrappedsocket.h"

#include <arpa/inet.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>

void Socket::setAsServer() {
  assert(socketType != 2 && "socket already used as a client\n");
  socketType = 1;
}

void Socket::setAsClient() {
  assert(socketType != 1 && "socket already used as a server\n");
  socketType = 2;
}

// ----------------- serverside ----------------------
void Socket::bind_(int port) {
  setAsServer();

  fd = socket(AF_INET, SOCK_STREAM, 0);

  assert(fd >= 0 && "error creating socket.\n");

  struct sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port = htons(port);
  sockaddr.sin_addr.s_addr = INADDR_ANY;

  int res = bind(fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
  assert(res >= 0 && "error binding address.\n");
}

void Socket::listen_(int max_num) {
  setAsServer();

  int res = listen(fd, max_num);
  assert(res >= 0 && "error listening.\n");
}

std::shared_ptr<Socket::Client> Socket::accept_() {
  setAsServer();

  struct sockaddr_in cliaddr;
  socklen_t clilen = sizeof(cliaddr);
  char ip[24] = {0};

  int cfd = accept(fd, (struct sockaddr *)&cliaddr, &clilen);
  assert(cfd != -1 && "error accepting client.\n");

  auto res = std::make_shared<Client>(
      cfd, inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)),
      ntohs(cliaddr.sin_port));
  return res;
}

// ---- serverside client ----
void Socket::Client::send(std::string msg) {
  write(cfd, msg.c_str(), msg.size());
}

std::string Socket::Client::recv(int MAX_BUF_SIZE) {
  char buffer[MAX_BUF_SIZE];
  memset(buffer, 0, MAX_BUF_SIZE);

  int len = read(cfd, buffer, MAX_BUF_SIZE);

  assert(len >= 0 && "receiving error.");

  // client requests disconnection
  if (len == 0) {
    return "";
  }

  return buffer;
}

// close fd
void Socket::Client::close_() { close(cfd); }

// ----------------- clientside ----------------------
void Socket::connect_(const std::string &ip, int port) {
  setAsClient();

  fd = socket(AF_INET, SOCK_STREAM, 0);

  assert(fd >= 0 && "error creating socket.");

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr);

  assert(connect(fd, (struct sockaddr *)&addr, sizeof(addr)) != -1 &&
         "error connecting.");
}

void Socket::send(std::string msg) {
  setAsClient();

  write(fd, msg.c_str(), msg.size());
}

std::string Socket::recv(int MAX_BUF_SIZE) {
  setAsClient();

  char buffer[MAX_BUF_SIZE];
  memset(buffer, 0, MAX_BUF_SIZE);

  int len = read(fd, buffer, MAX_BUF_SIZE);

  assert(len >= 0 && "receiving error.");

  // client requests disconnection
  if (len == 0) {
    return "";
  }

  return buffer;
}

void Socket::close_() { close(fd); }