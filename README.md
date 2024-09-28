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

## TODO

- [ ] 快速移植(预计6月底完成)
	- [x] 发射重写
	- [x] 底盘因子功率分配
	- [x] 小陀螺旋转
	- [x] 陀螺仪spi通信
	- [ ] 视觉通信
	- [x] 裁判系统
	- [ ] 调试运动pid
- [ ] 剩余优化任务
	- [ ] 灯显示车状态
	- [ ] freertos代替定时器
	- [ ] 仿真ros/matlab
	- [ ] 网线通信/can

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

* 底盘运动不正确

原因：数据结算时 vx, vy 不正常，没有用其他数据存放。

总结：以后写代码时注意点，不要犯这种错误。PS: 这玩意最少坑了2周以上，还是寒哥帮忙看出来的。