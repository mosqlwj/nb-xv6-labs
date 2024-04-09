#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define BUF_SIZE 512
// debug
void show_args(char *argv[]) {
  int i = 0;
  while (argv[i] != 0) {
    fprintf(1, "argv[%d] = %s ", i,  argv[i]);
    i++;
  }
  fprintf(1, "\n");
}
int main(int argc, char *argv[]) {

  int pid; // 进程的pid   
  char buf[BUF_SIZE] = {0}; // 保存输入的一行参数
  

  // 保存xargs后的命令及其参数到cmd_args中
  char* cmd_args[MAXARG] = {0};
  for (int i = 1; i < argc; i++) {
    cmd_args[i - 1] = argv[i];
  }

  // 从标准输入中读取一行参数
  int j = 0;
  while(read(0, &buf[j], 1) > 0) {
    if (buf[j] == '\n') {
      buf[j] = 0; // 换行符
      // fprintf(1, "buf = %s\n", buf);
      // 保存buf到cmd_args
      // show_args(cmd_args);
      cmd_args[argc - 1] = buf;
      // show_args(cmd_args);
      pid = fork();
      // 子进程
      if (pid == 0) {
        // show_args(cmd_args);
        exec(cmd_args[0], cmd_args);
        exit(0);
      } else {
        // 清空cmd_args
        for (int j = argc - 1; j < MAXARG; j++) {
          cmd_args[j] = 0;
        }
        // 清空buf
        memset(buf, 0, BUF_SIZE);
        wait(0);
      }
      j = 0;
    } else {
      j++;
    } 
  }
 
  exit(0);
}
