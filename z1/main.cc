#include <fstream>
#include <iostream>
#include <regex>
#include <stdlib.h>
#include <string>
#include <vector>

#include "frame.h"

int parse_line(const std::string line, Frame *fb) {
  std::regex command_reg("^\\s*(BCKG|LINE_H|LINE_V|RECT)\\s*:");
  std::regex line_reg(
      "\\s*(?::\\s*)(\\d*)\\s*,\\s*(\\d*)\\s*,\\s(\\d*)\\s*;\\s*(\\w*)$");
  std::smatch m;

  if (!std::regex_search(line, m, command_reg)) {
    std::cout << "No match found for line: " << '"' << line << '"' << std::endl;
    return -1;
  }

  if (m[1].compare("BCKG") == 0) {
    std::regex bckg_reg("\\b\\w*$");
    std::regex_search(line, m, bckg_reg);

    fb->change_background(m[0]);
    return 1;
  }
  if (m[1].compare("LINE_H") == 0) {
    if (!std::regex_search(line, m, line_reg)) {
      return -1;
    }

    fb->add_line_h(std::stoi(m[1]), std::stoi(m[2]), std::stoi(m[3]), m[4]);
    return 2;
  }

  if (m[1].compare("LINE_V") == 0) {
    if (!std::regex_search(line, m, line_reg)) {
      return -1;
    }

    fb->add_line_v(std::stoi(m[1]), std::stoi(m[2]), std::stoi(m[3]), m[4]);
    return 3;
  }

  if (m[1].compare("RECT") == 0) {
    std::regex rect_reg("\\s*(?::\\s*)(\\d*)\\s*,\\s*(\\d*)\\s*,\\s*(\\d*)\\s*,"
                        "\\s*(\\d*)\\s*;\\s*(\\w*)$");
    if (!std::regex_search(line, m, rect_reg)) {
      return -1;
    }
    fb->add_rect(std::stoi(m[1]), std::stoi(m[2]), std::stoi(m[3]),
                 std::stoi(m[4]), m[5]);
    return 4;
  }

  return -1;
}

int parse_file(const std::string fn, Frame *frame) {
  std::ifstream f;
  std::string line;
  f.open(fn, std::ifstream::in);
  if (f.fail()) {
    std::cout << "Failed to read file: " << '"' << fn << '"' << std::endl;
    return -1;
  }

  while (std::getline(f, line))
    parse_line(line, frame);

  f.close();
  return 0;
}

int main(int argc, char *argv[]) {
  Frame frame;
  std::string fn;
  if (argc < 2) {
    std::cout << "Please enter config file path\n";
    return -1;
  } else {
    fn = argv[1];
  }

  parse_file(fn, &frame);

#ifdef DEBUG
  frame.print();
  frame.dump_ppm("picture.ppm");
  system("feh picture.ppm");
#endif

  return 0;
}
