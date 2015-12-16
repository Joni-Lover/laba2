#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#define NAME_MAX 4096

int findFile(const char *filename, const char *dirname, int num_fd)
{
  DIR *dir_for_find;
  struct stat info;
  struct dirent *entry;
  char longest_name[NAME_MAX];

  if (!(dir_for_find = opendir(dirname)))
      _exit;

  // Main cycle for find needet file in dir and subdir
  while ( (entry = readdir(dir_for_find)) != NULL)
  {
    // Don't descend up the tree or include the current directory
    if ( !strcmp(entry->d_name, ".") ) continue;
    if ( !strcmp(entry->d_name, "..") ) continue;
      num_fd++;
      if ( entry->d_type == DT_DIR )
      {
        // Prepend the current directory and recurse
        strncpy(longest_name, dirname, NAME_MAX);
        strncat(longest_name, "/", NAME_MAX);
        strncat(longest_name, entry->d_name, NAME_MAX);
        findFile(filename, longest_name, num_fd);
      }
      else if ( strcmp(entry->d_name, filename)== 0 )
      {
        // Prepend the current directory and recurse
        strncpy(longest_name, dirname, NAME_MAX);
        strncat(longest_name, "/", NAME_MAX);
        strncat(longest_name, entry->d_name, NAME_MAX);
        stat(longest_name, &info);
        printf ("File is found: %s\n", longest_name);
//        printf ("Size in bytes: %s\n Time created: %n\n Permissions: %s\n Number of index: %s\n\n", info.st_size, info.st_ctime, info.st_mode, info.st_ino);
        printf ("Size in bytes: %li\n", info.st_size);
      }
   }
  closedir(dir_for_find);
  return num_fd;
};

int main(int argc, char **argv)
{
  struct stat info;
  char filename[NAME_MAX];
  char dirname[NAME_MAX];
  int num;

  if ( argc < 3 ) {
    // Check number arguments
    printf ("Error count arguments\n");
  }
  else {
    if( stat( argv[1], &info ) != 0) {
      // Check access to dir
      printf ( "Cannot access %s\n", argv[1] );
    }
    else if( info.st_mode & S_IFDIR ) {
      // Check is a dir and assigne variables
      strcpy(dirname,  argv[1]);
      strcpy(filename, argv[2]);
    }
    else {
      printf ( "%s is no directory\n", argv[1] );
    }
    num=findFile(filename, dirname, 0);
    printf ("Count of checked files: %i\n", num);
  }
};
