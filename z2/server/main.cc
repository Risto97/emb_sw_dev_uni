#include <arpa/inet.h>
#ifdef DEBUG
#include <asm-generic/errno.h>
#include <asm-generic/socket.h>
#include <bits/types/siginfo_t.h>
#endif
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

#include "frame.h"
#include "server_graphics.h"
#include "server_utils.h"
#include "sock_utils.h"

int main() {
  int sockfd, newsockfd, clilen;
  struct sockaddr_in serv_addr, cli_addr;
  int pids[4] = {0};

  Frame frame;
  init_screen(&frame);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "ERROR while opening socket\n";
    exit(1);
  }
  bzero(&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cerr << "ERROR while binding\n";
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
    /* If any slot is free check for new connections */
    if (check_any(pids, 4, 0)) {
      /* Configure socket to be NONBLOCKING */
      int flags = fcntl(sockfd, F_GETFL, 0);
      fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
      newsockfd =
          accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);

      if (newsockfd == -1) {
        /* If socket failed because there were no client requests, sleep 10ms */
        if (errno == EWOULDBLOCK) {
          usleep(10000);
        }
        /* Otherwise accept() failed with an error */
        else {
          std::cerr << "ERROR while accepting\n";
          exit(1);
        }
      }
      /* If accept() was succesfull, new client is connected */
      else {
        std::string cli_ip_str = inet_ntoa(cli_addr.sin_addr);
        /* Revert socket config to BLOCKING */
        int flags = fcntl(newsockfd, F_GETFL, 0);
        fcntl(sockfd, F_SETFL, flags & (~O_NONBLOCK));
        int first_free = first_free_sec(pids, 4);
        std::cout << "Client with IP: " << cli_ip_str
                  << " connected to section " << sec_n_to_str(first_free)
                  << "\n";
        int pid = fork();
        if (pid < 0) {
          std::cerr << "ERROR while forking\n";
          exit(1);
        }
        /* Child process serves the client
           Child process is terminating after if block
         */
        if (pid == 0) {
          int x_off = 0;
          int y_off = 0;
          clear_section(&frame, first_free);
          draw_rect(&frame, first_free, 0, 0);
          close(sockfd);
          sock_write(newsockfd, "Controlling: " + sec_n_to_str(first_free));
          std::string msg;
          do {
            msg = sock_read(newsockfd);
            move_rect(&frame, first_free, msg, &x_off, &y_off);
          } while (msg != "q" && msg != "");
          clear_section(&frame, first_free);
          close(newsockfd);

          exit(0);
        }
        /* Parent process add connected client to list
           Occupy free position in client list 'pids'
         */
        else {
          close(newsockfd);
          acquire_sec(pids, pid);
          print_sec_list(pids, 4);
        }
      }
    }

    /* Parrent process
       Checks if any of the child processes has terminated (client disconnected)
       On client disconnect release entry in PID list
     */
    for (int a = 0; a < 4; a++) {
      if (pids[a] != 0) {
        int status;
        if (waitpid(pids[a], &status, WNOHANG)) {
          std::cout << "Client disconnected\n";
          release_sec(pids, pids[a]);
          print_sec_list(pids, 4);
        }
      }
    }
  }
  close(sockfd);
  return 0;
}
