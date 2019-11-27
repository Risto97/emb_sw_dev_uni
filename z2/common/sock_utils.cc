#include "sock_utils.h"
#include <unistd.h>
#include <string.h>

void sock_write(int sockfd, std::string s) {
  write(sockfd, s.c_str(), s.size());
}

std::string sock_read(int sockfd) {
  const int MAX = 256;
  char buff[MAX];
  bzero(buff, MAX);
  read(sockfd, buff, sizeof(buff));
  std::string message(buff);

  return message;
}
