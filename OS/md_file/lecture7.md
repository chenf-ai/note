## 并发控制：互斥

### 操作系统是状态机

+ 在状态过程中，每一个状态的下一个状态都可能会切换到另一个CPU，每一个时刻都可能会进入中断，也可能继续执行。

### 操作系统中的自旋锁

+ 如果不使用自旋锁，中间临界区的printf会出现错位现象。
+ **Bug1**
  + 当把for(in volatile i = 0; i < 100000; i++)，使用普通的自旋锁，safety和liveness都没有问题，但fairness会出问题。
  + 解决方法：在进锁之前关中断，在出锁之后开中断。
+ **Bug2**
  + 当使用两把锁时，中断的开关会出现问题。
  + 解决方法：将EFLAGS push到堆栈当中，哪个堆栈——使用线程或CPU的堆栈都可以，但CPU更优。
+ **Bug3**
  + 重入会废掉这个进程，许多情况自旋锁会禁止重入的情况，即禁止先后两次调用lock函数。
+ **Bug4**
  + 如果在临界区中有睡眠调度的情况，可能会调度到另外一个正在尝试获得锁的线程，而因为锁已经被其他线程获得了，所以那里是while死循环，又因为此时是关中断状态，所以就成为了真正的死锁。
+ 自旋锁通常用于临界区较短，且不会在临界区中进行睡眠中断的情况。
+ 相关小结
  + 单CPU非抢占内核下：自旋锁会在编译时被忽略（因为单CPU且非抢占模式情况下，不可能发生进程切换，时钟只有一个进程处于临界区，自选锁也就没有什么用了）
  + 单CPU抢占内核下：自旋锁仅仅当作一个设置抢占的开关（因为单CPU不可能有并发访问临界区的情况，禁止抢占就可以保证临界区唯一被拥有）
  + 多CPU下：此时能够完全发挥自旋锁的作用，自旋锁在内核中主要用来防止多处理器中并发访问临界区，防止内核抢占造成的竞争。

### 操作系统中的互斥锁

+ 互斥锁的需求分析
  + 持有锁的线程
    + 允许处理器响应中断
    + 允许切换到其他线程执行
      + 访问磁盘的线程不能阻碍系统其他部分执行
  + 等待锁的线程
    + 不要占用处理器资源自旋
+ 解决方案
  + 不同于之前选用关中断的方法，互斥锁当出现需要进入已经锁住的临界区时，不会进行自旋，而是会将这一线程加入到任务列表当中，当之后当前锁住的协程结束后，它再来进入临界区。
  + 这样做代替了关中断的方法，满足了处理器中响应中断的需求，同时解决了死锁的可能性；另一方面，避免了另一处理器反复自旋的情况，提高了CPU的利用率。

### 相关思考

+ 关于互斥锁的部分，我之前一度在思考，为什么可以允许中断，这样yield到其他的线程，锁的作用不就没用了吗？我之所以这样想，是因为我理解错了锁和临界区的含义，可以调度到其他的线程，并发程序之所以会带来问题，很大程度上是因为共享内存的原因，所以临界区代码的含义是指并发敏感的那部分代码，所以即使调度到其他线程，因为临界区有锁的保护，所以并不会发生问题。