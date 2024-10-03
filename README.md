# Software-Group-Test
These programs are coded by Chen Sixiang. 
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

#### 2.2 **输入法切换与管理员权限**
- `RunAsAdmin()` 函数负责检测程序是否以管理员权限运行，若未以管理员权限运行，则重新启动程序获取权限。这是为了确保程序能够修改控制台窗口和切换输入法。
- 为了提高输入稳定性，程序通过 `SwitchToEnglish()` 函数切换系统输入法为英文（美国英语），避免由于使用不同输入法带来的操作问题。

#### 2.3 **游戏板操作**
- 通过 `move_left()`, `move_right()`, `move_up()`, `move_down()` 四个函数，实现了 2048 的核心操作：向四个方向移动数字、合并相同的数字、更新得分等。每次移动后，程序会在空白处生成一个随机的 2 或 4。
- 为了缓解闪屏问题，使用了 `draw_board(should_clear)` 来决定是否需要清除背景后重新绘制游戏板(在有效移动后才清屏)，从而减少不必要的清屏。
- 在从键盘输入获取有效的输入后，调用`move`等函数，具体实现方法：先用`save_to_undo()`保存当前游戏状态(游戏板和得分)，上下移动操作遍历每一列数字

#### 2.4 **撤销功能**
- 程序支持撤销操作，最多支持 2048 次撤销。`save_to_undo()` 会在每次移动操作前保存当前状态，并在 `undo()` 中恢复最近的游戏状态，保证玩家可以多次撤销操作。

### 3. **游戏流程**
- 游戏初始化时通过 `initialize_board()` 遍历数组清空棋盘，并随机生成两个 2 或 4 的数字。
- `add_random_number()` 负责在每次玩家有效移动之后，随机在空位上生成一个新的数字（2 或 4），游戏初始有两个数字。
- 游戏胜利和失败的判断分别通过 `check_win()` 和 `check_loss()` 完成。当任意格子数字达到 2048 时，判定玩家胜利；若没有空位且不能再进行任何合并操作时，判定玩家失败。

### 4. **游戏存档和读档**
- 玩家可以通过 `save_game()` 将当前游戏状态保存到文件中，通过 `load_game()` 恢复之前保存的游戏进度。使用了 `fwrite()` 和 `fread()` 函数分别进行数据的保存与读取，保证下次可以从保存的进度继续游戏。

### 5. **用户提示和反馈**
- `show_message()` 用于在特定情况下向玩家显示短暂的(1.5秒)提示信息，例如游戏存档成功、撤销操作失败等。
- `show_game_over()` 在游戏失败时显示结束信息，提示玩家游戏结束。
- `show_win()` 在游戏胜利时显示胜利信息，提示玩家游戏胜利。

### 6. **总结**
整个 2048 程序的设计基于 EasyX 图形库和 C/C++ 语言，实现了经典的 2048 游戏，基础功能包括移动、合并、生成随机数字，进阶功能包括撤销、存档/读档，记分，以及系统功能包括获得管理员权限和输入法切换，确保了良好的用户体验。
