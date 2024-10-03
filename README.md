# Software-Group-Test
These programs are coded by Chen Sixiang. 
初一学习信竞时与好朋友讨论完成了程序的雏形 
在高中本人再次完善改进

## 1. 程序介绍
目的是读取输入字符，将其转换为特定的 C++ 代码，并将生成的代码写入文件。随后，使用 gcc 编译生成的代码文件并运行生成的可执行文件。

### 主要功能
1. **重定向输出**：
   - `freopen("ou.cpp", "w", stdout);` 将程序的标准输出重定向到文件 `ou.cpp`，即所有 `printf` 输出的内容将被写入这个文件。
   
2. **读取输入并存储到字符串 n**：
   - 通过 `cin.get()` 逐字符读取输入，直到遇到字符 `'@'` 为止，并将每个读取的字符存入字符串 `n` 中。

3. **输出代码到 ou.cpp**：
   - 写入 Windows API 和其他头文件。
   - 隐藏当前窗口：调用 `GetForegroundWindow()` 获取当前窗口句柄，使用 `ShowWindow(hwnd, SW_HIDE)` 隐藏该窗口。
   - 检查并模拟按下 Caps Lock 键（如果当前是激活状态），通过 `keybd_event` 模拟按键。
   - 遍历输入的字符 `n`，调用 `change()` 函数逐个转换字符，并在生成代码中插入 `Sleep(10)` 模拟延迟。

4. **编译和执行**：
   - `system("cmd /c gcc ou.cpp -o ou");` 使用 `gcc` 编译生成的 `ou.cpp` 文件并生成可执行文件 `ou.exe`。
   - `WinExec("cmd /c start ou.exe", SW_HIDE);` 以隐藏模式启动生成的可执行文件 `ou.exe`。
   - `system("pause");` 暂停程序，以便用户查看结果。`change()` 函数对每个字符执行条件判断，根据字符的类型（换行符、符号、字母、数字等），生成如 `symbol('X');` 这样的代码并输出。

   其中判断的字符有：
   - 换行符 (`'\n'`)
   - 常见符号如 `'-'`、`'@'`、`'/'`、`'?'`、`'!'`、`':'`、`' '`、`'.'` 等
   - 数字字符 (`c >= 48 && c <= 57`，对应 ASCII 码的数字字符范围)
   - 小写字母 (`c >= 97 && c <= 122`)
   - 大写字母 (`c >= 65 && c <= 90`)

5. **type.h**  
   其中 `symbol()` 用于模拟键盘按键事件来输入字符和符号，在 `ou.cpp` 中调用编译生成程序。
- 特殊符号（如感叹号、冒号、问号等）需要按下 Shift 键来生成，因此在代码中有对应的 Shift 键按下和释放逻辑。
- 对于数字、小写字母、大写字母，程序分别根据 ASCII 码范围进行判断并触发相应的按键事件。
