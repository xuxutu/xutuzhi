### usb总线结构

![image-20210402221057145](/home/zy/.config/Typora/typora-user-images/image-20210402221057145.png)	

### usb otg

主从之间的切换



### usb热插拔之硬件实现



### usb驱动架构

![image-20210402230228297](/home/zy/.config/Typora/typora-user-images/image-20210402230228297.png)	

### usb设备驱动

lsusb 列出系统所有usb设备

lsusb -v 列出设备描述符



### 端点

usb通信的最基本形式

端点只能往一个方向传输数据

端点0既可以发送又可以接收 usb设备端

主机和端点之间的数据是通过管道



### usb传输模式

1.控制传输 获取/配置 设备

2.中断传输 鼠标、键盘 

​	并非硬件中断，不是设备主动发送一个中断请求，而是usb主机控制器在保证不大玉某个时间间隔内（端点描述符内定义）安排一次传输

3.批量传输 u盘

4.等时传输 音频视频 类似udp



### usb request_block urb

1.usb主机控制器和usb外设通信协议

主控制器发送urb的方式：同步方式、异步方式

由主机控制器发送给设备

```
struct urb
{
		pipe//决定发送给谁
		...
}
```

主机控制器发送给设备的过程

1.分配urb

```
struct  urb* usb_alloc_urb(int iso_packets, gfb_t mem_flags)
```

2.初始化urb

```
void usb_fill_control_urb
void usb_fill_int_urb
void usb_fill_bulk_urb
```

3.提交urb

提交给主机控制器，由主机控制器发送给设备

1.异步提交

```
提交完成后执行通过初始化urb函数传入的回调函数
int usb_submit_urb(struct urb* urb, gfp_t mem_flags);
只管发出去，不管成功与否
```

2.同步提交

```
int usb_[control|int|bulk]_msg(struct usb_device* usb_dev, ...)
一直等待urb传输成功
```



### struct usb_device 

描述usb设备



### 管道

主控制器与设备通讯的底层实现

每个端点通过管道和usb主控制器连接，管道包括以下几个部分：

```
1.端点地址
2.数据传输方向（IN 或 OUT）
3.数据传输模式

usb_[rcv|snd|ctrl|int|bulk|isoc]pipe()
接受|发送|

此函数返回一个pipe，它内部是如何实现的？
```



### usb鼠标驱动详解

drivers/hid/usbhid/mouse.c

