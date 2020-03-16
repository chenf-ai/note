## 什么是应用程序

### 软件和进程的概念

+ 软件：从狭义的角度来看软件是可执行文件和其他数据文件
+ 进程：正在运行的程序

### 系统常见的应用程序

+ Core Utilities(coreutils)
  + 系统中安装的是GNU Coreutils，可使用busybox代替
+ 系统/工具程序
  + bash, apt, ip, ssh, vim , tmux, jdk, python, ...
+ 其他各种应用程序

### ELF二进制文件

+ 可执行文件本质上也是一个普通的文件
  + 操作系统第一个对象，是一个存储在文件系统上的字节序列。
    + 与平时创建的文本文件（例如程序）没有本质区别。
    + 操作系统提供API打开、读取、改写。
    + 可使用vim, cat, xxd等命令查看可执行文件。
      + 在vim中打开，二进制的部分显示异常，但可以看到字符串常量（例子：vim /bin/ls）原因可能是help命令部分的字符串等等。

+ 解析ELF文件
  + readelf是专门解析ELF可执行文件的工具。
    + ELF文件的header
      + 文件内容的分布
      + 指令集体系结构
      + 入口地址
    + ELF的program headers
      + 决定ELF应该如何被加载器加载
  + 如果需要用代码解析，/usr/include/elf.h提供了必要的定义，例如某一份代码中需要实现相应的功能。

### 应用程序怎样调用操作系统

+ 错误例子1

  代码中调用printf，但在命令行命令如下：

  ```shell
  $ gcc -c hello.c
  $ ld hello.o
  ```

  此时会报出无法找到_start symbol的warning，同时会报出undefined reference to 'puts'的错误。

  分析如下：

  ​		即使是选择-O0选项不进行优化，实际上gcc还是会进行一定程度的优化，将printf优化为puts代替；另一方面，_start是链接器默认的入口，可以用-e指定，比如ld -e main hello.c，指定链接后程序的入口是从main开始。

+ 错误例子2

  代码十分简单

  ```c
  int main() {
  }
  ```

  shell命令如下

  ```shell
  $ gcc -c hello.c
  $ ld -e main hello.o
  ```

  此时会报出segmentation fault的错误，原因是从main开始执行程序后，如果objdump会发现汇编代码中有retq的指令，而此时从main开始执行，main并没有调用者，因此是一个非法的返回地址；事实上，尝试对一般正确方法得到的可执行文件进行objdump操作，可以发现<_start>部分的汇编指令当中存在callq的内容。

+ 使用gdb进行调试，gdb能够帮助我们观察程序的执行

  + starti可以帮助我们从第一条指令开始执行程序
  + layout asm可以方便地查看汇编
  + bt指令可以帮助我们查看当前用户栈中的内容
  + info register可以帮助我们查看当前寄存器中的值

  [gdb手册](sourceware.org/gdb/documentation/)

+ 成功的尝试：汇编

  写出正确的指令序列即可在操作系统上正常执行

  例子：minimal.S

  ```asm
  # See also: man 2 syscall
    
  .globl foo
  foo:
    movl $1,     %eax # write(
    movl $1,     %edi #   fd=1,
    movq $s,     %rsi #   buf=s,
    movl $(e-s), %edx #   count=e-s
    syscall           # );
  
    movl $60, %eax    # exit(
    movl $1,  %edi    #   status=1
    syscall           # );
  
  s:
    .ascii "\033[01;31mHello, OS World\033[0m\n"
  e:
  
  ```

  －－－－

  当然也存在C语言版本的系统调用

### C代码中调用操作系统

+ 关于objdump命令
  + -d: disassemeble; -S: source（需要-g选项支持，即编译时加上-g选项）
+ syscall@plt为动态链接（来自libc的代码）

+ main()之前的事情

  ​		事实上，在main函数执行之前，系统做了很多的事情。执行该程序的第一条调用并非main，也并非main对应的\_start，事实上第一条指令是在libc那里的\_start指令。可通过gdb查看进程状态发现进程的内存中多了libc-2.27.so，由此可知在main开始之前会首先进行libc自己的初始化。

+ 了解一个可执行程序具体使用了怎样的系统调用可以使用strace工具，例如下面的用法

  ```shell
  $ strace -f gcc hello.c 
  ```

  -f参数的作用是连同子进程一同打印，配合-p参数可应用到多进程的程序。

### 应用眼中的操作系统

+ 调用相应的操作系统API
+ 以`strace -f gcc a.c`为例，主要的系统调用为: execve, read, write，主要执行的程序包括：
  + cc1 - 编译器
  + as - 汇编器
  + collect2 - 收集器（收集构造函数信息）
    + 收集器在libc自己初始化环境时会使用
  + ld - 链接

## 积累

+ file命令可查看文件信息
+ 关于xxd命令==TO

+ -c, -E, -s ==TODO==

+ 关于C语言中的constructer，\_\_atribute\_\_的概念

+ Fira Code