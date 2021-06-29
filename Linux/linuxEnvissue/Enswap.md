# 扩展swap空间



## 创建swap文件

sudo fallocate -l 3G /swapfile
只有root用户启用了读写标志
sudo chmod 600 /swapfile
ls -lh /swapfile

## 激活swap

将文件标记为交换空间
sudo mkswap /swapfile

## 启用该交换文件

sudo swapon /swapfile

## 验证交换空间是否可用

free -h

## 永久保留交换文件

备份/etc/fstab文件
sudo cp /etc/fstab /etc/fstab.bak
将swap文件信息添加到/etc/fstab文件的末尾

```
echo '/swapfile none swap sw 0 0' | sudo tee -a /etc/fstab
```

卸载
swapoff /swapfile
卸载后可以删除这个文件，然后重新根据你新的需要创建和调整交换文件大小