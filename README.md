# 蓝牙指纹智能开关锁
> 主控板  STM32F103ZET6 
> HC-05蓝牙模块
> 2.7寸LCD 
> 继电器（1路5V）
> as608指纹识别模块 
> 电磁锁（5V）
> 使用HC05蓝牙模块 实现与上位机的通信 可与电脑端串口调试助手 手机端蓝牙调试助手通信 ，输入预先设定好的密码 可控制继电器的输出 从而达到对电磁锁的控制
> 使用AS608指纹模块 实现指纹模块的增加 删除 对比 等功能 ，用户可进行录指纹 删除指纹 比对指纹 等 ， 可录取255个指纹信息 ，通过比对库中的指纹 达到对继电器的控制 
> FreeRTOS 软定时器 TICKLESS 信号量 ， 通过系统调度来实现同时运作的能力
