#ifndef __SERVER_UTILS_H
#define __SERVER_UTILS_H

int acquire_sec(int *pids, int pid);
void print_sec_list(int *pids, int n);
int release_sec(int *pids, int pid);
int first_free_sec(int *pids, int n);
bool check_any(const int a[], int n, int val);

#endif
