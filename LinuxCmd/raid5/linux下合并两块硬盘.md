# linux下合并两块硬盘

**需求**：
 将两块空硬盘合并为“一块”，挂载到指定目录（/data）下，达到在一个目录使用2块硬盘所有空间的效果。



使用 **fdisk -l** 命令查看当前系统中的硬盘，发现系统中存在两块未分配的硬盘：**/dev/sdb** 和 **/dev/sdc**

现将两块硬盘合并成一块，挂载到/data目录下，具体方法如下：



## 1.分别创建pv

```
pvcreate /dev/sdb

pvcreate /dev/sdc
```



## 2.创建vg

先使用硬盘1创建vg

```
vgcreate lvm_data /dev/sdb
```



## 3.扩展vg

```
vgextend lvm_data /dev/sdc
```





## 4.创建lv

lvcreate -l 100%VG -n vg_data  lvm_data



## 5.格式化分区

mkfs -t ext4 /dev/lvm_data/vg_data 



## 6.挂载分区

```
mount /dev/lvm_data/vg_data /data
```



## 7.设置开机加载

![image-20210527162920298](linux%E4%B8%8B%E5%90%88%E5%B9%B6%E4%B8%A4%E5%9D%97%E7%A1%AC%E7%9B%98.assets/image-20210527162920298.png)	

