#include <unordered_map>
#include <string>

using std::string;

std::unordered_map<string, string> word_map = {
  {"Бога", "Науку"},
  {"Богу","Науке"},
  {"Богом", "Наукой"},
  {"Боге", "Науке"},
  {"Бог", "Наука"},
};