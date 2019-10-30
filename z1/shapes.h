#ifndef _SHAPES_H
#define _SHAPES_H

#include <iostream>

struct LINE_H {
  int x1;
  int x2;
  int y;
  std::string colour;

  void print(){
    std::cout << "LINE_H: " << x1 << ", " << x2 << ", "
              << y << ", " << colour << std::endl;
  }
};

struct LINE_V {
  int x;
  int y1;
  int y2;
  std::string colour;

  void print(){
    std::cout << "LINE_V: " << x << ", " << y1 << ", "
              << y2 << ", " << colour << std::endl;
  }
};

struct RECT {
  int x1;
  int x2;
  int y1;
  int y2;
  std::string colour;

  void print(){
    std::cout << "RECT: " << x1 << ", " << x2 << ", " << y1
              << ", " << y2 << ", " << colour << std::endl;
  }
};


#endif
