#ifndef __SOCK_UTILS_H
#define __SOCK_UTILS_H

#include <string>

#define PORT 8080

void sock_write(int sockfd, std::string s);
std::string sock_read(int sockfd);

#endif
