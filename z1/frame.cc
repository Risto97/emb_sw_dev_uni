#include "frame.h"
#include <algorithm>
#include <fstream>

template <class T> int swap(T *v1, T *v2) {
  if (*v1 > *v2) {
    T tmp = *v1;
    *v1 = *v2;
    *v2 = tmp;
    return 1;
  }
  return 0;
}

template <class T> inline T saturate(T in, T low, T high) {
  if (in < low)
    return low;
  if (in > high)
    return high;
  return in;
}

int get_colour(const std::string c) {
  if (c == "BLUE")
    return BLUE;
  if (c == "GREEN")
    return GREEN;
  if (c == "RED")
    return RED;
  if (c == "BLACK")
    return BLACK;
  if (c == "YELLOW")
    return YELLOW;

  return BLACK;
}

void Frame::print() {
  std::cout << "BCKG: " << bckg << std::endl;
  for (auto x : lines_h)
    x.print();
  for (auto x : lines_v)
    x.print();
  for (auto x : rects)
    x.print();
}

int Frame::add_line_h(int x1, int x2, int y, std::string colour) {
  x1 = saturate<int>(x1, 0, VGA_X - 1);
  x2 = saturate<int>(x2, 0, VGA_X - 1);
  y = saturate<int>(y, 0, VGA_Y - 1);
  swap<int>(&x1, &x2);

  LINE_H line;
  line.x1 = x1;
  line.x2 = x2;
  line.y = y;
  line.colour = colour;
  lines_h.push_back(line);
  draw_line_h(line);
}

int Frame::add_line_v(int x, int y1, int y2, std::string colour) {
  x = saturate<int>(x, 0, VGA_X - 1);
  y1 = saturate<int>(y1, 0, VGA_Y - 1);
  y2 = saturate<int>(y2, 0, VGA_Y - 1);
  swap<int>(&y1, &y2);

  LINE_V line;
  line.x = x;
  line.y1 = y1;
  line.y2 = y2;
  line.colour = colour;
  lines_v.push_back(line);
  draw_line_v(line);
}

int Frame::add_rect(int x1, int x2, int y1, int y2, std::string colour) {
  x1 = saturate<int>(x1, 0, VGA_X - 1);
  x2 = saturate<int>(x2, 0, VGA_X - 1);
  y1 = saturate<int>(y1, 0, VGA_Y - 1);
  y2 = saturate<int>(y2, 0, VGA_Y - 1);
  swap<int>(&x1, &x2);
  swap<int>(&y1, &y2);

  RECT r;
  r.x1 = x1;
  r.x2 = x2;
  r.y1 = y1;
  r.y2 = y2;
  r.colour = colour;
  rects.push_back(r);
  draw_rect(r);
}

void Frame::change_background(const std::string b) {
  bckg = b;
  int colour = get_colour(b);
  std::fill_n(buffer, VGA_Y * VGA_X, colour);
  redraw_shapes();
}

void Frame::redraw_shapes() {
  for (auto x : lines_h)
    draw_line_h(x);
  for (auto x : lines_v)
    draw_line_v(x);
  for (auto x : rects)
    draw_rect(x);
}

void Frame::draw_line_h(const LINE_H line) {
  int colour = get_colour(line.colour);
  for (int x = line.x1; x < line.x2; x++) {
    buffer[(line.y * VGA_X) + x] = colour;
  }
}

void Frame::draw_line_v(const LINE_V line) {
  int colour = get_colour(line.colour);
  for (int y = line.y1; y < line.y2; y++) {
    buffer[(y * VGA_X) + line.x] = colour;
  }
}

void Frame::draw_rect(const RECT r) {
  int colour = get_colour(r.colour);
  for (int y = r.y1; y < r.y2; y++) {
    for (int x = r.x1; x < r.x2; x++) {
      buffer[(y * VGA_X) + x] = colour;
    }
  }
}

void Frame::dump_ppm(const std::string fn) {
  std::ofstream img(fn);
  img << "P3" << std::endl;
  img << VGA_X << " " << VGA_Y << std::endl;
  img << "255" << std::endl;
  int r, g, b;

  for (int y = 0; y < VGA_Y; y++) {
    for (int x = 0; x < VGA_X; x++) {

      r = 0;
      g = 0;
      b = 0;
      if (buffer[(VGA_X * y) + x] == RED)
        r = 255;
      if (buffer[(VGA_X * y) + x] == BLUE)
        b = 255;
      if (buffer[(VGA_X * y) + x] == GREEN)
        g = 255;
      if (buffer[(VGA_X * y) + x] == YELLOW) {
        r = 255;
        g = 255;
      }
      img << r << " " << g << " " << b << std::endl;
    }
  }
}
