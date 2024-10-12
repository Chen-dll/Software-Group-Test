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
  - 单向链表 (Singly Linked List)
  - 双向链表 (Doubly Linked List)
  - 循环链表 (Circular Linked List)
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
- **定义**: 后进先出 (LIFO) 的数据结构。只允许从一端进行操作。
- **基本操作**:
  - `push`: 将元素压入栈中
  - `pop`: 从栈中弹出元素
  - `peek/top`: 查看栈顶元素
- **使用场景**: 表达式求值、函数调用管理、深度优先搜索等。

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

## 4. 队列 (Queue)
- **定义**: 先进先出的数据结构。数据从一端进入，另一端取出。
- **基本操作**:
  - `enqueue`: 将元素加入队列
  - `dequeue`: 从队列中取出元素
- **使用场景**: 任务调度、缓冲区管理、广度优先搜索等。

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
