#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>

#include "sock_utils.h"

char getch() {
  char buf = 0;
  struct termios old = {0};
  if (tcgetattr(0, &old) < 0)
    perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0)
    perror("tcsetattr ICANON");
  if (read(0, &buf, 1) < 0)
    perror("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0)
    perror("tcsetattr ~ICANON");
  return (buf);
}

int main(int argc, char **argv) {
  std::string serv_ip;
  if (argc < 2) {
    std::cerr << "Please enter Server IP addr\n";
    return -1;
  } else {
    serv_ip = argv[1];
  }

  int sockfd = 0;
  struct sockaddr_in serv_addr;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cerr << "\n Error: Could not create socket \n";
    return 1;
  } else{
    std::cout << "Socket Succesfully created...\n";
  }

  bzero(&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(serv_ip.c_str());
  serv_addr.sin_port = htons(PORT);

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cerr << "\nError : Connection Failed \n";
    return 1;
  } else{
    std::cout << "\nConnected to server " << serv_ip << ":" << PORT << "\n";
    std::string message = sock_read(sockfd);
    std::cout << message << "\n";
  }


  char key;
  std::string send_str;
  while (1) {
    key = getch();
    send_str = key;
    if (key == 'w' || key == 'a' || key == 's' || key == 'd' || key == 'q')
      sock_write(sockfd, send_str);

    if (key == 'q') {
      close(sockfd);
      return 0;
    }
  }
}
