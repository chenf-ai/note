# 二叉搜索树（BST）
+ 对于树的所有节点x，如果y是他的左子树，那么$y\leq x$；如果y是他的右子树，那么$y\geq x$. (BST property)

## OSet数据类型在BST上的实现
### 查找
+ 给定BST和一个键值k，要求查找值为k的节点是否存在。
+ 如果某个节点值为k，那么返回这个节点。如果小于k，递归到右子树；如果大于k，递归到左子树查找。
+ 最坏情况下，时间开销为$\Theta (h)$，h是BST的高度。

### 查找最小元素和最大元素
+ 查找最小元素：一直向左子树查找即可
+ 查找最大元素：一直向右子树查找即可

### 查找后继节点
+ 在BST中查找最小的比给定的节点x的键值更大的节点
+ 如果x的右子树非空，在以x为root的树查找最小元素即可。

### 插入节点
+ 类似于查找操作，最后将要插入的值插入到一个叶子节点的位置。

### 删除元素
+ 首先查找z
+ 如果z没有子树，直接移除z
+ 如果z有一个子树，将z移除，然后把z的子树提升到作为z的父节点的子树即可
+ 如果z有两个子树，那么思路就是要使用z的后继元素来替代z的位置。由于z有右子树，因此z的后继元素一定在右子树中
  + 情况一：z的后继元素就是z的右孩子，也就是z的右孩子的左子树非空。直接
  + 情况二：