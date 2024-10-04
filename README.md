# Software-Group-Test ![](https://komarev.com/ghpvc/?username=Chen-dll)
These programs are coded by Chen Sixiang.  
完成时间 2024年10月6日  
# 学生信息管理系统
## 基础功能
1.查询所有学生信息：系统能够显示所有学生的基本信息，如学号、姓名、年龄、班级等。  
2.添加、删除、修改学生信息：管理员可以方便地添加新的学生信息，删除不再需要的学生信息，或者修改已有的学生数据  
## 进阶功能：
1.使用文件功能保存或加载数据  
2.条件查询学生信息,管理员可以根据特定的条件（如姓名、年龄、成绩等）查询学生信息，快速定位需要的数据。  
3.条件排序学生信息,系统能够根据指定的条件（如成绩、年龄等）对学生信息进行排序  

## 设计思路
### 1. **基础架构**
- 核心是使用结构体数组来储存多个学生信息，再通过具体的函数实现添加、删除、修改、查询、排序、保存和加载功能
### 2. **功能模块**
#### 2.1 **添加学生信息**
- 先判断能不能继续添加，空间足够直接对应输入
#### 2.2 **删除学生信息**
- 根据用户输入选择 ID 或者 姓名 来查找删除，利用 `strcmp()` 来对比字符串内容，然后根据对应条件遍历结构体数组查找，查找到就将数组后面的元素往前移，覆盖要删除的学生，查找不到就提示用户。
#### 2.3 **修改学生信息**
- 根据用户输入选择 ID 或者 姓名 查找学生进行修改操作，然后根据对应条件遍历结构体数组查找，找到就重新输入对应学生数据，查找不到就提示用户。
#### 2.4 **查询学生信息**
- 根据用户输入选择 姓名 或 年龄 或 成绩 进行查询，同样是遍历结构体数组找到就输出，最后提示按任意键返回。
#### 2.5 **排序学生信息**
- 根据用户输入选择 成绩 或 年龄 或 班级 或 姓名 进行排序操作，使用冒泡排序，可将成绩从高到低排序，年龄从小到大排序，按班级排序，按姓名字母顺序排序
#### 2.6 **保存和加载功能**
- 重定向输出后，遍历结构体数组逐个写入文件即可
  
  




