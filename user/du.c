#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

uint
du(char *path, int is_top)
{
  int fd;
  struct stat st;
  struct dirent de;
  char buf[512], *p;
  uint total;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "du: cannot open %s\n", path);
    exit(1);
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "du: cannot stat %s\n", path);
    close(fd);
    exit(1);
  }

  if(st.type != T_DIR){
    close(fd);
    if(is_top)
      printf("%d\t%s\n", (int) st.size, path);
    return st.size;
  }

  total = st.size;
  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;
    if(de.name[0] == '.' && (de.name[1] == 0 || (de.name[1] == '.' && de.name[2] == 0)))
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    total += du(buf, 0);
  }
  close(fd);

  printf("%d\t%s\n", total, path);
  return total;
}

int
main(int argc, char *argv[])
{
  if(argc != 2){
    fprintf(2, "Usage: du path\n");
    exit(1);
  }
  du(argv[1], 1);
  exit(0);
}
