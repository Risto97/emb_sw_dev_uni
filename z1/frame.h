#ifndef _FRAME_H
#define _FRAME_H

#include <algorithm>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>

#include "shapes.h"

#define VGA_X 640
#define VGA_Y 480
#define MAX_MMAP_SIZE (VGA_X * VGA_Y * sizeof(unsigned int))

#define BLUE 0x001F
#define GREEN 0x07E0
#define RED 0xF800
#define BLACK 0x0000
#define YELLOW (RED + GREEN)

template <class T> int swap(T *v1, T *v2);
template <class T> inline T saturate(T in, T low, T high);
int get_colour(const std::string c);

class Frame {
private:
  std::string bckg;
  std::string dev_fn;
  std::vector<LINE_H> lines_h;
  std::vector<LINE_V> lines_v;
  std::vector<RECT> rects;
  int *buffer;
  int fd;

public:
  Frame(std::string fn = "/dev/vga_dma") : bckg("BLACK"), dev_fn(fn) {
#ifdef DEBUG
    buffer = new int[VGA_X * VGA_Y]();
#else
    fd = open(dev_fn.c_str(), O_RDWR | O_NDELAY);
    if (fd < 0) {
      std::cout << "Cannot open " << dev_fn << "\n";
      exit(EXIT_FAILURE);
    } else
      buffer = (int *)mmap(0, MAX_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
                           fd, 0);
#endif
  }

  ~Frame() {
#ifndef DEBUG
    munmap(buffer, MAX_MMAP_SIZE);
    close(fd);
    if (fd < 0)
      std::cout << "Cannot close " << dev_fn << "\n";
#endif
    delete[] buffer;
  }
  void print();

  int add_line_h(int x1, int x2, int y, std::string colour);
  int add_line_v(int x, int y1, int y2, std::string colour);

  int add_rect(int x1, int x2, int y1, int y2, std::string colour);
  void change_background(const std::string b);
  void redraw_shapes();

  void draw_line_h(const LINE_H line);
  void draw_line_v(const LINE_V line);
  void draw_rect(const RECT r);

  void dump_ppm(const std::string fn);
};

#endif
