## 并发：从入门到放弃

### 基本概念

+ In computer science, concurrency refers to the ability of different  parts or units of a program, algorithm, or problem to be executed  out-of-order or in partial order, without affecting the final outcome.
+ 一般多处理器的时候才会是并行，并行是真正地同时进行，而并发并非同时进行。

### 多处理器编程：入门

+ 两个线程有什么是共享的
  + 共享：代码、全局变量数据
  + 独享：寄存器、局部变量数据、堆栈

+ 每个线程都要分配 8 MiB 堆栈，为什么 1,000 个线程没有耗尽内存？
  + 只是标记了一下，并没有真的分配。
  + 通过pmap查看。

### 并发放弃：原子性

+ 对于一个程序，如果在一些指令当中断开反而先去执行其他的指令可能会带来意想不到的结果，特指针对共享内存所带来的问题。

+ 由于中断机制的存在几乎无法或者说很难实现原子性

+ 存在一些线程安全的函数或API

  > printf(), fprintf(), sprintf(), snprintf(), vprintf(), vfprintf(), vsprintf(), vsnprintf()

### 并发放弃：顺序

+ 在指令的执行过程中可能并不像我们想象的顺序去执行，编译器可能会进行优化，允许源代码中“内存访问”指令不再按顺序甚至不再出现；这导致在不同的编译优化选项下，我们可能会得到不同的程序运行结果。

+ 例如

  ```markdown
  R(x), W(y), R(y), R(x), W(x), W(x)
  ```

  第2个R(x)可能会在编译时删去，第1个W(x)可能会被编译器删去。

### 并发放弃：可见性

+ 为了使CPU运行得更快，CPU可以不按顺序执行指令。
+ 现代处理器（动态数据流分析）：
  + 如果两条指令没有数据依赖关系，就让它们并行执行。
  + 乱序执行（out -of-order）：多处理器上执行的结果可以不等价于指令按照某个顺序执行的结果。

----

注意：顺序和可见性中似乎都涉及了order，在第二部分“顺序”体现的是编译阶段，而第三部分”可见性“指的是执行阶段。

----

### 尝试解决并发

+ 控制编译器行为，阻止编译优化——**volatile**
+ 保证原子性的尝试：stop_the_world() and resume_the_world()
+ 保证可见性：之后会介绍一个简单的模型

----

### 积累

+ ldd查看链接了什么库，例如ldd a.out。
+ 需要知道destructor的含义和用法。
+ 线程ID在进程内部是unique的，但仅仅是在进程内部是unique的。

+ pmap怎么用

