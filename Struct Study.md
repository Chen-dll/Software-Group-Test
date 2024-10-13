# 数据结构学习笔记
- 10月12日

## 1. 数组 (Array)
- **定义**: 数组是存储在连续内存空间中的同一类型元素的集合。通过索引访问元素。
- **特点**:
  - 固定大小，在声明时指定长度，无法动态扩展。
  - 快速访问 (O(1))，但插入和删除效率低。
- **使用场景**: 存储相同类型的元素集合，如整数、浮点数等。

**示例代码**:
```c
#include <stdio.h>

int main() 
{
    int arr[5] = {10, 20, 30, 40, 50}; // 声明并初始化数组
    int i;

    // 访问数组元素
    for (i = 0; i < 5; i++) 
    {
        printf("Element at index %d: %d", i, arr[i]);
    }

    return 0;
}
```

## 2. 链表 (Linked List)
- **定义**: 由节点组成，每个节点包含数据部分和指向下一个节点的指针。
- **类型**:
  - 单向链表  
  - 双向链表  
  - 循环链表  
- **特点**:
  - 动态大小，可以随时增加或删除节点。
  - 插入和删除操作效率较高 (O(1))，但查找速度较慢 (O(n))。
- **使用场景**: 实现动态数据集合、队列、栈等。

**单向链表模板代码**:
```c
#include <stdio.h>
#include <stdlib.h>

// 定义链表节点
struct Node 
{
    int data;
    struct Node* next;
};

// 创建新节点
struct Node* createNode(int data) 
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// 插入节点到链表末尾
void appendNode(struct Node** head, int data) 
{
    struct Node* newNode = createNode(data);
    if (*head == NULL) 
    {
        *head = newNode;
        return;
    }
    struct Node* temp = *head;
    while (temp->next != NULL) 
    {
        temp = temp->next;
    }
    temp->next = newNode;
}

// 打印链表
void printList(struct Node* head) 
{
    while (head != NULL) 
    {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL
");
}

int main() 
{
    struct Node* head = NULL;
    appendNode(&head, 10);
    appendNode(&head, 20);
    appendNode(&head, 30);
    printList(head);
    return 0;
}
```

## 3. 栈 (Stack)
- **定义**: 后进先出的数据结构。只允许从一端进行操作。
- **基本操作**:
  - `push`: 将元素压入栈中
  - `pop`: 从栈中弹出元素
  - `peek/top`: 查看栈顶元素
- **使用场景**: 表达式求值、文本编辑器、深度优先搜索等。

**栈的数组实现**:
```c
#include <stdio.h>
#define MAX 10

int stack[MAX];
int top = -1;

void push(int data) 
{
    if (top >= MAX - 1) 
    {
        printf("Stack Overflow");
        return;
    }
    stack[++top] = data;
}

int pop() 
{
    if (top < 0) 
    {
        printf("Stack Underflow");
        return -1;
    }
    return stack[top--];
}

int peek() 
{
    if (top < 0) 
    {
        printf("Stack is Empty");
        return -1;
    }
    return stack[top];
}

int main() 
{
    push(10);
    push(20);
    printf("Top element: %d", peek());
    printf("Popped element: %d", pop());
    return 0;
}
```
**自定义栈类 (`Stack<T>`)**：
  - 用于存储文本和操作记录，自定义栈的功能包括动态扩展、插入、弹出、查看栈顶元素、安全性检查、获取栈的大小、复制栈的内容到字符串。

    ### 自定义栈的实现方法

    #### 类的基本结构

    ```cpp
    template <typename T>
    class Stack
    {
    private:
        T *data;      // 动态数组存储栈元素
        int capacity; // 栈的容量
        int top;      // 栈顶索引
    ```
    
    - **模板类**：使用 `template<typename T>` 使得栈可以存储任意类型的元素。
    - **私有成员**：
      - `data`：指向动态数组，存储栈的元素。
      - `capacity`：栈的总容量。
      - `top`：栈顶元素的索引，初始值为 `-1` 表示栈为空。
    
    ##### 动态扩展栈的容量
    
    ```cpp
    void resize()
    {
        int newCapacity = capacity * 2;  // 扩展为原来的两倍容量
        T *newData = new T[newCapacity]; // 分配新空间
    
        // 复制旧数据到新数组
        for (int i = 0; i <= top; ++i)
        {
            newData[i] = data[i];
        }
    
        // 删除旧数组，切换到新数组
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }
    ```
    
    - **功能**：当栈满时，动态扩展栈的容量。
    - **实现过程**：
      - 新的容量是当前容量的两倍。
      - 分配新的数组 `newData`，然后将旧数组的数据复制到新数组。
      - 释放旧数组的内存，更新 `data` 和 `capacity`。
    
    #### 构造函数和析构函数
    
    ```cpp
    // 构造函数
    Stack(int size = SIZE) : capacity(size), top(-1)
    {
        data = new T[capacity]; // 创建动态数组
    }
    
    // 析构函数
    ~Stack()
    {
        delete[] data; // 释放动态数组
    }
    ```
    
    - **构造函数**：初始化栈的容量和栈顶索引，并分配动态数组。
    - **析构函数**：在栈对象销毁时释放动态数组占用的内存，避免内存泄漏。
    
    #### 栈的基本操作
    
    ##### 推入元素（push）
    
    ```cpp
    void push(const T &value)
    {
        // 检查是否需要扩展容量
        if (top >= capacity - 1)
        {
            resize(); // 动态扩展容量
        }
        data[++top] = value; // 增加栈顶索引并赋值
    }
    ```
    
    - **功能**：将新元素添加到栈顶。
    - **实现过程**：
      - 检查当前栈是否已满，如果满则调用 `resize`。
      - 将新元素放入 `data` 数组，并更新 `top`。
    
    ##### 弹出元素（pop）
    
    ```cpp
    T pop()
    {
        if (isEmpty())
        {
            cerr << "栈为空，无法弹出元素！" << endl;
            throw out_of_range("栈为空"); // 抛出异常信息
        }
        return data[top--]; // 返回栈顶元素并减少栈顶索引
    }
    ```
    
    - **功能**：从栈顶移除元素。
    - **实现过程**：
      - 检查栈是否为空，若为空则输出错误信息并抛出异常。
      - 返回栈顶元素，并将 `top` 减少。
    
    ##### 查看栈顶元素（peek）
    
    ```cpp
    T peek() const
    {
        if (isEmpty())
        {
            cerr << "栈为空，无法查看栈顶元素！" << endl;
            throw out_of_range("栈为空"); // 抛出异常信息
        }
        return data[top]; // 返回栈顶元素
    }
    ```
    
    - **功能**：获取栈顶的元素但不移除它。
    - **实现过程**：
      - 检查栈是否为空，若为空则输出错误信息并抛出异常。
      - 返回 `data[top]`。
    
    ##### 检查栈是否为空（isEmpty）
    
    ```cpp
    bool isEmpty() const
    {
        return top == -1; // 栈顶索引为-1时栈为空
    }
    ```
    
    - **功能**：判断栈是否为空。
    - **实现过程**：如果 `top` 为 `-1`，则返回 `true`，否则返回 `false`。
    
    ##### 获取栈的大小（size）
    
    ```cpp
    int size() const
    {
        return top + 1; // 栈的大小为栈顶索引加1
    }
    ```
    
    - **功能**：返回栈中元素的个数。
    - **实现过程**：返回 `top + 1`，因为 `top` 是从 `0` 开始计数的。
    
    ##### 复制栈内容到字符串（toString）
    
    ```cpp
    string toString() const
    {
        string result;
        // 通过遍历操作将当前栈的内容存储到临时栈
        for (int i = 0; i <= top; ++i)
        {
            result += data[i]; // 逐个构建字符串
        }
        return result; // 返回字符串
    }
    ```
    
    - **功能**：将栈中的元素转换为字符串。
    - **实现过程**：
      - 通过循环遍历栈中的所有元素，将其逐个添加到结果字符串中。
      - 返回构建的字符串。

## 4. 队列 (Queue)
- **定义**: 先进先出的数据结构。数据从一端进入，另一端取出。
- **基本操作**:
  - `enqueue`: 将元素加入队列
  - `dequeue`: 从队列中取出元素
- **使用场景**: 任务调度、广度优先搜索等。

**队列的数组实现**:
```c
#include <stdio.h>
#define MAX 5

int queue[MAX];
int front = -1, rear = -1;

void enqueue(int data) 
{
    if (rear == MAX - 1) 
    {
        printf("Queue Overflow");
        return;
    }
    if (front == -1) front = 0;
    queue[++rear] = data;
}

int dequeue() 
{
    if (front == -1 || front > rear) 
    {
        printf("Queue Underflow");
        return -1;
    }
    return queue[front++];
}

int main() 
{
    enqueue(10);
    enqueue(20);
    printf("Dequeued: %d", dequeue());
    return 0;
}
```
