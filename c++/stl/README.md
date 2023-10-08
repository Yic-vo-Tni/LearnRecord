## STL六大部件
容器、 分配器、 算法、 迭代器、 适配器、 仿函数

---

## 容器

#### vector

用于需要快速定位任意位置上的元素，以及在尾部增删的场合

#### list

用于经常在元素任意位置插入、删除场合，用双向链表实现

forward_list，单项链表

#### deque

用于元素序列两端增删以及快速定位的场合，分段连续空间结构实现

#### stack

仅在元素序列的尾部增删场合

#### queue

仅在尾部增加，头部删除的场合

#### priority_queue

与queue相似，不同之处在于增删元素之后会对元素位置进行调整，使得头部元素最大，即每次删除最大元素

#### map和multimap

根据关键字访问元素的场合，常用二叉树实现

map：不同元素的关键字不能相同

multimap：不同元素的关键字可以相同

#### set和multiset

关键字与值合一

#### basic_string

与vector类似

---

# string

### string and C-style string

- string是一个类封装了char*
- 不用考虑内存释放和越界
- 本质是一个动态的char数组

### 常用操作




