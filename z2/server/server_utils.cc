#include <iostream>
#include "server_utils.h"

int acquire_sec(int *pids, int pid) {
  for (int i = 0; i < 4; i++) {
    if (pids[i] == 0) {
      pids[i] = pid;
      return i;
    }
  }
  return -1;
}

void print_sec_list(int *pids, int n) {
  std::cout << "Client list: ";
  for (int i = 0; i < n; i++)
    std::cout << pids[i] << " ";
  std::cout << "\n";
}

int release_sec(int *pids, int pid) {
  for (int i = 0; i < 4; i++) {
    if (pids[i] == pid) {
      pids[i] = 0;
      return i;
    }
  }
  return -1;
}

int first_free_sec(int *pids, int n) {
  for (int i = 0; i < n; i++) {
    if (pids[i] == 0)
      return i;
  }
  return -1;
}

bool check_any(const int a[], int n, int val) {
  for (int i = 0; i < n; i++)
    if (a[i] == val)
      return true;
  return false;
}
