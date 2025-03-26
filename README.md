# OMNI 转 KEIL 代码

> 本代码基于本人，以及何云昊共同完成.
> 
> 代码命名规则基于卢子瑞学长提出的规定.
> 
> 运动学结算来源实验室流传代码.

## 命名约定

* 文件名一律大写;
* 函数规则: `文件名 _F_Xxx`,允许出现小写;
* 变量规则: `文件名 _V_Xxx`, 全局变量全大写, 局部变量允许出现小写;
* 宏规则:     `文件名 _D_XXX`, 全大写;

**解算函数要求**

* 解算总函数命名 `_F_Ctl(TYPEDEF_MOTOR *MOTOR, ...)`，其中，`*MOTOR`是结构体数组指针;
* 其他小解算函数(不在time函数中使用的)， `*MOTOR`是结构体指针（除了ROOT和Ctl）.

## 核心思路

~~有待补充~~

## 日志

* 2024-06-03 完成底盘,云台,发射移植,待上车调试,一些核心代码待重写.
* 2024-06-08 发射重写完成，缺少裁判系统算出拨弹量和转速拟合.
* 2024-06-21 使用最小二乘法拟合曲线，VOFA实时观测拟合参数，缺少求其平均值.
* 2025-02-16 更改为老头代码，逐项测试中.
* 2025-03-27 完整形态考核一版提交，自瞄问题较大，飞坡控制良好.

## TODO

- [ ] 中期前完成
  - [ ] chassis.c 看todo
  - [x] gimbal.c 两种模式都写
  - [ ] 遥控控制写成define / 结构体整合
  - [ ] top.c 简化 a) 圈数解算和电机写一起 b) currentAngle 能否去掉 c) 完全简化
  - [ ] pid.c yaw 双环pid去掉陀螺仪 // 尝试完全简化
  - [ ] 灯显示车状态

- [ ] 网线通信/can
- [ ] c++ c 混编

- [x] ~~快速移植(预计6月底完成)~~
  - [x] ~~发射重写~~
  - [x] ~~底盘因子功率分配~~
  - [x] ~~小陀螺旋转~~
  - [x] ~~陀螺仪spi通信~~
  - [x] ~~视觉通信~~
  - [x] ~~裁判系统~~
  - [x] ~~调试运动pid~~
  - [x] ~~freertos代替定时器~~

## 提交到代码方式

### PR 方式

####  你的要求

```shell
git checkout -b rtos
```

创建并切换到 `rtos` 分支。

```shell
git pull --allow-unrelated-histories origin rtos
```
使用 `git pull` 命令的 `--allow-unrelated-histories` 选项来强制 `Git` 合并两个不相关的分支。这样，你的本地分支将合并远程分支的历史，从而使得它们相关.

以后正常提交即可。注意，需要

```shell
git push origin rtos
```

指明推送分支。创建合并请求时需要选对分支（你的，我的）！！！

#### 我的要求

* 创建新的测试分支
* 接收合并请求
* 测试后，合并分支
* 关联到本地仓库

仅指出第四步代码

```shell
git fetch origin
git merge origin/main
```
### 开发者模式

在仓库设置中给你权限后，你可以和我一样对代码进行修改。

因为能直接修改代码，所以建议你开一个`branch`。

### 删除分支

> using ChatGPT 4-o
> 
>To delete a branch both locally (home) and remotely, follow these steps:

### 1. Deleting a Local Branch
1. **List all branches** to verify the branch name:
   ```bash
   git branch
   ```

2. **Delete the local branch**:
   ```bash
   git branch -d branch_name
   ```
   If the branch hasn't been merged yet, you might need to force delete it:
   ```bash
   git branch -D branch_name
   ```

### 2. Deleting a Remote Branch
1. **Delete the remote branch**:
   ```bash
   git push origin --delete branch_name
   ```

## 已踩的坑

* 裁判系统数据不正常

原因： 串口DMA模式使用了`circle`模式，而移植代码中对缓存区进行了释放，产生冲突。

总结：CubeMX中的`circle`不可信，一定要小心使用。

* 虚连问题

电源线一定要换新线，包括信号线。

* 裁判系统通信有数据，但是较为杂乱

可能为与电源线共地问题，待完全测试。已经几乎确定是共地问题。（开始未接负载，裁判系统接受正常，后挂多个负载后，接受有问题。） 

PS: 可能因为nx上有串口滤波，导致可以直接读到数据。、

* CAN线L断

检查方法: CAN_H 对地2.5V, CAN_L对地2.5V,CAN_H与CAN_L正常0V / 或直接测试终端电阻
PS: 这种状态下能够开环控制电机,请仔细甄别. 

* 视觉跟随问题
	1. pid 参数与原遥控控制大为不同, 请从0开始调,可以先给1,逐渐增大,看反馈效果. 
	2. nx 会吞掉 0x13 这一个类型的数据, 保证包中除了解算可能存在, 其他校验类 0x13 全删除.
	3. 视觉 pid 的角度环输出应取负值, 因为电机均反装.

