#include "frame.h"
#include <asm-generic/errno.h>
#include <asm-generic/socket.h>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT 8080
#define MAX 256
#define STRIDE 10

std::string sock_read(int sockfd) {
  char buff[MAX];
  bzero(buff, MAX);
  read(sockfd, buff, sizeof(buff));
  std::cout << "From Client: " << buff << "\n";
  std::string message(buff);

  return message;
}

int set_pid(int *pids, int pid) {
  for (int i = 0; i < 4; i++) {
    if (pids[i] == 0) {
      pids[i] = pid;
      return i;
    }
  }
  return -1;
}

int release_pid(int *pids, int pid) {
  for (int i = 0; i < 4; i++) {
    if (pids[i] == pid) {
      pids[i] = 0;
      return i;
    }
  }
  return -1;
}

int first_free_pid(int *pids, int n) {
  for (int i = 0; i < n; i++) {
    if (pids[i] == 0)
      return i;
  }
  return -1;
}

inline bool check_any(const int a[], int n, int val) {
  for (int i = 0; i < n; i++)
    if (a[i] == val)
      return true;
  return false;
}

void clear_section(Frame *frame, int clinum) {
  if (clinum == 0) {
    frame->add_rect(0, VGA_X / 2 - 1, 0, VGA_Y / 2 - 1, "BLACK");
  } else if (clinum == 1) {
    frame->add_rect(VGA_X / 2 + 1, VGA_X - 1, 0, VGA_Y / 2 - 1, "BLACK");
  } else if (clinum == 2) {
    frame->add_rect(0, VGA_X / 2 - 1, VGA_Y / 2 + 1, VGA_Y - 1, "BLACK");
  } else if (clinum == 3) {
    frame->add_rect(VGA_X / 2 + 1, VGA_X - 1, VGA_Y / 2 + 1, VGA_Y - 1,
                    "BLACK");
  }
}

void draw_rect(Frame *frame, int clinum, int x_off, int y_off) {
  if (clinum == 0) {
    frame->add_rect(VGA_X / 4 - 20 + x_off, VGA_X / 4 + 20 + x_off,
                    VGA_Y / 4 - 20 + y_off, VGA_Y / 4 + 20 + y_off, "RED");
  } else if (clinum == 1) {
    frame->add_rect((3 * VGA_X / 4) - 20 + x_off, (3 * VGA_X / 4) + 20 + x_off,
                    VGA_Y / 4 - 20 + y_off, VGA_Y / 4 + 20 + y_off, "RED");
  } else if (clinum == 2) {
    frame->add_rect(VGA_X / 4 - 20 + x_off, VGA_X / 4 + 20 + x_off,
                    (3 * VGA_Y / 4) - 20 + y_off, (3 * VGA_Y / 4) + 20 + y_off,
                    "RED");
  } else if (clinum == 3) {
    frame->add_rect((3 * VGA_X / 4) - 20 + x_off, (3 * VGA_X / 4) + 20 + x_off,
                    (3 * VGA_Y / 4) - 20 + y_off, (3 * VGA_Y / 4) + 20 + y_off,
                    "RED");
  }
}

void saturate_section(int *x_off, int *y_off) {
  if (*x_off < -VGA_X / 4 + 21)
    *x_off = -VGA_X / 4 + 21;
  if (*x_off > VGA_X / 4 - 21)
    *x_off = VGA_X / 4 - 21;

  if (*y_off < -VGA_Y / 4 + 21)
    *y_off = -VGA_Y / 4 + 21;
  if (*y_off > VGA_Y / 4 - 21)
    *y_off = VGA_Y / 4 - 21;
}

void move_rect(Frame *frame, int clinum, std::string cmd, int *x_off,
               int *y_off) {
  clear_section(frame, clinum);
  if (cmd == "w")
    *y_off = *y_off - STRIDE;
  else if (cmd == "s")
    *y_off = *y_off + STRIDE;
  else if (cmd == "d")
    *x_off = *x_off + STRIDE;
  else if (cmd == "a")
    *x_off = *x_off - STRIDE;

  saturate_section(x_off, y_off);
  draw_rect(frame, clinum, (*x_off), (*y_off));
}

int main() {
  int sockfd, newsockfd, clilen;
  struct sockaddr_in serv_addr, cli_addr;
  int pids[4] = {0};
  Frame frame;
  frame.change_background("BLACK");
  frame.add_line_v(VGA_X / 2 - 1, 0, VGA_Y - 1, "BLUE");
  frame.add_line_h(0, VGA_X - 1, VGA_Y / 2 - 1, "BLUE");

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("ERROR opening socket");
    exit(1);
  }
  bzero(&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("ERROR on binding");
    exit(1);
  }
  std::cout << "Server Started, waiting for clients....\n";

  if ((listen(sockfd, 4)) != 0) {
    std::cout << "Listen Failed...\n";
    exit(1);
  } else
    std::cout << "Server listening...\n";

  clilen = sizeof(cli_addr);

  while (1) {
    if (check_any(pids, 4, 0)) {
      int flags = fcntl(sockfd, F_GETFL, 0);
      fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
      newsockfd =
          accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);

      if (newsockfd == -1) {
        if (errno == EWOULDBLOCK) {
          usleep(10000);
        } else {
          perror("error when accepting");
          exit(1);
        }
      } else {
        int flags = fcntl(newsockfd, F_GETFL, 0);
        fcntl(sockfd, F_SETFL, flags & (~O_NONBLOCK));
        std::cout << "Server accepted the client...\n";

        int first_free = first_free_pid(pids, 4);
        int pid = fork();
        if (pid < 0) {
          perror("ERROR on fork");
          exit(1);
        }
        if (pid == 0) {
          int x_off = 0;
          int y_off = 0;
          clear_section(&frame, first_free);
          draw_rect(&frame, first_free, 0, 0);
          close(sockfd);
          std::string msg;
          do {
            msg = sock_read(newsockfd);
            move_rect(&frame, first_free, msg, &x_off, &y_off);
          } while (msg != "q");
          clear_section(&frame, first_free);

          exit(0);
        } else {
          close(newsockfd);
          std::cout << "PID: " << pid << std::endl;
          set_pid(pids, pid);
          std::cout << "PID_list: ";
          for (auto x : pids)
            std::cout << x << " ";
          std::cout << "\n";
        }
      }
    }

    for (int a = 0; a < 4; a++) {
      if (pids[a] != 0) {
        int status;
        if (waitpid(pids[a], &status, WNOHANG)) {
          std::cout << "Process finished\n";
          release_pid(pids, pids[a]);

          std::cout << "PID_list: ";
          for (auto x : pids)
            std::cout << x << " ";
          std::cout << "\n";
        }
      }
    }
  }
  close(sockfd);
  return 0;
}
