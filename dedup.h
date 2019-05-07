#ifndef __DEDUP_H__
#define __DEDUP_H__

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

std::string file_to_str(const std::string & filepath);  // file content to string.
void file_path_make(struct dirent * entry,
                    std::string & filepath);  // creates the filepath.
void create_hashtable(const std::string & filepath,
                      std::vector<std::list<std::pair<size_t, std::string> > > &
                          hashtable);  // search through directory for files and populate hashtable.
bool file_compare(const std::string & filepath1,
                  const std::string & filepath2);  // compare files together.
void file_remove(
    std::vector<std::list<std::pair<size_t, std::string> > > & hashtable);  // deduplicate.

#endif
