# Software-Group-Test ![](https://komarev.com/ghpvc/?username=Chen-dll)

These programs are coded by Chen Sixiang.
完成时间 2024年10月7日

# 文本编辑器

## 基础功能

1. 使用两个栈储存信息，一个存储文本内容，另一个存储操作记录。  
2. 支持以下操作：  
    - 插入字符：在当前光标位置插入一个字符。  
    - 删除字符：删除当前光标位置的字符。  
    - 撤销：撤销最近的一次操作。  
3. 编写函数，模拟文本编辑和撤销操作。  
4. 编写一个测试函数，验证多个示例输入的文本编辑和撤销过程。  

## 进阶功能

1. 支持多种编辑操作，例如复制、粘贴。
2. 支持重做（redo）功能，允许用户恢复最近撤销的操作。（使用栈操作实现）  
3. 添加错误处理，例如操作非法的情况。（如删除空文本时的操作）。  

## 设计思路

### 1. **基础架构**

- **操作记录结构体 (`Operation`)**：
  - 用于记录用户的操作，包括插入、删除和粘贴操作的信息。

- **自定义栈类 (`Stack<T>`)**：
  - 用于存储文本和操作记录，自定义栈的功能包括动态扩展、插入、弹出、查看栈顶元素、安全性检查、获取栈的大小、复制栈的内容到字符串。

    #### 自定义栈的实现方法（核心）

    ##### 类的基本结构

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
    
    ###### 动态扩展栈的容量
    
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
    
    ##### 构造函数和析构函数
    
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
    
    ##### 栈的基本操作
    
    ###### 推入元素（push）
    
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
    
    ###### 弹出元素（pop）
    
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
    
    ###### 查看栈顶元素（peek）
    
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
    
    ###### 检查栈是否为空（isEmpty）
    
    ```cpp
    bool isEmpty() const
    {
        return top == -1; // 栈顶索引为-1时栈为空
    }
    ```
    
    - **功能**：判断栈是否为空。
    - **实现过程**：如果 `top` 为 `-1`，则返回 `true`，否则返回 `false`。
    
    ###### 获取栈的大小（size）
    
    ```cpp
    int size() const
    {
        return top + 1; // 栈的大小为栈顶索引加1
    }
    ```
    
    - **功能**：返回栈中元素的个数。
    - **实现过程**：返回 `top + 1`，因为 `top` 是从 `0` 开始计数的。
    
    ###### 复制栈内容到字符串（toString）
    
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

- **文本编辑器类 (`TextEditor`)**：
  - 实现了文本编辑的核心功能，包括插入、删除、撤销、重做、复制、粘贴等操作。
  - 实现了控制台实时显示文本内容，按照特定操作清屏的功能，并且有操作信息和错误信息的延长提示，同时设置了按键操作提示。
  - 实现了计算并设置光标位置的功能，确保其保持在文章末尾，在需要输出时移到相应位置，确保正常输出信息。
  - 实现了清空文本栈的功能，供测试操作使用。
  - 实现了测试操作时生成日志文件的功能，以供查看调试。
  - 实现了模拟键盘基本按键和特殊按键的输入。
  - 实现了随机生成测试文本和测试操作的功能。

### 2. **关键功能**

- **插入字符 (`insertChar`)**：
  - 将字符推入文本栈，并记录操作到撤销栈中。同时清空重做栈。

- **删除字符 (`deleteChar`)**：
  - 从文本栈中删除字符，记录操作到撤销栈中。返回删除是否成功。

- **撤销操作 (`undo`)**：
  - 从撤销栈中弹出最后的操作记录，根据操作类型（插入、删除、粘贴）恢复文本状态，并将该操作压入重做栈中。
  - 撤销粘贴操作时，删除粘贴的文本，而不是逐个字符删除。
  - 不能撤销复制操作。

- **重做操作 (`redo`)**：
  - 从重做栈中弹出操作记录，并根据操作类型重新执行相应的文本操作，将该操作压回撤销栈。
  - 重做粘贴操作时反向插入粘贴的文本，确保显示顺序正确。

- **复制与粘贴 (`copy` 和 `paste`)**：
  - 复制当前文本到剪贴板，并允许从剪贴板粘贴内容到文本中。粘贴操作记录在撤销栈中。
  - 粘贴时将剪贴板内容逐个插入到文本堆栈中，反向遍历保证粘贴的顺序正确

### 3. **显示&交互**

- **显示文本 (`displayText`)**：
  - 显示当前文本内容并提供操作说明，更新光标位置 (通过文本长度和窗口宽度来计算) ,以便用户继续输入。
  - 在发生行数变化和完成测试操作时清屏，能支持输入多行文本。

- **显示错误信息 (`displayError`) 和延迟显示 (`showErrorWithDelay`)**：
  - 提供错误信息反馈，并在延迟后清除信息。

- **显示测试消息 (`showOperationWithDelay`)**
    - 提示当前模拟的操作，并且提示用户日志的状态。

### 4. **日志记录&文件操作**

- **操作日志**：
  - 全局操作日志数组用于记录所有用户操作，支持写入日志 (`appendLog`)、保存日志 (`saveLogToFile`)，以便后续查看。

### 5. **自动化测试**

- **模拟按键和文本输入**：
  - 提供模拟键盘按键 (`simulateCtrlKey`、`simulateBackspaceKey`、 `simulateTextInput`、 `simulateKey`) 的功能，允许自动化测试文本编辑功能，通过虚拟按键模拟用户输入。
  - 随机生成测试文本 ('generateRandomText') 和测试操作 (先复制粘贴撤销重做，再让五个操作随机进行) ，测试文本包括大小写字母和数字，实现测试示例的多样。
  - 测试操作时，在界面显示此时操作以供调试。

### 6. **其他功能**
- **获得管理员权限**
  - 获得管理员权限以屏蔽控制台对 `Ctrl+C` 的响应 (关闭窗口), 确保测试操作正常进行。
  - 获取管理员权限以切换输入法

- **切换输入法**：
  - 切换成英文输入法减少输入问题，保证测试操作的正确进行。  
  - 在测试操作打印完随机生成的文本后切换为中文输入法，避免日志文件出现不必要的乱码问题。

- **多线程进行**
- 实现主循环函数 (处理键盘输入和文本操作) 和测试函数同时进行，测试函数监测按下 `F1` 按键的动作，按下后进入测试内容。
- 多线程进行使得测试的每步操作文本编辑器都能实时处理显示。
- 接收到 `Esc` 按键后，结束两个线程，退出程序。

### 7. **其他&后续优化**
- 代码使用的字符编码是 `UTF-8`，`Dev-C++` 直接打开可能有乱码，在 `VS 2022` 的项目一并打包成压缩包放到目录下了，利用 `VS 2022` 生成的程序大小更小。
- 未能实现用方向键控制光标的移动，也没有实现移动光标后对光标位置处文本进行编辑。
- 后续考虑加上剪切操作 `Ctrl + X`，实现按住 `Shift + 方向键` 能选中文本进行操作的功能。
- 另外在最初界面直接按下 `F1` 进入测试操作可能会有一点显示异常。
- 最终代码是 `Editor.cpp` ，`Editor 调试.cpp` 在调试功能逻辑。
- 运行结束后记得切换输入法。

### 8. **程序截图**
![1](https://github.com/user-attachments/assets/a68420dc-0139-4a79-917b-49044c9ab739)
![2](https://github.com/user-attachments/assets/80ec94d6-7501-488c-8280-b2551e994851)
![3](https://github.com/user-attachments/assets/c1a4cb49-d9c6-47c7-8c50-700b7506f61d)
![4](https://github.com/user-attachments/assets/72ae788c-4346-4a15-b140-b67453a22c0b)
![5](https://github.com/user-attachments/assets/efa39771-1d0b-4829-ace4-d402283b5bfb)
