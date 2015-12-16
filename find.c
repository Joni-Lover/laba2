#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

#define NAME_MAX 256

int findFile(const char *filename, const char *dirname)
{
  DIR *dir_for_find;
  struct stat info;
  struct dirent *entry;
  int num_fd;

  dir_for_find = opendir(dirname);

  // Main cycle for find needet file in dir and subdir
  while ( (entry = readdir(dir_for_find)) != NULL)
    num_fd++;
    stat( entry->d_name, &info);
    if ( info.st_mode & S_IFDIR )
      printf ("%s\n", entry->d_name);
    else if (strcmp(entry->d_name, filename)== 0)
      printf ("File is found: %s/n", entry->d_name);
    else
      findFile(filename, entry->d_name);

  closedir(dir_for_find);
};

int main(int argc, char **argv)
{
  struct stat info;
  char filename[NAME_MAX];
  char dirname[NAME_MAX];

  if ( argc < 3 )
    // Check number arguments
    printf ("Error count arguments\n");
  else
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

  findFile(filename, dirname);
  return 0;
};
