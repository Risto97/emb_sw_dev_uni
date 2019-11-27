
#include "server_graphics.h"

#define STRIDE 10

void init_screen(Frame *frame){
  frame->change_background("BLACK");
  frame->add_line_v(VGA_X / 2 - 1, 0, VGA_Y - 1, "BLUE");
  frame->add_line_h(0, VGA_X - 1, VGA_Y / 2 - 1, "BLUE");
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

std::string sec_n_to_str(int n) {
  if(n == 0)
    return "Top left";
  if(n == 1)
    return "Top right";
  if(n == 2)
    return "Bottom left";
  if(n == 3)
    return "Bottom right";

  return "Undefined";
}
