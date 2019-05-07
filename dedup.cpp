#include "dedup.h"
// recursion with stat does not work. use the dt in the struct.

std ::string file_to_str(const std::string & filepath) {
  std::ifstream input;
  input.open(filepath);  // filename can be pathway of filename
  std::stringstream buffer;
  buffer << input.rdbuf();
  std::string lines = buffer.str();
  input.close();

  return lines;
}  // convert file content to string.

void file_path_make(struct dirent * entry, std::string & filepath) {
  filepath += "/";
  filepath += std::string(entry->d_name);

}  // make the file path in the create_hashtable function

void create_hashtable(const std::string & filepath,
                      std::vector<std::list<std::pair<size_t, std::string> > > & hashtable) {
  DIR * dir;              // pointer to the opened directory
  struct dirent * entry;  // for readdir - a pointer to the dirent formed.
  char current[] = ".";
  char parent[] = "..";
  //struct * stat sb

  dir = opendir(filepath.c_str());  // c_str returns a pointer to array rep

  if (dir != NULL) {
    while ((entry = readdir(dir)) != NULL) {
      // stat(entry->d_name, &sb);

      // if it is a regular file.
      if (entry->d_type == DT_REG) {
        std::string filepath_new = filepath;
        file_path_make(entry, filepath_new);  // make the filepath for the file.
        //std::cout << filepath_new << std::endl;

        std::string filecontent = file_to_str(filepath_new);
        // std::cout << filecontent << std::endl;

        // hash the contents, fill hashtable
        size_t hasher = std::hash<std::string>{}(filecontent);
        int table_index = hasher % hashtable.size();
        // std::cout << table_index << std::endl;

        std::pair<size_t, std::string> kv_pair;
        kv_pair = std::make_pair(hasher, filepath_new);
        //  std::cout << kv_pair.first << " " << kv_pair.second << std::endl;
        hashtable[table_index].push_front(kv_pair);
      }

      // if it is a directory.
      else if ((std::string(current) != std::string(entry->d_name)) &&
               (std::string(parent) != std::string(entry->d_name)) && ((entry->d_type == DT_DIR)) &&
               ((entry->d_type != DT_LNK))) {
        std::string filepath_new = filepath;
        file_path_make(entry, filepath_new);
        //        std::cout << filepath_new << std::endl;

        create_hashtable(filepath_new, hashtable);  // recursive call.
      }
    }
  }

  closedir(dir);
}
// read through directories and fill hashtable with the files.

bool file_compare(const std::string & filepath1, const std::string & filepath2) {
  std::string content1 = file_to_str(filepath1);
  std::string content2 = file_to_str(filepath2);
  if (content1 == content2) {
    return true;
  }
  else {
    return false;
  }
}  // compare the content of 2 files.

void file_remove(std::vector<std::list<std::pair<size_t, std::string> > > & hashtable) {
  for (size_t k = 0; k < hashtable.size(); k++) {
    size_t file1_hash;
    std::string file1;
    std::string file2;

    while (hashtable[k].size() > 0) {
      file1_hash = hashtable[k].front().first;
      file1 = hashtable[k].front().second;
      //std::cout << file1 << " " << file1_hash << std::endl;

      for (std::list<std::pair<size_t, std::string> >::iterator it = hashtable[k].begin();
           it != hashtable[k].end();
           ++it) {
        if ((it->second != file1) && (it->first == file1_hash)) {
          file2 = it->second;
          //  std::cout << file2 << std::endl;
          if (file_compare(file1, file2) == true) {
            std::cout << "#Removing " << file1 << " (duplicate of " << file2 << ")." << std::endl;
            std::cout << "rm " << file1 << std::endl;

            break;
          }
        }
      }
      // std::cout << file1_hash << " " << file1 << std::endl;
      hashtable[k].pop_front();
      // std::cout << file1_hash << " " << file1 << std::endl;
    }
  }
}  // deduplicate the hashtable.

int main(int argc, char ** argv) {
  if (argc > 1) {
    std::cout << "#!/bin/bash" << std::endl;
    std::vector<std::list<std::pair<size_t, std::string> > > htable(2000);
    for (int i = 1; i <= argc - 1; i++) {
      // char dir2[] = "/home/rwy2/ece551/098_dedup"
      std::string directory = std::string(argv[i]);
      create_hashtable(directory, htable);
    }
    file_remove(htable);
  }
  return EXIT_SUCCESS;
}
