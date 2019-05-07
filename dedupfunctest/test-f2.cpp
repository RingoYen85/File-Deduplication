
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <fstream>
#include <functional>  // for hash
#include <iostream>
#include <list>  // for list
#include <sstream>
#include <string>
#include <utility>  // for pair
#include <vector>   // for vector

void dirwork(const char * dir_name) {
  DIR * dir;              // pointer to directory opened
  struct dirent * entry;  // for readdir (content of directory)
  struct stat sb;
  char current[] = ".";
  char parent[] = "..";

  // open directory
  dir = opendir(dir_name);

  if (dir == NULL) {
    std::cout << "Directory not here" << std::endl;
  }

  while ((entry = readdir(dir)) != NULL) {
    std::cout << entry->d_name << std::endl;
    //std::cout << entry->d_type << std::endl;
    stat(entry->d_name, &sb);
    if (S_ISREG(sb.st_mode)) {
      //std::cout << S_ISDIR(sb.st_mode) << std::endl;
      std::cout << entry->d_name << S_ISREG(sb.st_mode) << std::endl;
    }

    else if ((std::string(current) != std::string(entry->d_name)) &&
             (std::string(parent) != std::string(entry->d_name)) && (S_ISDIR(sb.st_mode))) {
    }
  }
}
void addstr(const std::string & str1, const std::string & str2) {
  std::string s3 = str1 + str2;
  std::cout << s3 << std::endl;
}

void fileconv(const char * filename) {
  std::ifstream input;
  input.open(filename);
  std::stringstream buffer;
  buffer << input.rdbuf();
  std::string lines = buffer.str();
  std::cout << lines << std::endl;
}

int main() {
  /* 
  addstr("this is a ", "test");

  std::vector<int> int_list{0, 1, 2, 3, 4};
  for (size_t i = 0; i < int_list.size(); i++) {
    //  std::cout << int_list[i] << std::endl;
  }

  std::cout << "#!/bin/bash" << std::endl;

  std::string s1 = "hello";
  std::size_t hash_s1 = std::hash<std::string>{}(s1);

  std::cout << hash_s1 << std::endl;
  */

  dirwork("/home/rwy2/ece551/098_dedup/dedupfunctest");

  /*
  std::vector<std::list<std::pair<int, int> > > table(5);
  int key1 = 10;
  int value1 = 200;

  int key2 = 50;
  int value2 = 70;

  std::pair<int, int> pair1;
  pair1 = std::make_pair(key1, value1);

  std::pair<int, int> pair2;
  pair2 = std::make_pair(key2, value2);

  table[0].push_front(pair1);
  table[0].push_front(pair2);
  //std::list<int> :: iterator it;
  for (std::list<std::pair<int, int> >::iterator i = table[0].begin() + 1; i != table[0].end();
       i++) {
    std::cout << i->first << " " << i->second << std::endl;
  }

  */
  // fileconv("text1.txt");

  /*
  char test1[] = "hello";
  char test2[] = "hello";
  char test3[] = "bye";
  if (std::string(test1) == (test2)) {
    std::cout << "true" << std::endl;
  }

  if (std::string(test1) == std::string(test3)) {
    std::cout << "false" << std::endl;
  }

  */

  return 0;
}
