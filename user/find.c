#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void find(char* path, char* filename) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0 ) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  strcpy(buf, path); // copy path to buf
  p = buf + strlen(buf);
  *p++ = '/';

  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0) {
      continue;
    } 

    if (!strcmp(de.name, ".") || !strcmp(de.name, "..")) {
      continue;
    }

    memmove(p, de.name, DIRSIZ);
    *(p + DIRSIZ) = 0;
    if (stat(buf, &st) < 0) {
      printf("find: cannot stat %s\n", buf);
      continue;
    }

    switch (st.type) {
      case T_FILE:
        if (!strcmp(filename, de.name)) {
          printf("%s\n", buf);
        }
        break;
      case T_DIR:
        find(buf, filename);
        break;
    }

  }

  close(fd);
}

int
main(int argc, char *argv[])
{

  if(argc != 3){
    fprintf(2, "usage: find [path] [target filename]\n");
    exit(0);
  }
  
  char* path = argv[1];
  char* filename = argv[2];

  find(path, filename);
  exit(0);
}
