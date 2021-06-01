# Raid5搭建



## 参考文章：

https://www.cnblogs.com/tcicy/p/8462271.html

https://blog.csdn.net/yulianxuan/article/details/86575433

https://blog.csdn.net/weixin_48185643/article/details/108233835

https://www.php.cn/linux-369417.html

## 查看版本 

```
cat /etc/issue

```

使用`fdisk`命令列出我们在系统上增加的三个20GB的硬盘。

```
fdisk -l | grep sd
```

现在该检查这三个磁盘是否存在 RAID 块，使用下面的命令来检查。

```
mdadm -E /dev/sd[b-d]
```

## 查看分区

```
lsblk
```



## 1.创建阵列

```
mdadm -Cv /dev/md5 -l5 -n3 -x1 /dev/sd[bcde]
                                                                        大C 创建冗余 
                                                                        v显示过程 
                                                                        l raid级别
                                                                        n raid硬盘数 
                                                                        x热备盘数量
```

## 2.查看同步状态

```
madam -D /dev/md5
或者
cat /proc/mdstat
```



## 3.*挂载 RAID 设备*

### 3.1 格式化

```
mkfs.ext4 /dev/md5
```

![image-20210531105347933](Raid5%E6%90%AD%E5%BB%BA.assets/image-20210531105347933.png)	

### 3.2 创建目录并挂载

在`/mnt`下创建目录 raid5，然后挂载文件系统到 /mnt/raid5/ 下，并检查挂载点下的文件，你会看到 lost+found 目录。

```

    # mkdir /mnt/raid5
    # mount /dev/md0 /mnt/raid5/
    # ls -l /mnt/raid5/
```

### 3.3 验证

在挂载点 /mnt/raid5 下创建几个文件，并在其中一个文件中添加一些内容然后去验证

```

    # touch /mnt/raid5/raid5_tecmint_{1..5}
    # ls -l /mnt/raid5/
    # echo "tecmint raid setups" > /mnt/raid5/raid5_tecmint_1
    # cat /mnt/raid5/raid5_tecmint_1
    # cat /proc/mdstat

```



### 3.4 配置/etc/fstab

我们需要在 fstab 中添加条目，否则系统重启后将不会显示我们的挂载点。编辑 fstab 文件添加条目，在文件尾追加以下行。挂载点会根据你环境的不同而不同。

```

    # vim /etc/fstab
    
    /dev/md0 /mnt/raid5 ext4 defaults 0 0

```

ubutun16.04这样配置会导致无法开机启动，应该按照如下格式来添加：

![image-20210528140720894](Raid5%E6%90%AD%E5%BB%BA.assets/image-20210528140720894.png)	



#### 3.4.1 UUID的获取

// mdadm --detail /dev/md5 | grep uuid 这是配置文件(/etc/mdadm.conf)需要的uuid

**要获取的md5的全局id**

```
blkid /dev/md5  //该指令在格式化之后有效
```



### 3.5 检查配置文件

接下来，运行`mount -av`命令检查 fstab 条目中是否有错误。

```
 mount -av
```



### 3.6 保存 Raid 5 的配置

17、 在前面章节已经说过，默认情况下 RAID 没有配置文件。我们必须手动保存。如果此步中没有跟随不属于 md0 的 RAID 设备，它会是一些其他随机数字。

所以，我们必须要在系统重新启动之前保存配置。如果配置保存它在系统重新启动时会被加载到内核中然后 RAID 也将被加载

```
# mdadm --detail --scan --verbose >> /etc/mdadm.conf
```



## 4.生成配置文件

**（注：如果不生成配置文件。开机raid就会损坏，md5 会变为 md127）**

```
 mdadm -D -s >/etc/mdadm.conf
 生成后需要在文件末尾添加： 
 auto=yes
```



## 5.测试raid5

### 5.1 模拟sdb损坏 -f表示损坏

```
mdadm /dev/md5 -f /dev/sdb
```

### 5.2 拔出模拟损坏的sdb

```
mdadm /dev/md5 -r /dev/sdb 
```

### 5.3 插回模拟损坏的sdb

```
mdadm /dev/md5 -a /dev/sdb
```

### 5.4 raid拉伸 把热备盘sdb也加入磁盘阵列

```
mdadm -G /dev/md5 -n4
```

### 5.5 在线动态格式化 使新加入的sdb容量也被挂在上

```
 resize2fs /dev/md5
```



### 5.6 增加raid成员数量从3变为4个

#### 5.6.1 这里添加设备/dev/sdd到阵列

![image-20210528144930292](Raid5%E6%90%AD%E5%BB%BA.assets/image-20210528144930292.png)	

#### 5.6.2 检查文件的完整性

![image-20210528145024753](Raid5%E6%90%AD%E5%BB%BA.assets/image-20210528145024753.png)	

#### 5.6.3 同步文件系统

![image-20210528145100845](Raid5%E6%90%AD%E5%BB%BA.assets/image-20210528145100845.png)	

#### 5.6.4 再来查看/dev/md5阵列情况

![image-20210528145132475](Raid5%E6%90%AD%E5%BB%BA.assets/image-20210528145132475.png)	



## 6.正确删除raid

### 6.1 先卸载阵列

```
 umount /md5
```

### 6.2 停止raid运行 (*大S stop)

```
mdadm -S /dev/md5
```

### 6.3 删除磁盘

```
 mdadm --misc --zero-superblock /dev/sd[bcdef]
```

### 6.4 删除配置文件

**(*如果fstab或rc.local配置了自动挂载都要删除)**

```
rm -rf /etc/mdadm.conf
```



## 7.禁用和启用

### 7.1 停用raid

![image-20210528144320882](Raid5%E6%90%AD%E5%BB%BA.assets/image-20210528144320882.png)	

​	查看状态确认是否停止成功

​	![image-20210528144409001](Raid5%E6%90%AD%E5%BB%BA.assets/image-20210528144409001.png)

### 7.2 启用raid设备

![image-20210528144454398](Raid5%E6%90%AD%E5%BB%BA.assets/image-20210528144454398.png)	





## 总结

```
创建完raid要等同步到100%后再格式化

raid拉伸后要把拉伸的部分resize2fs格式化

删除时 停止挂载 停止raid 删除硬盘 删配置文件
```

## **涉及目录**

```
/etc/mdadm.conf 要手动生成这个配置文件 里面内容末尾加上auto=yes 否则开机会raid损坏

/proc/mdstat 通过查看这个文件可以监控raid工作状态 几个U代表几个盘在工作
```



## 参数

```
-Cv 创建阵列

-l -n -x 阵列级别 几个硬盘组raid 几个热备盘

-D 查看信息

-D -s 写入配置文件

-f -r -a 模拟损坏 拔除硬盘 插回硬盘

-G -n 将新增的热备盘加入阵列

-S 停止raid
```

