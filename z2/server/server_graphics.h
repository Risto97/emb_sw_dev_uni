#ifndef __SERVER_GRAPHICS_H
#define __SERVER_GRAPHICS_H

#include "frame.h"
#include <string>

void init_screen(Frame *frame);
void clear_section(Frame *frame, int clinum);
void draw_rect(Frame *frame, int clinum, int x_off, int y_off);
void saturate_section(int *x_off, int *y_off);
void move_rect(Frame *frame, int clinum, std::string cmd, int *x_off,
               int *y_off);
void saturate_section(int *x_off, int *y_off);
void move_rect(Frame *frame, int clinum, std::string cmd, int *x_off,
               int *y_off);
std::string sec_n_to_str(int n);

#endif
