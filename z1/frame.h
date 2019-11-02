#ifndef _FRAME_H
#define _FRAME_H

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <vector>
#include <string>
#include "shapes.h"
#include <cstring>

#define VGA_X 640
#define VGA_Y 480
#define MAX_MMAP_SIZE (VGA_X*VGA_Y*sizeof(unsigned int))

#define BLUE 0x001F
#define GREEN 0x07E0
#define RED 0xF800
#define BLACK 0x0000
#define YELLOW (RED+GREEN)

template <class T> int swap(T *v1, T *v2);
template <class T> inline T saturate(T in, T low, T high);
int get_colour(const std::string c);

class Frame {
public:
  std::string bckg;
  std::vector<LINE_H> lines_h;
  std::vector<LINE_V> lines_v;
  std::vector<RECT> rects;
  int *buffer;

  Frame() : bckg("BLACK"){
#ifdef DEBUG
    buffer = new int[VGA_X*VGA_Y];
    memset(buffer, 0, sizeof(buffer[0]) * VGA_Y * VGA_X);
#else
#endif
  }

  ~Frame(){
    delete []buffer;
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

  int send_frame();
  void dump_ppm(const std::string fn);

};


#endif
