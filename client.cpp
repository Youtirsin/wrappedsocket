#include <iostream>

#include "wrappedsocket.h"

// test client-side socket of wrappedsocket
int main() {
  Socket client;

  client.connect_("127.0.0.1", 10010);

  auto msg = client.recv(1024);

  std::cout << msg << '\n';

  client.close_();

  return 0;
}
