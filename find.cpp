#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX 4096 //max namelenth

int * findFile(char *filename, char *dirname, int num_fd)
{
  DIR *dir_for_find;
  struct stat info;
  struct dirent *entry;
  char *date;
  char full_path[MAX];
  static int num[2];
  num[0] = num_fd;

  if (!(dir_for_find = opendir(dirname)))
    exit(EXIT_FAILURE);

  // Main cycle for find needet file in dir and subdir
  while ( (entry = readdir(dir_for_find)) != NULL) {
    // Don't descend up the tree or include the current directory
    if ( !strcmp(entry->d_name, ".") ) continue;
    if ( !strcmp(entry->d_name, "..") ) continue;
      num[0]++;
      if ( entry->d_type == DT_DIR ) {
        // Prepend the current directory and recurse
        strncpy(full_path, dirname, MAX);
        if ( strrchr(dirname, '/')-dirname+1 != strlen(dirname) ) strncat(full_path, "/", MAX);
        strncat(full_path, entry->d_name, MAX);
        num[0]=findFile(filename, full_path, num[0])[0];
      }
      else if ( strcmp(entry->d_name, filename)== 0 ) {
        num[1]++;
        // Prepend the current full path
        strncpy(full_path, dirname, MAX);
        if ( strrchr(dirname, '/')-dirname+1 != strlen(dirname) ) strncat(full_path, "/", MAX);
        strncat(full_path, entry->d_name, MAX);
        stat(full_path, &info);
        printf ("File is found: %s\n", full_path);
        date = asctime(localtime(&info.st_ctime));
        printf (" Size in bytes: %ld\n Time created: %s Permissions: %o\n Number of index: %lu\n\n", info.st_size, date, info.st_mode, info.st_ino);
      }
   }
  closedir(dir_for_find);
  return num;
};

int main(int argc, char **argv)
{
  struct stat info;
  char filename[MAX];
  char dirname[MAX];
  int *num;

  if ( argc != 3 ) {
    // Check number arguments
    printf ("Error count arguments\n");
    fprintf(stderr, "Example usage: %s /root id_rsa.pub\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  else {
    if( stat( argv[1], &info ) != 0) {
      // Check access to dir
      fprintf (stderr, "Cannot access %s\n", argv[1] );
      exit(EXIT_FAILURE);
    }
    else if( info.st_mode & S_IFDIR ) {
      // Check is a dir and assigne variables
      strcpy(dirname,  argv[1]);
      strcpy(filename, argv[2]);
    }
    else {
      fprintf (stderr, "%s is no directory\n", argv[1] );
      exit(EXIT_FAILURE);
    }
    num = findFile(filename, dirname, 0);
    if ( *(num+1) == 0 ) printf("File not found.\n");
    printf ("Count of checked files and directories: %i\n", *(num+0));

    exit(EXIT_SUCCESS);
  }
};
