# Software-Group-Test ![](https://komarev.com/ghpvc/?username=Chen-dll)
These programs are coded by Chen Sixiang.   
完成时间 2024年10月3日 
# 2048
## 基础功能：
1. 随机数产生函数: 在初始化和每次操作后，在空区域产生2或4的随机数。
2. 上下左右移动操作: 移动数字朝指定方向并合并相同数字。
(合并时不仅限于相邻相同数字的合并，而是向特定方向合并直到边界)
3. 判断游戏状态: 达到2048时判断游戏胜利，无法移动时判断游戏失败。
(在每次移动后立即判断游戏是否结束)
## 进阶功能：
1. 返回上一步操作: 实现撤销上一步操作的功能。
2. 保存游戏进度: 将当前游戏状态保存到文件中，以便后续加载。
3. 加载历史游戏: 从文件中读取已保存的游戏状态，恢复游戏进度。
4. 计算并显示游戏得分。

## 设计思路

### 1. **基础架构**
- 游戏的核心逻辑是 4x4 的棋盘（`board`），每个格子可以存储一个数字，目标是通过上下左右操作移动方块（合并相同数字）最终生成 2048 方块。
- 游戏状态通过二维数组 `board` 表示，撤销操作通过三维数组 `previous_board` 和 `previous_scores` 保存历史游戏板和得分。

### 2. **功能模块**
#### 2.1 **图形显示和操作提示**
- 游戏通过图形界面来显示操作和游戏进展，使用 `draw_board()` 函数绘制当前的棋盘，不同的数字使用不同的颜色。
- `show_key_intro()` 函数用于在游戏开始时显示按键操作的提示，方便玩家了解如何移动方块、使用撤销、保存和加载游戏等功能。
- ##### 具体实现方法：
- 先利用 `initgraph()` 初始化窗口，再用 `setfillcolor()`、`solidrectangle()`、 `setbkmode()` 设置背景， `settextcolor()`，`settextstyle()`设置文本，用 `outtextxy()` 显示, 其中用`snprintf()`将 `int` 整形 转为 字符串，最后用 `closegraph()` 关闭图形窗口。

#### 2.2 **输入法切换与管理员权限**
- `RunAsAdmin()` 函数负责检测程序是否以管理员权限运行，若未以管理员权限运行，则重新启动程序获取权限。这是为了确保程序能够修改控制台窗口和切换输入法。
- 为了提高输入稳定性，程序通过 `SwitchToEnglish()` 函数切换系统输入法为英文（美国英语），避免由于使用不同输入法带来的操作问题。
- ##### 具体实现方法：
- 先判断当前程序是否以管理员身份运行，如果没有管理员权限，调用 `ShellExecute()` 设置 `runas` 动作来以管理员身份重新启动程序。
- 再调用 `LoadKeyboardLayout()` 切换当前输入法为英文（美国英语）

#### 2.3 **游戏板操作**
- 通过 `move_left()`, `move_right()`, `move_up()`, `move_down()` 四个函数，实现了 2048 的核心操作：向四个方向移动数字、合并相同的数字、更新得分等。每次移动后，程序会在空白处生成一个随机的 2 或 4。
- 为了缓解闪屏问题，使用了 `draw_board(should_clear)` 来决定是否需要清除背景后重新绘制游戏板(在有效移动后才清屏)，从而减少不必要的清屏。
- ##### 核心操作具体实现方法：
- 以 `move_up()` 为例

   其目标是将数字向上移动并合并相同的数字，移动结束后，生成一个随机的数字 (2 或 4) 到空位置，其余三个方向的移动操作类似。

1. **保存状态用于撤销**
   - 在移动之前，游戏状态应该被保存下来，以便玩家可以撤销这次移动操作。这可以通过将 `board` 和 `score` 复制到撤销状态中来实现（`save_to_undo()`）。

2. **遍历列并向上移动**
   - `move_up()` 的逻辑是逐列处理数字，从上到下依次遍历每个数字，尝试将它们向上移动。为了避免重复合并，每列中使用一个 `current` 变量指示当前可以合并或移动的目标位置。
   - 外层 `for` 循环遍历每一列（列号 `j`）。内层 `for` 循环遍历该列中的每个元素（行号 `i`），跳过第一行（`i = 1`），因为第一行不能向上移动。

3. **移动数字**
   - 如果当前 `board[i][j]` 不为零，则有可能向上移动。检查 `board[current][j]`（当前行的目标位置）是否为空或相同数字，以决定是否移动或合并：
     - **目标位置为空**: 如果当前的 `board[current][j]` 是 0，则将 `board[i][j]` 的数字移动到 `board[current][j]`，并将原来的位置 `board[i][j]` 清空为 0。
     - **数字相同**: 如果 `board[current][j]` 与 `board[i][j]` 数字相同，则将两个数字合并，即将 `board[current][j]` 翻倍，同时得分增加翻倍后的数值，`board[i][j]` 清空为 0，并且 `current` 行数增加（指示下一个可合并或移动的行）。
     - **处理完数字不相同**: 如果 `board[current][j]` 和 `board[i][j]` 不相同，则 `current` 行数增加，但不进行合并，仅仅是移动到下一个位置。

4. **状态更新和新数字生成**
   - 如果 `moved` 标志为真，意味着本次操作中有数字发生了移动或合并。这时需要在空白位置生成新的数字（2 或 4），并重新绘制游戏板（`add_random_number()`、`draw_board()`）。

  
#### 2.4 **撤销功能**
- 程序支持撤销操作，最多支持 2048 次撤销。`save_to_undo()` 会在每次移动操作前保存当前状态，并在 `undo()` 中恢复最近的游戏状态，保证玩家可以多次撤销操作。
- ##### 具体实现方法：
- `save_to_undo()` 遍历游戏板数组，将当前状态和得分存起来用于撤销，同时用 `undo_count`记录当前可撤销数量
- `undo()` 遍历游戏板数组，把存进 `previous_board` 的数据复制给游戏板数组，实现撤销至上一步，当 `undo_count` 为 0，提示不可再撤销。

### 3. **游戏流程**
- 游戏初始化时通过 `initialize_board()` 遍历数组清空棋盘，并随机生成两个 2 或 4 的数字。
- `add_random_number()` 负责在每次玩家有效移动之后，随机在空位上生成一个新的数字（2 或 4），游戏初始有两个数字。
- 游戏胜利和失败的判断分别通过 `check_win()` 和 `check_loss()` 完成。当任意格子数字达到 2048 时，判定玩家胜利；若没有空位且不能再进行任何合并操作时，判定玩家失败。
- #### 具体实现方法：
-  `initialize_board()` 遍历游戏板数组全部赋值0，实现清空
-  `add_random_number()` 利用 `do-while` 循环找到空位置，随机生成 2 或 4
-  `check_win()` 遍历游戏板，判断是否有2048出现
-  `check_loss()` 遍历游戏板，判断是否有空位或可以合并的位置

### 4. **游戏存档和读档**
- 玩家可以通过 `save_game()` 将当前游戏状态保存到文件中，通过 `load_game()` 恢复之前保存的游戏进度。使用了 `fwrite()` 和 `fread()` 函数分别进行数据的保存与读取，保证下次可以从保存的进度继续游戏。
- #### 具体实现方法：
- `fopen()`：保存时以二进制写入模式打开文件。如果文件不存在，则会创建新文件；如果文件存在，则会覆盖原内容。读取时以二进制读取模式打开保存的文件。
- `fwrite()`：将 4x4 游戏板状态和得分存入文件。
- `fread()`：从文件中读取保存的游戏板状态和得分，填充到当前游戏板中。
- `fclose(file)`：关闭文件，确保读取完成。
- `show_message()`：提示用户。
- 设置了错误提示，如果操作失败，程序会显示相应的错误提示，让玩家知道存档或加载操作未成功。

### 5. **用户提示和反馈**
- `show_message()` 用于在特定情况下向玩家显示短暂的(1.5秒)提示信息，例如游戏存档成功、撤销操作失败等。
- `show_game_over()` 在游戏失败时显示结束信息，提示玩家游戏结束。
- `show_win()` 在游戏胜利时显示胜利信息，提示玩家游戏胜利。
- #### 具体实现方法：
- 用 `setfillcolor()` 设置背景，`solidrectangle()` 填写消息框，`settextcolor()` 设置文本颜色， `outtextxy()` 显示消息
- 居中操作用 `textwidth()` 获取文本宽度，然后根据窗口大小计算得到中间位置坐标
- `_getch()` 用来等待用户按键，与 `system("pause")` 作用相同

### 6. **其他&后续优化**
- 代码使用的字符编码是 `UTF-8`，`Dev-C++` 直接打开可能有乱码，而且要配置好 `EasyX` 才能正常编译运行
- 在 `VS 2022` 的项目一并打包成压缩包放到目录下了，配置 `EasyX` 比较方便，而且利用 `VS 2022` 生成的程序大小更小。项目配置属性字符集要使用多字节字符集， 并且在C/C++ 预处理器定义添加 `_CRT_SECURE_NO_WARNINGS`
- `move()`等函数实现也可以用旋转游戏板的操作来简化，只用写一个方向的操作就可以，再写一个旋转游戏板的函数，其他三个方向先旋转对应次数再利用已有方向操作的函数，最后再旋转回去。
   - 具体就是用 `temp` 变量辅助交换，用 `for` 遍历游戏板数组，`temp = board[j][SIZE - i - 1]` 暂存旋转后的值，`board[j][SIZE - i - 1] = board[i][j];` 、 `board[i][j] = temp;` 交换原来的值和旋转后的值。
   - 考虑到这样可能会有值覆盖的问题(在交换数据时，直接覆盖了后续需要的数据)，有了下面的方案
   - 用 `#include <algorithm>` 里的 `swap()` 简化交换过程，再用上四元组交换，即外层循环只需要遍历矩阵的一半，内层循环遍历当前层的元素，一次处理四个互为旋转位置的元素。
     - `swap(board[i][j], board[j][SIZE - i - 1]);`  交换 `(i,j)` 和 `(j, SIZE-i-1)`
     - `swap(board[i][j], board[SIZE - i - 1][SIZE - j - 1]);`  交换 `(i,j)` 和 `(SIZE-i-1, SIZE-j-1)`
     - `swap(board[i][j], board[SIZE - j - 1][i]);`  交换 `(i,j)` 和 `(SIZE-j-1,i)`
     - 以此类推，达到顺时针旋转90°的操作。
- `2048 show 草稿.cpp` 里面主要在调试，尝试优化未果orz，软件组懂双缓冲技术的大佬教教我吧%%%，最终代码是 `![微信图片_20241007010333](https://github.com/user-attachments/assets/47d69d5d-a9b0-49d3-9cba-ace9cba4eec9)
2048 show.cpp`，后续考虑加入显示历史最高成绩的功能，再试试优化代码逻辑和图像显示。
- 运行结束完记得切换输入法

### 7. **程序截图**
![1](https://github.com/user-attachments/assets/59cb2e2f-7e80-4b5e-a218-f1043b261763)
![2](https://github.com/user-attachments/assets/d465e1bc-f4e5-4f2a-ae4d-f72efe41df5c)
![3](https://github.com/user-attachments/assets/16c78c58-fe26-41f1-a266-28804471db65)
![4](https://github.com/user-attachments/assets/e8d4bac0-7707-416f-9053-c05ecd65baff)
![5](https://github.com/user-attachments/assets/d256a28a-9043-4447-a8b0-e60de2ac9f3c)
