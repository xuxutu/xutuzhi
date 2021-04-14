# USB core

## USB tree

![image-20210404160319784](/home/zhaoyong/.config/Typora/typora-user-images/image-20210404160319784.png)	

**USB device 还包括了 hub 和功能设备，也就是上图里的 Func**

### USB controller

在一个 USB 系统中只能有一个 host，其实说白了就是咱们的主 机，而 USB 和主机的接口就是 host controller，一个主机可以支持多个 host controller， 比如分别属于不同厂商的。那么 USB host controller 本身是做什么的？controller，控制 器，顾名思义，用于控制，控制什么，控制所有的 usb 设备的通信。通常计算机的 cpu 并 不是直接和 usb 设备打交道，而是和控制器打交道，他要对设备做什么，他会告诉控制器， 而不是直接把指令发给设备，然后控制器再去负责处理这件事情，他会去指挥设备执行命令， 而 cpu 就不用管剩下的事情，他还是该干嘛干嘛去，控制器替他去完成剩下的事情，事情 办完了再通知 cpu。

### hub

在大学里，有的宿舍里网口有限，但是我们这一代人上大学基本上是每 人一台电脑，所以网口不够，于是有人会使用 hub，让多个人共用一个网口，这是以太网 上的 hub，而 usb 的世界里同样有 hub，其实原理是一样的，任何支持 usb 的电脑不会说 只允许你只能一个时刻使用一个 usb 设备，比如你插入了 u 盘，你同样还可以插入 usb 键 盘，还可以再插一个 usb 鼠标，因为你会发现你的电脑里并不只是一个 usb 接口。这些口 实际上就是所谓的 hub 口。而现实中经常是让一个 usb 控制器和一个 hub 绑定在一起， 专业一点说叫集成，而这个 hub 也被称作 **root hub**，换言之，和 usb 控制器绑定在一起 的 hub 就是系统中最根本的 hub，其它的 hub 可以连接到她这里，然后可以延伸出去，外 接别的设备，当然也可以不用别的 hub，让 usb 设备直接接到 root hub 上。

### USB 连接

USB 连接指的就是连接 device 和 host（或 hub）的四线电缆。电缆中包括 VBUS（电 源线）、GND（地线）还有两根信号线。USB 系统就是通过 VBUS 和 GND 向设备提供电 源的。USB 设备可以使用主机提供的电源，也可以使用外接电源供电。

![image-20210404161304816](/home/zhaoyong/.config/Typora/typora-user-images/image-20210404161304816.png)	

## USB 通信

USB 通信最基本的形式是通过 USB 设备里一个叫 endpoint 的东东，而主机和 endpoint 之间的数据传输是通过 pipe。endpoint 就是通信的发送或者接收点，你要发送数据，那你 只要把数据发送到正确的端点那里就可以了。端点也是有方向的，从 usb 主机到设备称为 out 端点，从设备到主机称为 in 端点。而管道，实际上只是为了让我们能够找到端点，就 相当于我们日常说的邮编地址，比如一个国家，为了通信，我们必须给各个地方取名，完了 给各条大大小小的路取名，比如你要揭发你们那里的官员腐败，你要去上访，你从某偏僻的 小县城出发，要到北京来上访，那你的这个端点(endpoint)就是北京，而你得知道你来北 京的路线，那这个从你们县城到北京的路线就算一条管道。有人好奇的问了，管道应该有两 端吧，一个端点是北京，那另一个端点呢?答案是，这条管道有些特殊，就比如上访，我们 只需要知道一端是北京，而另一端是哪里无所谓，因为不管你在哪里你都得到北京来上访。 没听说过你在山西你可以上访，你要在宁夏还不能上访了，没这事对吧。严格来说，管道的 另一端应该是 usb 主机，即前面说的那个 host，usb 协议里边也是这么说的，协议里边说 pipes 代表着一种能力，怎样一种能力呢，在主机和设备上的端点之间移动数据，听上去挺 玄的。

### 端点

端点不但是有方向的，而且这个方向还是确定的，或者 in，或者 out，没有又是 in 又是 out 的，鱼与熊掌是不可兼得的，脚踩两只船虽然是每个男人的美好愿望，但不具可操作性，也 不提倡。所以你到北京就叫上访，北京的下来就叫慰问，这都是生来就注定的。有没有特殊 的那，看你怎么去理解 0 号端点了，协议里规定了，所有的 USB 设备必须具有端点 0，它 可以作为 in 端点，也可以作为 out 端点，USB 系统软件利用它来实现缺省的控制管道，从 而控制设备。端点也是限量供应的，不是想有多少就有多少的，除了端点 0，低速设备最多 只能拥有 2 个端点，高速设备也最多只能拥有 15 个 in 端点和 15 个 out 端点。这些端点 在设备内部都有唯一的端点号，这个端点号是在设备设计时就已经指定的。

#### 0号端点

为什么端点 0 就非要那么的个性那？这还是有内在原因的。管道的通信方式其实有两种， 一种是 stream 的，一种是 message 的，message 管道要求从它那儿过的数据必须具有 一定的格式，不是随便传的，因为它主要就是用于主机向设备请求信息的，必须得让设备明 白请求的是什么。而 stream 管道就没这么苛刻，要随和多了，它对数据没有特殊的要求。 协议里说，message 管道必须对应两个相同号码的端点，一个用来 in，一个用来 out，咱 们的缺省管道就是 message 管道，当然，与缺省管道对应的端点 0 就必须是两个具有同 样端点号 0 的端点。

### 端点类型

USB endpoint 有四种类型，也就分别对应了四种不同的数据传输方式。它们是控制传输 12 （Control Transfers），中断传输（Interrupt Data Transfers），批量传输(Bulk Data Transfers)，等时传输(Isochronous Data Transfers)。

#### 控制传输

控制传输用来控制对 USB 设备 不同部分的访问，通常用于配置设备，获取设备信息，发送命令到设备，或者获取设备的状 态报告。总之就是用来传送控制信息的，每个 USB 设备都会有一个 endpoint 0 的控制端 点，内核里的 USB core 使用它在设备插入时进行设备的配置。这么说吧，君士坦丁旁边 有非常信赖的这么一个人，往往通过他来对其它人做些监控迫害什么的，虽然他最后被判了 君士坦丁，但我们的 endpoint 0 不会，它会一直在那里等待着 USB core 发送控制命令。 最不忠诚的往往是人心，不是么。

#### 中断传输

中断传输用来以一个固定的速率传送少量的数据，USB 键盘和 USB 鼠标使用的就是这种方 式，USB 的触摸屏也是，传输的数据包含了坐标信息。

#### 批量传输

批量传输用来传输大量的数据，确保没有数据丢失，并不保证在特定的时间内完成。U 盘使 用的就是批量传输，咱们用它备份数据时需要确保数据不能丢，而且也不能指望它能在一个 固定的比较快的时间内拷贝完。

#### 等时传输

等时传输同样用来传输大量的数据，但并不保证数据是否到达，以稳定的速率发送和接收实 时的信息，对传送延迟非常敏感。显然是用于音频和视频一类的设备，这类设备期望能够有 个比较稳定的数据流，比如你在网上 QQ 视频聊天，肯定希望每分钟传输的图像/声音速率 是比较稳定的，不能说这一分钟对方看到你在向她向你深情表白，可是下一分钟却看见画面 停滞在那里，只能看到你那傻样一动不动，你说这不浪费感情嘛。

## USB 系统实现

![image-20210404162930717](/home/zhaoyong/.config/Typora/typora-user-images/image-20210404162930717.png)	

一个完整的 USB 系统应该实现上面图里的各个部分，USB 协议如是说。图里主要显示了四个层次，USB 物理设备（USB Physical Device）、客户软件（Client SW）、 USB 系统软件（USB System SW）以及 USB Host Controller。Host Controller 已经 说过了，系统软件就是操作系统里用来支持 USB 的部分，像咱们的 usb core，还有各种 设备驱动等等，客户软件么，就是上层应用了，只有设备和驱动程序，我们仍然什么都做不 了，现实生活中一个很浅显的道理就是只靠摄像头和驱动是不可能和 mm 视频的，不是么， 这是个应用为王的时代。

实际上

上面的**系统软件**，只表示了系统里支持 USB 的部 分，也就是系统相关设备无关的部分，相对于咱们的 linux 来说，就是 usb core，并不包 括所谓的各种设备驱动。而**客户软件**则指设备相关，也就是对应于特定设备的部分，你的 USB 键盘驱动、触摸屏驱动什么的都在这儿。这里的名字太迷惑人了，一直觉得写驱动是系统级的编程，原来搞协议的这些同志觉得不是这么回事，我羞愧的低下了无知的脑袋。

主机这边就分这三层，**Host Controller** 看似在最低层，却掌控着整个 USB 的通信，你的 USB 设备要想发挥作用，首先得获得它的批准，此路是它开，要想从此过，留下买路财。 我们也在最底层，不同的是被掌握，不同的角色决定了不同的命运。

**USB 物理设备**这边看着好像也分了三层，其实我们可以把它们看成一样的东东，只是为了 对应了主机这边的不同层次，Host Controller 看到的是一个个 hub 还有 hub 上的 USB device，而在系统软件的眼里没那么多道道儿，hub 还有各种设备什么的都是一个个的 usb 逻辑设备，客户软件看到的是设备提供的功能。接下来还会有说到。站在不同的高度看到不 同的风景，不然为什么买房子时高一层要加多少钱那。

已经被计算机网络中的七层协议洗过脑的我们应该很容易的就看出，**真实的数据流**只发生在 **Host Controller 和设备的 Bus Interface** 之间，其它的都是**逻辑上的，也就说是虚的**，如 果谁对我们说什么什么是逻辑上存在的，那它肯定就是虚的，比如说任小强逻辑上给你了一 套北京的房子，你相信么，给是给，得掏钱，而且还得掏的多。

各种 USB 设备提供的功能是不同的，但是面向主机的 **Bus Interface** 却是一致的，**主机也 不是神仙**，掐指一算就可以知道哪个是哪个，所以，那些设**备本身还必须要提供用来确认自 己身份的信息**，这些信息里有些是共有的，有些是个别设备特有的，我们都是光荣的中国公 民，但是有的人是卖房子，有的人买房子。

### compound device

compound device 是那些将 hub 和连在 hub 上的设备封装 在一起所组成的设备

### composite device

是包含彼此独立的多个接口的设备

**从主机 的角度看**，一个 compound device 和单独的一个 hub 然后连接了多个 USB 设备是一样 的，它里面包含的 hub 和各个设备都会有自己独立的地址，而一个 composite device 里 不管具有多少接口，它都只有一个地址。

![image-20210404164633663](/home/zhaoyong/.config/Typora/typora-user-images/image-20210404164633663.png)	

上面都是站在 host controller 的层次上，说的是实实在在的物理拓扑，对于系统软件来说， 没有这么复杂，所有的 hub 和设备都被看作是一个个的逻辑设备，好像它们本来就直接连 接在 root hub 上一样。站的越高，看的越远，快乐如此简单，可以做售楼广告了。

### USB逻辑设备

一个 USB 逻辑设备就是一群端点（endpoint）的集合，它与主机之间的通信发生在主机上 的一个缓冲区和设备上的一个端点之间，通过管道来传输数据。意思就是管道的一端是主机 上的一个缓冲区，一端是设备上的端点。

![image-20210404164839821](/home/zhaoyong/.config/Typora/typora-user-images/image-20210404164839821.png)	

### Interface 

图里的 Interface 是怎么回事？这里先简单说说吧，反正代码里会不停的遇到再遇到。USB 端点被捆绑为接口（Interface），一个接口代表一个基本功能。有的设备具有多个接口，像 USB 扬声器就包括一个键盘接口和一个音频流接口。在内核里**一个接口要对应一个驱动程 序**，USB 扬声器在 linux 里就需要两个不同的驱动程序。到目前为止，可以这么说，**一个 设备可以包括多个接口**，**一个接口可以具有多个端点**，当然以后我们会发现并不仅仅止于此。 不过先说这么多吧，省得说得慷慨激昂，看的昏昏欲睡。



## drivers/usb

### usb core

Linux 内核开发者们，专门写了一些代码， 负责实现一些核心的功能，为别的 设备驱动程序提供服务，比如申请内存，比如实现一些所有的设备都会需要的公共的函数， 并美其名曰 usb core。

在 drivers/usb/目录下面出来了一个 core 目录，就专门放一些核 心的代码，比如初始化整个 usb 系统，初始化 root hub，初始化 host controller 的代码， 再后来甚至把 host controller 相关的代码也单独建了一个目录，叫 host 目录，这是因为 usb host controller 随着时代的发展，也开始有了好几种，不再像刚开始那样只有一种， 所以呢，设计者们把一些 host controller 公共的代码仍然留在 core 目录下，而一些各 host controller 单独的代码则移到 host 目录下面让负责各种 host controller 的人去维护。

usb core 负责实现一些核心的功能，为别的设备驱动程序提供服务，提供一个用于访问和 20 控制 USB 硬件的接口，而不用去考虑系统当前存在哪种 host controller。

### usb gadget

gadget 白了说就是配件的意思，主要就是一些内部运行 linux 的嵌 入式设备，如 PDA，设备本身有 USB 设备控制器（usb device controller），可以将 PC， 也就是我们的 host 作为 master 端，将这样的设备作为 slave 端和 PC 通过 USB 进行通 信。从 host 的观点来看，主机系统的 USB 驱动程序控制插入其中的 USB 设备，而 usb gadget 的驱动程序控制外围设备如何作为一个 USB 设备和主机通信。比如，我们的嵌入 式板子上支持 SD 卡，如果我们希望在将板子通过 USB 连接到 PC 之后，这个 SD 卡被模 拟成 U 盘，那么就要通过 usb gadget 架构的驱动。

gadget 目录下大概能够分为两个模块，一个是 udc 驱动，这个驱动是针对具体 cpu 平台 的，如果找不到现成的，就要自己实现。另外一个就是 gadget 驱动，主要有 file_storage、 ether、serial 等。另外还提供了 USB gadget API，即 USB 设备控制器硬件和 gadget 驱动通信的接口。PC 及服务器只有 USB 主机控制器硬件，它们并不能作为 USB gadget 存在，而对于嵌入式设备，USB 设备控制器常被集成到处理器中，设备的各种功能，如 U 盘、网卡等，常依赖这种 USB 设备控制器来与主机连接，并且设备的各种功能之间可以切 换，比如可以根据选择作为 U 盘或网卡等。

### 其他目录

剩下的几个目录分门别类的放了各种 USB 设备的驱动，U 盘的驱动在 storage 目录下，触 摸屏和 USB 键盘鼠标的驱动在 input 目录下，等等。多说一下的是，Usb 协议中，除了通 用的软硬件电气接口规范等，还包含了各种各样的 Class 协议，用来为不同的功能定义各 自的标准接口和具体的总线上的数据交互格式和内容。这些 Class 协议的数量非常多，最 常见的比如支持 U 盘功能的 Mass Storage Class，以及通用的数据交换协议：CDC class。 此外还包括 Audio Class, Print Class 等等。理论上说，即使没有这些 Class，通过专用 驱动也能够实现各种各样的应用功能。但是，正如 Mass StorageClass 的使用，使得各 19 个厂商生产的 U 盘都能通过操作系统自带的统一的驱动程序来使用，对 U 盘的普及使用起 了极大的推动作用，制定其它这些 Class 也是为了同样的目的。

![image-20210404170850641](/home/zhaoyong/.config/Typora/typora-user-images/image-20210404170850641.png)

driver 和 host controller 像不像 core 的两个保镖？没办法，这可是 core 啊。协议里也 说了，host controller 的驱动（HCD）必须位于 USB 软件的最下一层，任小强们也说了， 咱们必须位于房地产这个链子的最下一层。HCD 提供 host controller 硬件的抽象，隐藏 硬件的细节，在 host controller 之下是物理的 USB 及所有与之连接的 USB 设备。而 HCD 只有一个客户，对一个人负责，就是咱们的 USB core，USB core 将用户的请求映射到相 关的 HCD，用户不能直接访问 HCD。 咱们写 USB 驱动的时候，只能调用 core 的接口，core 会将咱们的请求发送给相应的 HCD， 用得着咱们操心的只有这么一亩三分地，core 为咱们完成了大部分的工作，linux 的哲学 是不是和咱们生活中不太一样那？

## Kconfig

### usbfs 

usbfs 文件系统挂载在/proc/bus/usb 上(mount -t usbfs none /proc/bus/usb)，显示了当前连接的 USB 设备及总线的各种信息，每个连接 的 USB 设备在其中都会有一个文件进行描述。比如文件/proc/bus/usb/xxx/yyy，xxx 表示总线的序号，yyy 表示设备在总线的地址，不过不能够依赖它们来稳定地访问设备，因 为同一设备两次连接对应的描述文件可能会不同，比如，第一次连接一个设备时，它可能是 002/027，一段时间后再次连接，它可能就已经改变为 002/048。就好比好不容易你暗恋 的 mm 今天见你的时候对你抛了个媚眼，你心花怒放，赶快去买了 100 块彩票庆祝，到第 二天再见到她的时候，她对你说你是谁啊，你悲痛欲绝的刮开那 100 块彩票，上面清一色 的谢谢你，谢谢你送钱。usbfs 可以开个专题来讨论了，以后的日子里并不会花更多的口舌 在它上面。

### USB_SUSPEND 

这一项是有关 usb 设备的挂起和恢复。开发 USB 的人都是节电节能的好孩子，所以协议里 就规定了，所有的设备都必须支持挂起状态，就是说为了达到节电的目的，当设备在指定的 时间内，3ms 吧，如果没有发生总线传输，就要进入挂起状态。当它收到一个 non-idle 的 信号时，就会被唤醒。在这里呼吁一下，多利用利用有太阳的时候，少熬夜，又费电对身体 又不好，不过，我应该明天才说这句话，因为半夜还有米兰的冠军杯那。节约用电从 USB 做起。不过目前来说内核对挂起休眠的支持普遍都不太好，而且许多的 USB 设备也没有支 持它，还是暂且不表了。

## Makefile

Makefile可比Kconfig简略多了，所以看起来也更亲切点，咱们总是拿的money越多越好， 看的代码越少越好。这里之所以会出现CONFIG_PCI，是因为通常USB的root hub包含在 一个PCI设备中，前面也已经聊过了。hcd-pci和hcd顾名而思义就知道是说host controller 的，它们实现了host controller公共部分，按协议里的说法它们就是HCDI（HCD的公共 接口），host目录下则实现了各种不同的host controller，咱们这里不怎么会聊到具体host controller的实现。CONFIG_USB_DEVICEFS前面的Kconfig文件里也见到了，关于 usbfs的，已经说了这里不打算过多关注它，所以 inode.c和 devices.c两个文件也可以不 用管了。 这么看来，好像 core 下面的代码几乎都需要关注的样子，并没有减轻多少压力，不过要知 道，这里本身就是 usb 的 core 部分，是要做很多的事为咱们分忧的，所以多点也是可以 理解的。



## drivers/usb/core/usb.c

```
938 subsys_initcall(usb_init);
939 module_exit(usb_exit)
```

我们看到一个 subsys_initcall，它是一个宏，我们可以把它理解为module_init，只不过 因为这部分代码比较核心，开发者们把它看作一个子系统，而不仅仅是一个模块，这也很好 理解，usbcore这个模块它代表的不是某一个设备，而是所有usb设备赖以生存的模块， Linux中，像这样一个类别的设备驱动被归结为一个子系统。比如pci子系统，比如scsi子系 统，基本上，drivers/目录下面第一层的每个目录都算一个子系统，因为它们代表了一类设 备。subsys_initcall(usb_init)的意思就是告诉我们usb_init是我们真正的初始化函数， 而usb_exit()将是整个usb子系统的结束时的清理函数，于是我们就从usb_init开始看起。 至于子系统在内核里具体的描述，牵涉到linux设备模型了，可以去看ldd3，或者更详细的。 目前来说，我们只需要知道子系统通常显示在sysfs分层结构中的顶层，比如块设备子系统 24 对应/sys/block，当然也不一定，usb子系统对应的就是/sys/bus/usb。

```
860 /*
861 * Init
862 */
863 static int __init usb_init(void)
864 {
865 int retval;
866 if (nousb) {
867 pr_info("%s: USB support disabled\n", usbcore_name);867 行，pr_info只是一个打印信息的宏，printk的变体，在 include/linux/kernel.h里定
义
868 return 0;
869 }

866 行，知道 C 语言的人都会知道 nousb 是一个标志，只是不同的标志有不一样的精彩，
这里的 nousb 是用来让我们在启动内核的时候通过内核参数去掉 USB 子系统的，linux 社
会是一个很人性化的世界，它不会去逼迫我们接受 USB，一切都只关乎我们自己的需要。
不过我想我们一般来说是不会去指定 nousb 的吧，毕竟它那么的讨人可爱。如果你真的指
定了 nousb，那它就只会幽怨的说一句“USB support disabled”，然后退出 usb_init。

870
871 retval = ksuspend_usb_init();//871 行，电源管理方面的。如果在编译内核时没有打开电源管理，也就是说没有定义
CONFIG_PM，它就什么也不做。
872 if (retval)
873 goto out;
874 retval = bus_register(&usb_bus_type);//874 行，注册 USB 总线，只有成功的将 USB 总线子系统注册到系统中，我们才可以向这
个总线添加 USB 设备。基于它显要的江湖地位，就拿它做为日后突破的方向了，擒贼先擒
王，这个越老越青春的道理在 linux 中也是同样适用的。
875 if (retval)
876 goto bus_register_failed;
877 retval = usb_host_init();//执行 host controller 相关的初始化。
878 if (retval)
879 goto host_init_failed;
880 retval = usb_major_init();//一个实际的总线也是一个设备，必须单独注册，因为 USB 是通过快速串行通信来
读写数据，这里把它当作了字符设备来注册。
881 if (retval)
882 goto major_init_failed;
883 retval = usb_register(&usbfs_driver);
884 if (retval)
885 goto driver_register_failed;
886 retval = usb_devio_init();
887 if (retval)
888 goto usb_devio_init_failed;
889 retval = usbfs_init();
890 if (retval)
891 goto fs_init_failed;
//883~891 行，都是 usbfs 相关的初始化。
892 retval = usb_hub_init();//hub 的初始化，这个某人讲了。
893 if (retval)
894 goto hub_init_failed;
895 retval = usb_register_device_driver(&usb_generic_driver,
THIS_MODULE);//注册 USB device driver，戴好眼镜看清楚了，是 USB device driver 而不是
USB driver，前面说过，一个设备可以有多个接口，每个接口对应不同的驱动程序，这里
所谓的 device driver 对应的是整个设备，而不是某个接口。内核里结构到处有，只是 USB
这儿格外多。
896 if (!retval)
897 goto out;
898
899 usb_hub_cleanup();
900 hub_init_failed:
901 usbfs_cleanup(); 
902 fs_init_failed:
903 usb_devio_cleanup();
904 usb_devio_init_failed:
905 usb_deregister(&usbfs_driver);
906 driver_register_failed:
907 usb_major_cleanup();
908 major_init_failed:
909 usb_host_cleanup();
910 host_init_failed:
911 bus_unregister(&usb_bus_type);
912 bus_register_failed:
913 ksuspend_usb_cleanup();
914 out:
915 return retval;
916 } 
```

### __init

看到上面定义里的__init 标记没，写过驱动的应该不会陌生，它对内核来说就是一种暗示， 表明这个函数仅在初始化期间使用，在模块被装载之后，它占用的资源就会释放掉用作它处。 它的暗示你懂，可你的暗示，她却不懂或者懂装不懂，多么让人感伤。它在自己短暂的一生 中一直从事繁重的工作，吃的是草吐出的是牛奶，留下的是整个 USB 子系统的繁荣。

对__init 的定义在 include/linux/init.h 里

```
43 #define __init __attribute__ ((__section__ (".init.text"))) 
```

#### __attribute__

而 __attribute__就是这些扩展中的一个， 它主要被用来声明一些特殊的属性，这些属性主要被用来指示编译器进行特定方面的优化和 更仔细的代码检查。GNU C支持十几个属性，section是其中的一个，我们查看gcc的手册 可以看到下面的描述

通常编译器将函数放在.text 节，变量放在.data 或 .bss 节，使用 section 属性，可以让 编译器将函数或变量放在指定的节中。那么前面对__init 的定义便表示将它修饰的代码放 在.init.text 节。连接器可以把相同节的代码或数据安排在一起，比如__init 修饰的所有代 码都会被放在.init.text 节里，初始化结束后就可以释放这部分内存。

那内核又是如何调用到这些__init 修饰的初始化函数那？

要回答这个问题，还需要回顾一下上面 938 行的代码，那里已经提到 subsys_initcall也是 一个宏，它也在include/linux/init.h里定义 125 #define subsys_initcall(fn) __define_initcall("4",fn,4) 这里又出现了一个宏 __define_initcall，它是用来将指定的函数指针fn放到initcall.init节 里，也在include/linux/init.h文件里定义，这里就不多说了，有那点意思就可以了。而对 于具体的 subsys_initcall宏，则是把fn放到.initcall.init的子节.initcall4.init里。要弄清 楚.initcall.init、.init.text和.initcall4.init这样的东东，我们还需要了解一点内核可执行文 件相关的概念。 内核可执行文件由许多链接在一起的对象文件组成。对象文件有许多节，如文本、数据、init 数据、bass 等等。这些对象文件都是由一个称为链接器脚本的文件链接并装入的。这个链 接器脚本的功能是将输入对象文件的各节映射到输出文件中；换句话说，它将所有输入对象 文件都链接到单一的可执行文件中，将该可执行文件的各节装入到指定地址处。 vmlinux.lds 是存在于 arch// 目录中的内核链接器脚本，它负责链接内核的各 个节并将它们装入内存中特定偏移量处。 涉及到的东西越来越多了是吧，先深呼吸，平静一下，坚定而又勇敢的打开 arch/i386/kernel/vmlinux.lds 文件，你就会见到前所未见的景象。我可以负责任的说， 要看懂这个文件是需要一番功夫的，不过大家都是聪明人，聪明人做聪明事，所以你需要做 的只是搜索 initcall.init，然后便会看到似曾相识的内容

```
__inicall_start = .;
.initcall.init : AT(ADDR(.initcall.init) – 0xC0000000) {
*(.initcall1.init) 
 27
*(.initcall2.init)
*(.initcall3.init)
*(.initcall4.init)
*(.initcall5.init)
*(.initcall6.init)
*(.initcall7.init)
}
```

这里的__initcall_start 指向.initcall.init 节的开始，__initcall_end 指向它的结尾。 而.initcall.init 节又被分为了 7 个子节，分别是

```
.initcall1.init
.initcall2.init
.initcall3.init
.initcall4.init
.initcall5.init
.initcall6.init
.initcall7.init 
```

我们的 subsys_initcall宏便是将指定的函数指针放在了.initcall4.init子节。其它的比如 core_initcall 将函数指针放在 .initcall1.init 子节， device_initcall 将函数指针放在 了.initcall6.init子节等等，都可以从include/linux/init.h文件找到它们的定义。各个子节 的顺序是确定的，即先调用.initcall1.init中的函数指针再调用.initcall2.init中的函数指针， 等等。__init修饰的初始化函数在内核初始化过程中调用的顺序和.initcall.init节里函数指 针的顺序有关，不同的初始化函数被放在不同的子节中，因此也就决定了它们的调用顺序。 至于实际执行函数调用的地方，就在/init/main.c 文件里，内核的初始化么，不在那里还 能在哪里，里面的 do_initcalls 函数会直接用到这里的__initcall_start、__initcall_end 来进行判断，不多说了。我的思念已经入滔滔江水泛滥成灾了，还是回到久违的 usb_init 函数吧。

## 

## linux 的设备模型

总线是如何发现设备的，设备又是如何 和驱动对应起来的，它们经过怎样的艰辛才找到命里注定的那个他，它们的关系如何，白头 偕老型的还是朝三暮四型的，这些问题就不是他们关心的了，是咱们需要关心的。经历过高 考千锤百炼的咱们还能够惊喜的发现，这些疑问的中心思想中心词汇就是总线、设备和驱动， 没错，它们都是咱们这里要聊的 linux 设备模型的名角。

### 总线：struct bus_type 

当然，struct bus_type 中的drivers和devices分别表示了这个总线拥有哪些设备和哪些驱动

```
52 struct bus_type {
53 const char * name;
54 struct module * owner;
55
56 struct kset subsys;
57 struct kset drivers;
58 struct kset devices;
59 struct klist klist_devices;
60 struct klist klist_drivers;
61
62 struct blocking_notifier_head bus_notifier;
63
64 struct bus_attribute * bus_attrs;
65 struct device_attribute * dev_attrs;
66 struct driver_attribute * drv_attrs;
67 struct bus_attribute drivers_autoprobe_attr;
68 struct bus_attribute drivers_probe_attr;
69
70 int (*match)(struct device * dev, struct device_driver *
drv);
71 int (*uevent)(struct device *dev, char **envp,
72 int num_envp, char *buffer, int
buffer_size);
73 int (*probe)(struct device * dev);
74 int (*remove)(struct device * dev);
75 void (*shutdown)(struct device * dev);
76
77 int (*suspend)(struct device * dev, pm_message_t state);
78 int (*suspend_late)(struct device * dev, pm_message_t state);
79 int (*resume_early)(struct device * dev);
80 int (*resume)(struct device * dev);
81
82 unsigned int drivers_autoprobe:1;
83 }; 
```

### 设备：struct device 

struct device中的bus表示这个设备连到哪个总线上， driver表示这个设备的驱动是什么

```
410 struct device {
411 struct klist klist_children;
412 struct klist_node knode_parent; /* node in sibling list */
413 struct klist_node knode_driver;
414 struct klist_node knode_bus;
415 struct device *parent;
416
417 struct kobject kobj; 
 32
418 char bus_id[BUS_ID_SIZE]; /* position on parent bus */
419 struct device_type *type;
420 unsigned is_registered:1;
421 unsigned uevent_suppress:1;
422 struct device_attribute uevent_attr;
423 struct device_attribute *devt_attr;
424
425 struct semaphore sem; /* semaphore to synchronize calls to
426 * its driver.
427 */
428
429 struct bus_type * bus; /* type of bus device is on */
430 struct device_driver *driver; /* which driver has allocated this
431 device */
432 void *driver_data; /* data private to the driver */
433 void *platform_data; /* Platform specific data, device
434 core doesn't touch it */
435 struct dev_pm_info power;
436
437 #ifdef CONFIG_NUMA
438 int numa_node; /* NUMA node this device is close to */
439 #endif
440 u64 *dma_mask; /* dma mask (if dma'able device) */
441 u64 coherent_dma_mask;/* Like dma_mask, but for
442 alloc_coherent mappings as
443 not all hardware supports
444 64 bit addresses for consistent 
445 allocations such descriptors. */
446
447 struct list_head dma_pools; /* dma pools (if dma'ble) */
448
449 struct dma_coherent_mem *dma_mem; /* internal for coherent mem
450 override */
451 /* arch specific additions */
452 struct dev_archdata archdata;
453
454 spinlock_t devres_lock;
455 struct list_head devres_head;
456
457 /* class_device migration path */
458 struct list_head node;
459 struct class *class;
460 dev_t devt; /* dev_t, creates the sysfs "dev" */
461 struct attribute_group **groups; /* optional groups */ 
462
463 void (*release)(struct device * dev);
464 }; 
```

### 驱动：struct device_driver 

struct device_driver中的bus表示这个驱动属于哪个 34 总线，klist_devices表示这个驱动都支持哪些设备，因为这里device是复数，又是list， 因为一个驱动可以支持多个设备，而一个设备只能绑定一个驱动。

```
124 struct device_driver {
125 const char * name;
126 struct bus_type * bus;
127
128 struct kobject kobj;
129 struct klist klist_devices;
130 struct klist_node knode_bus;
131
132 struct module * owner;
133 const char * mod_name; /* used for built-in modules */
134 struct module_kobject * mkobj;
135
136 int (*probe) (struct device * dev);
137 int (*remove) (struct device * dev);
138 void (*shutdown) (struct device * dev);
139 int (*suspend) (struct device * dev, pm_message_t state);
140 int (*resume) (struct device * dev);
141 }; 
```

#### kobject

我可以肯定的告诉 你，kobject和kset都是linux设备模型中最基本的元素，总线、设备、驱动是西瓜，**kobjcet、 klist** 是种瓜的人，没有幕后种瓜人的汗水不会有清爽解渴的西瓜，我们不能光知道西瓜的 的甜，还要知道种瓜人的辛苦。kobject 和 kset 不会在意自己自己的得失，它们存在的意 义在于**把总线、设备和驱动这样的对象连接到设备模型上**。种瓜的人也不会在意自己的汗水， 在意的只是能不能送出甜蜜的西瓜。

一般来说应该这么理解，整个 linux 的设备模型是一个 OO 的体系结构，总线、设备和驱动 都是其中鲜活存在的对象，**kobject 是它们的基类**，所实现的只是一些公共的接口，**kset 是同种类型 kobject 对象的集合**，也可以说是对象的容器。只是因为 C 里不可能会有 C++ 里类的 class 继承、组合等的概念，只有通过 **kobject 嵌入到对象结构里来实现**。这样， **内核使用 kobject 将各个对象连接起来组成了一个分层的结构体系**，就好像通过马列主义 将我们 13 亿人也连接成了一个分层的社会体系一样。**kobject 结构里包含了 parent 成员， 指向了另一个 kobject 结构，也就是这个分层结构的上一层结点**。而 kset 是通过链表来实 现的，这样就可以明白，struct bus_type 结构中的成员 drivers 和 devices 表示了一条 总线拥有两条链表，一条是设备链表，一条是驱动链表。我们知道了总线对应的数据结构， 就可以找到这条总线关联了多少设备，又有哪些驱动来支持这类设备。

那么**klist**那？其实它就包含了一个链表和一个自旋锁，我们暂且把它看成链表也无妨，本 来在早先的内核版本里，struct device_driver结构的devices成员就是一个链表类型。

![image-20210404184845696](/home/zhaoyong/.config/Typora/typora-user-images/image-20210404184845696.png)	

### 总线中的两条链表的形成

这要求每次出现一个设备就要向总线汇报，或者说注册，每次出现 一个驱动，也要向总线汇报，或者说注册。比**如系统初始化的时候，会扫描连接了哪些设备， 并为每一个设备建立起一个 struct device 的变量，每一次有一个驱动程序，就要准备一个 struct device_driver 结构的变量。把这些变量统统加入相应的链表，device 插入 devices 链表，driver 插入 drivers 链表。**这样通过总线就能找到每一个设备，每一个驱 动。然而，假如计算机里只有设备却没有对应的驱动，那么设备无法工作。反过来，倘若只 有驱动却没有设备，驱动也起不了任何作用。在他们遇见彼此之前，双方都如同路埂的野草， 一个飘啊飘，一个摇啊摇，谁也不知道未来在哪里，只能在生命的风里飘摇。于是总线上的 两张表里就慢慢的就挂上了那许多孤单的灵魂。devices 开始多了，drivers 开始多了，他 们像是两个来自世界，devices 们彼此取暖，drivers 们一起狂欢，但他们有一点是相同的， 都只是在等待属于自己的那个另一半。

### 设备与驱动的绑定

现在，总线上的两条链表已经有了，这个三角关系三个边已经有了两个，剩下的那个那？链 表里的 device 和 driver 又是如何联系那？先有 device 还是先有 driver？很久很久以前， 在那激情燃烧的岁月里，先有的是 device，每一个要用的 device 在计算机启动之前就已 经插好了，插放在它应该在的位置上，然后计算机启动，然后操作系统开始初始化，总线开 始扫描设备，每找到一个设备，就为其申请一个 struct device 结构，并且挂入总线中的 devices 链表中来，然后每一个驱动程序开始初始化，开始注册其 struct device_driver 结构，然后它去总线的 devices 链表中去寻找(遍历)，去寻找每一个还没有绑定 driver 的 设备，即 struct device 中的 struct device_driver 指针仍为空的设备，然后它会去观察 这种设备的特征，看是否是他所支持的设备，如果是，那么调用一个叫做 device_bind_driver 的函数，然后他们就结为了秦晋之好。换句话说，把 struct device 中的 struct device_driver driver 指向这个 driver，而 struct device_driver driver 把 struct device 加入他的那张 struct klist klist_devices 链表中来。就这样，bus、device 和 driver，这三者之间或者说他们中的两两之间，就给联系上了。知道其中之一，就能找 到另外两个。一荣俱荣，一损俱损。

![image-20210404192043392](/home/zhaoyong/.config/Typora/typora-user-images/image-20210404192043392.png)

但现在情况变了，在这红莲绽放的日子里，在这樱花伤逝的日子里，出现了一种新的名词， 叫热插拔。device 可以在计算机启动以后在插入或者拔出计算机了。因此，很难再说是先 有 device 还是先有 driver 了。因为都有可能。device 可以在任何时刻出现，而 driver 也 可以在任何时刻被加载，所以，出现的情况就是，**每当一个 struct device 诞生，它就会去 bus 的 drivers 链表中寻找自己的另一半，反之，每当一个一个 struct device_driver 诞 生，它就去 bus 的 devices 链表中寻找它的那些设备。如果找到了合适的，那么 ok，和之 前那种情况一下，调用 device_bind_driver 绑定好。如果找不到，没有关系，等待吧， 等到昙花再开，等到风景看透，心中相信，这世界上总有一个人是你所等的，只是还没有遇 到而已。**

### Linux 设备模型中的总线

Linux 设备模型中的总线落实在 USB 子系统里就是 usb_bus_type，它在 usb_init 函数 的 874 行注册，在 driver.c 文件里定义

```
1523 struct bus_type usb_bus_type = {
1524 .name = "usb",
1525 .match = usb_device_match,//match这个函数指针
就比较有意思了，它充当了一个红娘的角色，在总线的设备和驱动之间牵线搭桥
1526 .uevent = usb_uevent,
1527 .suspend = usb_suspend,
1528 .resume = usb_resume,
1529 }; 
```

#### usb_device_match

```
540参数我们都已经很熟悉了，对应的就是总线两条
链表里的设备和驱动，也可以说是鹊桥版上的挂牌的和摘牌的。总线上有新设备或新的驱动
添加时，这个函数总是会被调用，如果指定的驱动能够处理指定的设备，也就是匹配成功，
函数返回 0。梦想是美好的，现实是残酷的，匹配是未必成功的，红娘再努力，双方对不上
眼也是实在没办法的事。
540 static int usb_device_match(struct device *dev, struct device_driver *drv)
541 {
542 /* devices and interfaces are handled separately */

543 if (is_usb_device(dev)) {
544
545 /* interface drivers never match devices */
546 if (!is_usb_device_driver(drv))
547 return 0;
548
549 /* TODO: Add real matching code */
550 return 1;
551
552 } else {
553 struct usb_interface *intf;
554 struct usb_driver *usb_drv;
555 const struct usb_device_id *id;
556
557 /* device drivers never match interfaces */
558 if (is_usb_device_driver(drv))
559 return 0;
560
561 intf = to_usb_interface(dev);
562 usb_drv = to_usb_driver(drv);
563
564 id = usb_match_id(intf, usb_drv->id_table);
565 if (id)
566 return 1;
567
568 id = usb_match_dynamic_id(intf, usb_drv);
569 if (id) 
570 return 1;
571 }
572
573 return 0;
574 } 
```

USB的世界里，设备和驱动不会有向左走向右走那般的遗憾，缘分对于我们来说是不可言 传的玄机，对于它们来说只是 usb_device_match函数的两端。usb_device_match函数 为它们指明向左走还是向右走，为它们指明哪个才是它们命中注定的缘，而我们的生活里却 不会有这样的一个角色，所以只能有无数的“如果你尝试换个方向，你可能发现，一切原来 都不一样。”这样的假设。

543 行，第一次遇到这个函数的时候，我说了这里有两条路，一条给 USB 设备走，一条给 USB 接口走。前面站在这个路口上，将两条路上可能遇到的主要角色大都介绍了下，它们 类似于影片刚开幕的演员列表里的领衔主演，总是会谋杀比较多的菲林。先来看看设备走的 这条路，上面儿只有两个函数，好像要平坦的多，可以一下走到底儿的样子。

```
85 static inline int is_usb_device(const struct device *dev)
86 {
87 return dev->type == &usb_device_type;
88 } 
```

usb.h 里定义的这个函数看长相就是个把门儿的角色，好像那些所谓高尚小区的门卫，看到 你和你同事分别开一辆宝来和奥拓要进去，气势汹汹的过来告你，宝来才能进，奥拓一边儿 去。这个函数就是要告诉你，是 usb_device 才能打这儿过，否则一边儿该干吗干吗去。 但关键问题不是它让不让你进，而是它怎么知道你是不是 usb_device，那些门卫起码也得 认识啥是宝来啥是奥拓啊，否则直接拿奥拓充宝来不就可以开进去了，现在可是 21 世纪， 假的比真的还流行的时代，塞几块硅胶就成性感女神流行代名词儿了。所以咱们要了解这里 是怎么做到真的假不了的，关键就在于这个 dev->type，设备的类型，看它是不是等于咱 们定义的 usb_device_type，也就是说 usb 设备类型，相等的话，那好说，他躬送你进去， 不相等，那就此路不是为你开的，你找别的路吧。usb_device_type 在 usb.c 里定义

##### usb 设备类型

```
195 struct device_type usb_device_type = {
196 .name = "usb_device",
197 .release = usb_release_dev,
198 }; 
```

它和咱们前面看到的那个 usb_bus_type差不多，一个表示总线的类型，一个表示设备的 类型，总线有总线的类型，设备有设备的类型，就好像在那个红色中国的时期里，人有很多 种成分儿，地主老财就不能混到贫下中农的革命队伍里，当时俺爷爷有几亩薄田，身子骨儿 薄，找了几个人帮着种下，结果就成了剥削劳动人民血汗的地主老财，俺姥爷挑着个剃头担 子走街串巷，帮人剃头谋几个小钱，结果被打到划成小手工业劳动者，他们顶着成分不好的 帽子含辛茹苦好多年，做梦都想与贫下中农靠的近点儿再近点儿，可现在就不同了，谁还愿 意去做贫下中农。 假设现在过来一个设备，经过判断，它要走的是设备这条路，可问题是，这个设备的 **type 字段啥时候被初始化成 usb_device_type**了。嗯，这到是个问题，不过先按下它不表，继 71 续向前走，带着疑问上路。

546 行，又见到一个 if，一个把门儿的，这年头儿，就是关关卡卡的多，走个人民的公路还
有许多人民的收费站来收人民的钱。先看看它是干吗的，收的合理不合理。它就跟在上面的
is_usb_device 函数后面在 usb.h 文件里定义

```
90 /* Do the same for device drivers and interface drivers. */
91
92 static inline int is_usb_device_driver(struct device_driver *drv)
93 {
94 return container_of(drv, struct usbdrv_wrap, driver)->
95 for_devices;
96 } 
```

这个函数的脸上就写着我是用来判断是不是 usb device driver 的，那咱们就要问问什么 是 usb device driver？前面不是一直都是说一个 usb 接口对应一个 usb 驱动么？作为一 个吃社会主义粮长大的人，我可以负责任的告诉你前面说的一点错都没有，一个接口就是要 对应一个 usb 驱动，可是我们不能只钻到接口的那个口里边儿，我们应该眼光放的更加开 阔些，要知道接口在 usb 的世界里并不是老大，它上边儿还有配置，还有设备，都比它大。 每个接口对应了一个独立的功能，是需要专门的驱动来和它交流，但是接口毕竟整体是作为 一个 usb 设备而存在的，设备还可以有不同的配置，我们还可以为设备指定特定的配置， 那谁来做这个事情？接口驱动么？它还不够级别，它的级别只够和接口会谈会谈。这个和整 个 usb 设备进行对等交流的光荣任务就交给了 struct usb_device _driver，即 usb 设备 驱动，它和 usb 的接口驱动 struct usb_driver 都定义在 include/linux/usb.h 文件里

##### usb 接口驱动

蒲松龄曰，每个男人的心中都有一个狐狸精，每个写usb驱动的人心中都有一个 usb_driver。一般来说，我们平时所谓的编写usb驱动指的也就是写usb接口的驱动，需要 以一个struct usb_driver结构的对象为中心，以设备的接口提供的功能为基础，开展usb 驱动的建设。

```
833 struct usb_driver {
834 const char *name;
835驱动程序的名字，对应了在/sys/bus/usb/drivers/下面的子目录名称。
和我们每个人一样，它只是彼此区别的一个代号，不同的是我们可以有很多人叫张三或者李
四，但这里的名字在所有的 usb 驱动中必须是唯一的。
836 int (*probe) (struct usb_interface *intf,
837 const struct usb_device_id *id);
838用来看看这个 usb 驱动是否愿意接受某个接口的函数。每个驱动自诞生
起，它的另一半就已经确定了，这个函数就是来判断哪个才是她苦苦等待的那个他。当然，
这个他应该是他们，因为一个驱动往往可以支持多个接口。
839 void (*disconnect) (struct usb_interface *intf);
840当接口失去联系，或使用rmmod卸载驱动将它和接口强行分开时
这个函数就会被调用。
841 int (*ioctl) (struct usb_interface *intf, unsigned int code,
842 void *buf);当你的驱动有通过usbfs和用户空间交流的需要的话，就使用它吧。
843
844 int (*suspend) (struct usb_interface *intf, pm_message_t message);
845 int (*resume) (struct usb_interface *intf);
分别在设备被挂起和唤醒时使用
846 
 73
847 void (*pre_reset) (struct usb_interface *intf);
分别在设备将要复位（reset）和已经复位后
使用。
848 void (*post_reset) (struct usb_interface *intf);
849
850 const struct usb_device_id *id_table;
驱动支持的所有设备的花名册，所有的三宫六院要想受到宠幸都要在
这里登记。驱动就靠这张表儿来识别是不是支持哪个设备接口的，如果不属于这张表，那就
躲一边儿去吧。
851
852 struct usb_dynids dynids;//
853 struct usbdrv_wrap drvwrap;
854 unsigned int no_dynamic_id:1;//，no_dynamic_id，可以用来禁止动态 id 的，设置了之后，驱动就从一而终吧，
别七想八想了
855 unsigned int supports_autosuspend:1;//对 autosuspend 的支持，如果设置为 0 的话，就不
再允许绑定到这个驱动的接口 autosuspend。
856 }; 
```

852 行，dynids，支持动态 id 的。什么是动态 id？本来前面刚说每个驱动诞生时她的另一 半在 id_table 里就已经确定了，可是谁规定了女同胞就一定要从一而终了，那是封建旧思 想要打到的，听听她们内心的呼声“谁说我白，瘦，漂亮~我就跟他做好朋友”，Greg 大侠 显然也听到了，于是在一年多前的一个寒风萧萧的日子里平地一声吼，加入了动态 id 的机 74 制。即使驱动已经加载了，也可以添加新的 id 给她，只要新 id 代表的设备存在，对她说“你 又白又瘦又漂亮”，她就会和他绑定起来。

怎么添加新的 id？到驱动所在的地方瞅瞅，也就是/sys/bus/usb/drivers 目录下边儿，那 里列出的每个目录就代表了一个 usb 驱动，随便选一个进去，能够看到一个 new_id 文件 吧，使用 echo 将厂商和产品 id 写进去就可以了。看看 Greg 举的一个例子

```
echo 0557 2008 > /sys/bus/usb/drivers/foo_driver/new_id 
```

就可以将 16 进制值 0557 和 2008 写到 foo_driver 驱动的设备 id 表里取。

853 行 ， drvwrap ，这个字段有点意思， struct usbdrv_wrap 结构的，也在 include/linux/usb.h 里定义

```
774 /**
775 * struct usbdrv_wrap - wrapper for driver-model structure
776 * @driver: The driver-model core driver structure.
777 * @for_devices: Non-zero for device drivers, 0 for interface drivers.
778 */
779 struct usbdrv_wrap {
780 struct device_driver driver;
781 int for_devices;
782 }; 
```

###### usb设备驱动

近距离观察一下这个结构，它里面内容是比较的贫乏的，只有一个struct device_driver 结构的对象和一个for_devices的整型字段。回想一下linux的设备模型，我们的心头就会 产生这样的疑问，这里的struct device_driver对象不是应该嵌入到struct usb_driver结 构里么，怎么这里又包装了一层？再包装这么一层当然不是为了美观，写代码的哥们儿没这 么媚俗，这主要还是因为本来挺单纯的驱动在usb的世界里不得已分成了设备驱动和接口驱 动两种，为了区分这两种驱动，就中间加了这么一层，添了个for_devices标志来判断是哪 种。大家发现没，之前见识过的结构里，很多不是 1 就是 0 的标志使用的是位字段，特别 是几个这样的标志放一块儿的时候，而这里的for_devices虽然也只能有两个值，但却没有 使用位字段，为什么？简单的说就是这里没那个必要，那些使用位字段的是几个在一块儿， 可以节省点儿存储空间，而这里只有这么一个，就是使用位字段也节省不了，就不用多此一 举了，这个大家都知道哈，我有点多说了，还是言归整传。其实就这么说为了加个判断标志 就硬生生的塞这么一层，还是会有点模糊的，不过，其它字段不敢说，这个drvwrap咱们 以后肯定还会遇到它，这里先有个概念，混个面熟，等到再次相遇的那一刻，我保证你会明 白它的用心。



usb 设备驱动与接口驱 动到底都有多大的不同。

```
878 struct usb_device_driver {
879 const char *name;
880
881 int (*probe) (struct usb_device *udev);
882 void (*disconnect) (struct usb_device *udev);
883
884 int (*suspend) (struct usb_device *udev, pm_message_t message);
885 int (*resume) (struct usb_device *udev);
886 struct usbdrv_wrap drvwrap;
887 unsigned int supports_autosuspend:1;
888 };
```

就当一个ppmm在走近你，你先用放光的眼睛上上下下的扫一遍，这个usb设备驱动比前面 的接口驱动要苗条多了，除了少了很多东西外，剩下的将参数里的struct usb_interface 换成struct usb_device后就几乎一摸一样了。友情提醒一下，这里说的是几乎，而不是完 全，这是因为probe，它的参数里与接口驱动里的probe相比少了那个设备的花名册，也就 是说它不用再去根据花名册来判断是不是愿意接受一个usb设备。那么这意味着什么？是它 来者不拒，接受所有的usb设备？还是独锁深闺垂影自恋决绝所有人？当然只会是前者，不 然这个usb设备驱动就完全毫无疑义了，这里不像咱们的政府大院儿，容不得毫无作用的东 76 东存在。而且我们在内核里找来找去，也就只能找得着它在generic.c文件里定义了 usb_generic_driver这么一个对象

```
210 struct usb_device_driver usb_generic_driver = {
211 .name = "usb",
212 .probe = generic_probe,
213 .disconnect = generic_disconnect,
214 #ifdef CONFIG_PM
215 .suspend = generic_suspend,
216 .resume = generic_resume,
217 #endif
218 .supports_autosuspend = 1,
219 }; 
```

即使这么一个独苗儿，也早在 usb_init 的 895 行就已经注册给 usb 子系统了。那么我们 该用什么样的言语表达自己的感受？所谓的 usb 设备驱动完全就是一个博爱的主儿，我们 的 core 用它来与所有的 usb 设备进行交流，它们都交流些什么？现在人人都有一颗八卦的 心，不过这是后话，我会告诉你的。



### 接口

前面的前面已经说了，接口是设备的接口。设备可以有多个接口，每个接口代表一个功能， 每个接口对应着一个驱动。

**Linux 设备模型的 device 落实在 USB 子系统，成了两个结构， 一个是 struct usb_device，一个是 struct usb_interface**，一个石头砸了两个坑，一支 箭射下来两只麻雀，你说怪不怪。怪不怪还是听听复旦人甲怎么说，一个 usb 设备，两种 功能，一个键盘，上面带一个扬声器，两个接口，那这样肯定得要两个驱动程序，一个是键 盘驱动程序，一个是音频流驱动程序。道上的兄弟喜欢把这样两个整合在一起的东西叫做一 个设备，那好，让他们去叫吧，我们用 interface 来区分这两者行了吧。于是有了这里提到 的那个数据结构，struct usb_interface。

#### struct usb_interface

```
140 struct usb_interface {
141 /* array of alternate settings for this interface,
142 * stored in no particular order */
143 struct usb_host_interface *altsetting;//143 行，这里有个 altsetting 成员，只用耗费一个脑细胞就可以明白它的意思就是
alternate setting，可选的设置。
144
145 struct usb_host_interface *cur_altsetting; /* the currently
146 * active alternate setting *///那么再耗费一个脑细胞就可以知道 145 行的
cur_altsetting 表示当前正在使用的设置，
147 unsigned num_altsetting; /* number of alternate settings *///47 行的 num_altsetting 表示这个接口具有
可选设置的数量。
148
149 int minor; /* minor number this interface is
150 * bound to *///149 行，minor，分配给接口的次设备号。地球人都知道，linux 下所有的硬件设备都是用
文件来表示的，俗称设备文件，在/dev 目录下边儿，为了显示自己并不是普通的文件，它
们都会有一个主设备号和次设备号，比如
151 enum usb_interface_condition condition; /* state of binding *///151 行，condition 字段表示接口和驱动的
绑定状态
152 unsigned is_active:1; /* the interface is not suspended *///152 行，153 行与 157 行都是关于挂起和唤醒的。协议里规定，所有的 usb 设备都必须支
持挂起状态，就是说为了达到节电的目的，当设备在指定的时间内，3ms 吧，如果没有发
生总线传输，就要进入挂起状态。当它收到一个 non-idle 的信号时，就会被唤醒。
152表示接口是不是处于挂起状态。
153 unsigned needs_remote_wakeup:1; /* driver requires remote wakeup */表示是否需要
打开远程唤醒功能。远程唤醒允许挂起的设备给主机发信号，通知主机它将从挂起状态恢复，
注意如果此时主机处于挂起状态，就会唤醒主机，不然主机仍然在睡着，设备自个醒过来干
吗用。协议里并没有要求 USB 设备一定要实现远程唤醒的功能，即使实现了，从主机这边
儿也可以打开或关闭它。
154
155 struct device dev; /* interface specific device info */
156 struct device *usb_dev; /* pointer to the usb class's device,
if any */
接下来就剩下 155 行的struct device dev和 156 行的struct device *usb_dev，看到
struct device没，它们就是linux设备模型里的device嵌在这儿的对象，我们的心中要时时
有个模型。不过这么想当然是不正确的，两个里面只有dev才是模型里的device嵌在这儿的，
usb_dev则不是。当接口使用 USB_MAJOR作为主设备号时，usb_dev才会用到，你找遍
整个内核，也只在usb_register_dev和usb_deregister_dev两个函数里能够看到它，
usb_dev指向的就是usb_register_dev函数里创建的usb class device。
157 int pm_usage_cnt; /* usage counter for autosuspend */
//pm 就是电源管理，usage_cnt 就是使
用计数，当它为 0 时，接口允许 autosuspend。什么叫 autosuspend？用过笔记本吧，
曾经拥有过一台 DELL，现在正在拥有另一台 DELL，好无奈啊，为什么不是小黑或小白？
有时合上笔记本后，它会自动进入休眠，这就叫 autosuspend。但不是每次都是这样的，
就像这里只有当 pm_usage_cnt 为 0 时才会允许接口 autosuspend。至于这个计数在哪
里统计，暂时还是飘过吧。
158 }; 
```

#### 配置与设置

咱们是难得糊涂几千年了，不会去区分配置还有设置有什么区别，起码我平时即使是再无聊 也不会去想这个，但老外不一样，他们不知道老子也不知道郑板桥，所以说他们挺较真儿这 个，还分了两个词，配置是 configuration，设置是 setting。先说配置，一个手机可以有 40 多种配置，比如可以摄像，可以接在电脑里当做一个 U 盘，那么这两种情况就属于不同的 配置，在手机里面有相应的选择菜单，你选择了哪种它就按哪种配置进行工作，供你选择的 这个就叫做配置。很显然,当你摄像的时候你不可以访问这块 U 盘，当你访问这块 U 盘的时 候你不可以摄像，因为你做了选择。第二，既然一个配置代表一种不同的功能，那么很显然， 不同的配置可能需要的接口就不一样，我假设你的手机里从硬件上来说一共有 5 个接口， 那么可能当你配置成 U 盘的时候它只需要用到某一个接口，当你配置成摄像的时候，它可 能只需要用到另外两个接口，可能你还有别的配置，然后你可能就会用到剩下那两个接口。 再说说设置，一个手机可能各种配置都确定了，是振动还是铃声已经确定了，各种功能都确 定了，但是声音的大小还可以变吧，通常手机的音量是一格一格的变动，大概也就 5、6 格， 那么这个可以算一个 setting 吧。 不过你还是不明白啥是配置啥是设置的话，就直接用大小关系来理解好了，毕竟大家对互相 之间的大小关系都更敏感一些，不要说不是，一群 mm 走过来的时候，你的眼神已经背叛 了你。这么说吧，**设备大于配置，配置大于接口，接口大于设置**，更准确的说是设备可以有 多个配置，配置里可以包含一个或更多的接口，而接口通常又具有一个或更多的设置。

#### 设备号

![image-20210404194942353](/home/zhaoyong/.config/Typora/typora-user-images/image-20210404194942353.png)	

主 设备号表明了设备的种类，也表明了设备对应着哪个驱动程序，而次设备号则是因为一个驱 动程序要支持多个设备而为了让驱动程序区分它们而设置的。

主设备号用来帮你 找到对应的驱动程序，次设备号给你的驱动用来决定对哪个设备进行操作。

设备要想在 linux 里分得一个主设备号，有个立足之地，也并不是那么容易的，主设备号虽 说不是什么特别稀缺的资源，但还是需要设备先在驱动里提出申请，获得系统的批准才能拥 有一个。因为一部分的主设备号已经被静态的预先指定给了许多常见的设备，你申请的时候 要避开它们，选择一个里面没有列出来的，也就是名花还没有主的，很严肃的说，挖墙角是 很不道德的。这些已经被分配掉的主设备号都列在 Documentation/devices.txt 文件里。 当然，如果你是用动态分配的形式，就可以不去理会这些，直接让系统为你作主，替你选择 一个即可。

很显然，任何一个有理智有感情的人都会认为 USB 设备是很常见的，linux 理应为它预留 了一个主设备号。看看 include/linux/usb.h 文件

```
7 #define USB_MAJOR 180
8 #define USB_DEVICE_MAJOR 189 
```

苏格拉底说过，学的越多，知道的越多，知道的越多，发现需要知道更多。当我们知道了主 设备号，满怀激情与向往的来寻找 USB 的主设备号时，我们却发现这里在上演真假李逵。 这两个哪个才是我们苦苦追寻的她？ 你可以在内核里搜索它们都曾经出现什么地方，或者就跟随我回到 usb_init 函数。

```
880 retval = usb_major_init();
881 if (retval)
882 goto major_init_failed;
883 retval = usb_register(&usbfs_driver);
884 if (retval)
885 goto driver_register_failed;
886 retval = usb_devio_init();
887 if (retval)
888 goto usb_devio_init_failed;
889 retval = usbfs_init();
890 if (retval)
891 goto fs_init_failed;
```

前面只提了句 883~891 是与usbfs相关的就简单的飘过了，这里略微说的多一点。usbfs 为咱们提供了在用户空间直接访问usb硬件设备的接口，但是世界上没有免费的午餐，它需 要内核的大力支持， usbfs_driver 就是用来完成这个光荣任务的。咱们可以去 usb_devio_init函数里看一看，它在devio.c文件里定义

```
 retval = register_chrdev_region(USB_DEVICE_DEV, USB_DEVICE_MAX,
 "usb_device");
 if (retval) {
 err("unable to register minors for usb_device");
 goto out;
 } 
```

register_chrdev_region 函数获得了设备 usb_device 对应的设备编号，设备 usb_device 对应的驱动当然就是 usbfs_driver，参数 USB_DEVICE_DEV 也在同一个 文件里有定义 #define USB_DEVICE_DEV MKDEV(USB_DEVICE_MAJOR, 0) 终于再次见到了 USB_DEVICE_MAJOR，也终于明白它是为了 usbfs 而生，为了让广大 人民群众能够在用户空间直接和 usb 设备通信而生。因此，它并不是我们所要寻找的。

那么答案很明显了，USB_MAJOR就是咱们苦苦追寻的那个她，就是linux为USB设备预留 的主设备号。事实上，前面usb_init函数的 880 行，usb_major_init函数已经使用 USB_MAJOR注册了一个字符设备，名字就叫usb。我们可以在文件/proc/devices里看到 它们。

![image-20210404200141308](/home/zhaoyong/.config/Typora/typora-user-images/image-20210404200141308.png)	

/proc/devices 文件里显示了所有当前系统里已经分配出去的主设备号，当然上面只是列 出了字符设备，Block devices 被有意的飘过了。很明显，咱们前面提到的 usb_device 和 usb 都在里面。

不过到这里还没完，USB设备有很多种，并不是都会用到这个预留的主设备号。比如俺的 移动硬盘显示出来的主设备号就是 8，你的摄像头在linux显示的主设备号也绝对不会是这 里的 USB_MAJOR。坦白的说，咱们经常遇到的大多数usb设备都会与input、video等子 系统关联，并不单单只是作为usb设备而存在。如果usb设备没有与其它任何子系统关联， 就需要在对应驱动的probe函数里使用usb_register_dev函数来注册并获得主设备号 USB_MAJOR ，你可以在 drivers/usb/misc 目录下看到一些例子， drivers/usb/ usb-skeleton.c文件也属于这种。如果usb设备关联了其它子系统，则需要在对应驱动的 probe函数里使用相应的注册函数，USB_MAJOR也就该干吗干吗去，用不着它了。比如， usb键盘关联了input子系统，驱动对应drivers/hid/usbhid目录下的usbkbd.c文件，在它 的probe函数里可以看到使用了input_register_device来注册一个输入设备。 43 准确的说，这里的USB设备应该说成USB接口，当USB接口关联有其它子系统，也就是说 不使用 USB_MAJOR作为主设备号时，struct usb_interface的字段minor可以简单的忽 略。**minor只在 USB_MAJOR起作用时起作用。**

#### 接口和驱动的 绑定状态

```
83 enum usb_interface_condition {
84 USB_INTERFACE_UNBOUND = 0,
85 USB_INTERFACE_BINDING,
86 USB_INTERFACE_BOUND,
87 USB_INTERFACE_UNBINDING,
88 }; 
孤苦、期待、幸福、分开
```



<<<<<<< HEAD
##### 接口设置
=======
#### 设置
>>>>>>> 904f55db4800469b8842ddfad4a225adc6224139

不过这里还是有点小悬念的，前面 struct usb_interface 里表示接口设置的 struct usb_host_interface 就被有意无意的飘过了，咱们在这里看看它的真面目，同样在 include/linux/usb.h 文件里定义。

```
69 /* host-side wrapper for one interface setting's parsed descriptors */
70 struct usb_host_interface {
<<<<<<< HEAD
71 struct usb_interface_descriptor desc;
=======
71 struct usb_interface_descriptor desc;//71 行，desc，接口的描述符。
>>>>>>> 904f55db4800469b8842ddfad4a225adc6224139
72
73 /* array of desc.bNumEndpoint endpoints associated with this
74 * interface setting. these will be in no particular order.
75 */
<<<<<<< HEAD
76 struct usb_host_endpoint *endpoint;//一个数组，表 示这个设置所使用到端点。
77
78 char *string; /* iInterface string, if present */，string，用来保存从设备里取出来的字符串描述符信息的，既然字符串描述符可有
可无，那这里的指针也有可能为空了。
79 unsigned char *extra; /* Extra descriptors */
80 int extralen; 行 extralen，有关额外的描述符的。除了前面提到的四大描述符还有字
符串描述符外，还有为一组设备也就是一类设备定义的描述符，和厂商为设备特别定义的描
述符，extra 指的就是它们，extralen 表示它们的长度。比如上海规定了，社保必须得交
多少多少，公积金多少多少，有个最低的比例，有地儿觉得太少，给你多交些，叫补充什么
金的，还有些地儿，觉得补充都不过瘾，像公务员这种特殊行业特别劳心劳力的，再加些特
殊行业补贴等什么的，既规定了必须实现的，也给你特殊行业发挥的空间，当然怎么发挥就
不是咱说了算了，不操那份儿心了。
81 }; 
```

###### 接口描述符
=======
76 struct usb_host_endpoint *endpoint;
77
78 char *string; /* iInterface string, if present */
79 unsigned char *extra; /* Extra descriptors */
80 int extralen;
81 }; 
```

#### 描述符

什么叫描述符？我们的生活就是一个不断的遇到人认识人的 过程，有些人注定只是擦肩而过，有些人却深深的留在我们的内心里，比如 USB 的描述符。 实际上，usb 的描述符是一个带有预定义格式的数据结构，里面保存了 usb 设备的各种属 性还有相关信息，姓甚名谁啊，哪儿生产的啊等等，我们可以通过向设备请求获得它们的内 容来深刻的了解感知一个 usb 设备。主要有四种 usb 描述符，设备描述符，配置描述符， 接口描述符和端点描述符，协议里规定一个 usb 设备是必须支持这四大描述符的，当然也 有其它一些描述符来让设备可以显得个性些，但这四大描述符是一个都不能少的。

##### EEPROM

这些描述符放哪儿？当然是在设备里。就好像你要把身份证放自己身上以免在哪里心情舒畅 的散步时被新时代最可爱的人警察叔叔查到一样，你不会直接放他们那儿吧，然后在他们亲 切慈祥的向你招手时，告诉他们说不就在你那儿么，那样的话等待你又是个什么样的结果， 我不知道，我想你也不会想知道。咱们的描述符就在设备里，等着主机去拿。具体在哪儿？ usb 设备里都会有一个叫 EEPROM 的东东，没错，就是放在它那儿，它就是用来存储设备 本身信息的。如果你的脑海里还残存着一些大学里的美好时光的话，应该还会记得 EEPROM 45 就是电可擦写的可编程 ROM，它与 Flash 虽说都是要电擦除的，但它可以按字节擦除，Flash 只能一次擦除一个 block，所以如果要改动比较少的数据的话，使用它还是比较合适的，但 是世界上没有完美的东西，此物成本相对 Flash 比较高，所以一般来说 usb 设备里只拿它 来存储一些本身特有的信息，要想存储数据，还是用 Flash 吧。

##### 接口描述符

就是电可擦写的可编程 ROM，它与 Flash 虽说都是要电擦除的，但它可以按字节擦除，Flash 只能一次擦除一个 block，所以如果要改动比较少的数据的话，使用它还是比较合适的，但 是世界上没有完美的东西，此物成本相对 Flash 比较高，所以一般来说 usb 设备里只拿它 来存储一些本身特有的信息，要想存储数据，还是用 Flash 吧。 具体到接口描述符，它当然就是描述接口本身的信息的。一个接口可以有多个设置，使用不 同的设置，描述接口的信息会有些不同，所以接口描述符并没有放在 struct usb_interface 结构里，而是放在表示接口设置的 struct usb_host_interface 结构里。定义在 include/linux/usb/ch9.h 文件里
>>>>>>> 904f55db4800469b8842ddfad4a225adc6224139

```
294 /* USB_DT_INTERFACE: Interface descriptor */
295 struct usb_interface_descriptor {
<<<<<<< HEAD
296 __u8 bLength;
297 __u8 bDescriptorType;
=======
296 __u8 bLength;//296 行，bLength，描述符的字节长度。协议里规定，每个描述符必须以一个字节打头来
表明描述符的长度。那可以扳着指头数一下，接口描述符的 bLength 应该是 9，两个巴掌
就数完了，没错，ch9.h 文件里紧挨着接口描述符的定义就定义了这个长度
308 #define USB_DT_INTERFACE_SIZE 
297 __u8 bDescriptorType;//297 行，bDescriptorType，描述符的类型。各种描述符的类型都在ch9.h文件里有定义，
对应spec 中的Table 9.5。对于接口描述符来说，值为 USB_DT_INTERFACE，也就是
0x04。
>>>>>>> 904f55db4800469b8842ddfad4a225adc6224139
298
299 __u8 bInterfaceNumber;
300 __u8 bAlternateSetting;
301 __u8 bNumEndpoints;
302 __u8 bInterfaceClass;
303 __u8 bInterfaceSubClass;
304 __u8 bInterfaceProtocol;
305 __u8 iInterface;
306 } __attribute__ ((packed)); 
```

<<<<<<< HEAD
又看到了 __attribute__，不过这里改头换面成了 __attribute__ ((packed))，意思就是
告诉编译器，这个结构的元素都是 1 字节对齐的，不要再添加填充位了。因为这个结构和
spec里的Table 9.12 是完全一致的，包括字段的长度，如果不给编译器这么个暗示，编译
器就会依据你平台的类型在结构的每个元素之间添加一定的填充位，如果你拿这个添加了填
充位的结构去向设备请求描述符，你想想会是什么结果。
296 行，bLength，描述符的字节长度。协议里规定，每个描述符必须以一个字节打头来
表明描述符的长度。那可以扳着指头数一下，接口描述符的 bLength 应该是 9，两个巴掌
就数完了，没错，ch9.h 文件里紧挨着接口描述符的定义就定义了这个长度
308 #define USB_DT_INTERFACE_SIZE 9
297 行，bDescriptorType，描述符的类型。各种描述符的类型都在ch9.h文件里有定义，
对应spec 中的Table 9.5。对于接口描述符来说，值为 USB_DT_INTERFACE，也就是
0x04。

###### 接口描述符类型

![image-20210406123159879](/home/zhaoyong/.config/Typora/typora-user-images/image-20210406123159879.png)	

299 行，bInterfaceNumber，接口号。每个配置可以包含多个接口，这个值就是它们的 索引值。 300 行，bAlternateSetting，接口使用的是哪个可选设置。协议里规定，接口默认使用的 设置总为 0 号设置。 

301 行，bNumEndpoints，接口拥有的端点数量。这里并不包括端点 0，端点 0 是所有 的设备都必须提供的，所以这里就没必要多此一举的包括它了。 

302 行，bInterfaceClass，303 行 bInterfaceSubClass，304 行 bInterfaceProtocol。 这个世界上有许许多多的 usb 设备，它们各有各的特点，为了区分它们，usb 规范，或者 说 usb 协议，把 usb 设备分成了很多类，然而每个类又分成子类，这很好理解，我们一个 大学也是如此，先是分成很多个学院，然后每个学院又被分为很多个系，然后可能每个系下 边又分了各个专业，usb协议也是这样干的，首先每个Device或Interface属于一个Class， 然后 Class 下面又分了 SubClass，完了 SubClass 下面又按各种设备所遵循的不同的通信 协议继续细分。usb 协议里边为每一种 Class，每一种 SubClass，每一种 Protocol 定义 一个数值，比如 mass storage 的 Class 就是 0x08，hub 的 Class 就是 0x09。

305 行，iInterface，接口对应的字符串描述符的索引值。疑？这里怎么又跳出来一个叫字 符串描述符的东东？你没看错我也没说错，除了前面提到的四大描述符，是还有字符串描述 符，不过那四大描述符是每个设备必须支持的，这个字符串描述符却是可有可无的，有了你 欢喜我也欢喜，没有也不是什么问题。使用 lsusb 命令看一下

###### 字符串描述符

![image-20210406123602543](/home/zhaoyong/.config/Typora/typora-user-images/image-20210406123602543.png)

第一行里显示的是我手边儿的 Cypress USB 开发板，看里面的 Cypress Semiconductor Corp.，这么一长串的东东从哪里来？是不是应该从设备里来？设备的那几个标准描述符， 整个描述符的大小也不一定放得下这么一长串，所以，一些设备专门准备了一些字符串描述 符（string descriptor），就用来记这些长串的东西。字符串描述符主要就是提供一些设备 接口相关的描述性信息，比如厂商的名字，产品序列号等等。字符串描述符当然可以有多个， 这里的索引值就是用来区分它们的。

说过了接口描述符，回到struct usb_host_interface的 76 行，endpoint，一个数组，表 示这个设置所使用到端点。至于端点的结构struct usb_host_endpoint，天这么热，让它 先一边儿凉快凉快吧，不怕春光乍泄的话，可以去思源湖冲个凉，咱们先看完struct usb_host_interface再去说它。

###### 端点

折腾 USB spec 的同志应该不会读过庄子，也不会知道于丹这个人物，可别人也知道端点， 于是端点成了 USB 数据传输的终点。看看它在内核里的定义

```
59 struct usb_host_endpoint {
60 struct usb_endpoint_descriptor desc;
61 struct list_head urb_list;
62 void *hcpriv;
63 struct ep_device *ep_dev; /* For sysfs info */
64
65 unsigned char *extra; /* Extra descriptors */
66 int extralen;
67 }; 
```

回到struct usb_host_endpoint，61 行，urb_list，端点要处理的urb队列。urb是什么？ 这年头儿钱不多就是新名词儿多，是个新名词爆炸的时代，不过urb可是usb通信的主角， 它包含了执行urb传输所需要的所有信息，你要想和你的usb通信，就得创建一个urb，并 且为它赋好值，交给咱们的usb core，它会找到合适的host controller，从而进行具体的 数据传输。设备中的每个端点都可以处理一个urb队列，当然，urb是内核里对usb传输数 据的封装也叫抽象吧，协议里可不这么叫。基于urb特殊的江湖地位，接下来的哪一个黄道 吉日里，我会对它大书特书的。

62 行，hcpriv，这是提供给 HCD（host controller driver）用的。比如等时端点会在里 边儿放一个 ehci_iso_stream，什么意思？郑板桥告诉我们要难得糊涂

63 行，ep_dev，这个字段是供 sysfs 用的。好奇的话可以去/sys 下看一看

![image-20210406125308250](/home/zhaoyong/.config/Typora/typora-user-images/image-20210406125308250.png)

ep_00 端点目录下的这些文件从哪儿来的？就是在 usb_create_ep_files 函数里使用 ep_dev 创建的

65 行 ， extra ， 66 行 ， extralen ，有关一些额外扩展的描述符的，和 struct usb_host_interface里差不多，只是这里的是针对端点的，如果你请求从设备里获得描述 符信息，它们会跟在标准的端点描述符后面返回给你。



###### 端点描述符

```
312 /* USB_DT_ENDPOINT: Endpoint descriptor */
313 struct usb_endpoint_descriptor {
314 __u8 bLength;
315 __u8 bDescriptorType;
316
317 __u8 bEndpointAddress;
318 __u8 bmAttributes;
319 __le16 wMaxPacketSize;
320 __u8 bInterval;
321
322 /* NOTE: these two are _only_ in audio endpoints. */
323 /* use USB_DT_ENDPOINT*_SIZE in bLength, not sizeof. */
324 __u8 bRefresh;
325 __u8 bSynchAddress;
326 } __attribute__ ((packed));
327
328 #define USB_DT_ENDPOINT_SIZE 7
329 #define USB_DT_ENDPOINT_AUDIO_SIZE 9 /* Audio extension */ 
```

这个结构与 spec 中的 Table 9.13 是一一对应的，0 号端点仍然保持着它特殊的地位，它 没有自己的端点描述符。

314 行，bLength，描述符的字节长度，数一下，前边儿有 7 个，后边儿又多了两个字节， 那是针对音频设备扩展的，不用管它，紧接着 struct usb_host_endpoint 定义的就是两 个长度值的定义。

315 行，bDescriptorType，描述符类型，这里对于端点就是 USB_DT_ENDPOINT，0x05。

317 行，bEndpointAddress，这个字段描述的信息挺多的，比如这个端点是输入端点还 是输出端点，这个端点的地址，以及这个端点的端点号。它的bits 0~3 表示的就是端点号， 你使用 0x0f和它相与就可以得到端点号。不过，开发内核的同志想的都很周到，定义好了 一个掩码USB_ENDPOINT_NUMBER_MASK，它的值就是 0x0f，当然，这是为了让咱们 更容易去读他们的代码，也为了以后的扩展。另外，它的bit 8 表示方向，输入还是输出， 同样有掩码 USB_ENDPOINT_DIR_MASK，值为 0x80，将它和bEndpointAddress相 与，并结合USB_DIR_IN和USB_DIR_OUT作判断就可以得到端点的方向。

319 行，wMaxPacketSize，端点一次可以处理的最大字节数。比如你老板比较看重你， 一次给你交代了几个任务，于是你大声的疾呼，神啊，我一次只能做一个，当然神是听不到 的，怎么办那，加班加点儿，一个一个的分开做呗。端点也是，如果你发送的数据量大于端 点的这个值，也会分成多次一次一次来传输。友情提醒一下，这个字段还是有点门道的，对 不同的传输类型也有不同的要求，日后碰到了再说

 320 行，bInterval， USB 是轮询式的总线，这个值表达了端点一种美好的期待，希望主 机轮询自己的时间间隔，但实际上批准不批准就是 host 的事了。不同的传输类型 bInterval 也有不同的意义，暂时就提这么一下，碰到各个实际的传输类型了再去说它。不是俺捂盘惜 售，而是初次照面儿就对人家寻根问底的不大礼貌，这里先留个印象，有缘总会再相见的。

```
42 /*
43 * USB directions
44 *
45 * This bit flag is used in endpoint descriptors' bEndpointAddress field.
46 * It's also one of three fields in control requests bRequestType.
47 */ 
 50
48 #define USB_DIR_OUT 0 /* to device */
49 #define USB_DIR_IN 0x80 /* to host */ 
```

318 行，bmAttributes，属性，总共 8 位，其中bit1 和bit0 共同称为Transfer Type,即 传输类型， 00 表示控制，01 表示等时，10 表示批量，11 表示中断。前面的端点号还 有端点方向都有配对儿的掩码，这里当然也有，就在struct usb_endpoint_descriptor定 义的下面

```
338 #define USB_ENDPOINT_XFERTYPE_MASK 0x03 /* in bmAttributes */
339 #define USB_ENDPOINT_XFER_CONTROL 0
340 #define USB_ENDPOINT_XFER_ISOC 1
341 #define USB_ENDPOINT_XFER_BULK 2
342 #define USB_ENDPOINT_XFER_INT 3 
```



### 设备

#### struct usb_device

```
328 /*
329 * struct usb_device - kernel's representation of a USB device
330 *
331 * FIXME: Write the kerneldoc!
332 *
333 * Usbcore drivers should not set usbdev->state directly. Instead use
334 * usb_set_device_state().
335 */
336 struct usb_device {
337 int devnum; /* Address on USB bus */
338 char devpath [16]; /* Use in messages: /port/port/... */
339 enum usb_device_state state; /* configured, not attached, etc */
340 enum usb_device_speed speed; /* high/full/low (or error) */
341
342 struct usb_tt *tt; /* low/full speed dev, highspeed hub
*/
343 int ttport; /* device port on that tt hub */ 
 52
344
345 unsigned int toggle[2]; /* one bit for each endpoint
346 * ([0] = IN, [1] = OUT) */
347
348 struct usb_device *parent; /* our hub, unless we're the root */
349 struct usb_bus *bus; /* Bus we're part of */
350 struct usb_host_endpoint ep0;
351
352 struct device dev; /* Generic device interface */
353
354 struct usb_device_descriptor descriptor;/* Descriptor */
355 struct usb_host_config *config; /* All of the configs */
356
357 struct usb_host_config *actconfig;/* the active configuration */
这就是设备描述符，它和spec Table 9-8 是一一对应的。咱们回到struct usb_device的
355 行，config，357 行，actconfig，分别表示设备拥有的所有配置和当前激活的，也就
是正在使用的配置。usb设备的配置用struct usb_host_config结构来表示，下节再说。

358 struct usb_host_endpoint *ep_in[16];
358 行，ep_in[16]，359 行，ep_out[16]，除了端点 0，一个设备即使在高速模式下也
最多只能再有 15 个 IN 端点和 15 个 OUT 端点，端点 0 太特殊了，对应的管道是 Message
管道，又能进又能出特能屈能伸的那种，所以这里的 ep_in 和 ep_out 数组都有 16 个值。

359 struct usb_host_endpoint *ep_out[16];
360
361 char **rawdescriptors; /* Raw descriptors for each config */
362
361 行，rawdescriptors，这是个字符指针数组，数组里的每一项都指向一个使用
GET_DESCRIPTOR 请求去获得配置描述符时所得到的结果。考虑下，为什么我只说得到
的结果，而不直接说得到的配置描述符？不是请求的就是配置描述符么？这是因为你使用
GET_DESCRIPTOR 去请求配置描述符时，设备返回给你的不仅仅只有配置描述符，它把
该配置所包括的所有接口的接口描述符，还有接口里端点的端点描述符一股脑的都塞给你
了。第一个接口的接口描述符紧跟着这个配置描述符，然后是这个接口下面端点的端点描述
符，如果有还有其它接口，它们的接口描述符和端点描述符也跟在后面，这里面，专门为一
类设备定义的描述符和厂商定义的描述符跟在它们对应的标准描述符后面。这和我们去买水
果，买了 5 斤苹果却只有 5 个真实天壤之别，现实生活中的愤懑在 USB 世界里得到了发泄

363 unsigned short bus_mA; /* Current available from the bus */
bus_mA，这个值是在 host controller 的驱动程序中设置的，通常来讲，计算机
的 usb 端口可以提供 500mA 的电流。
364 u8 portnum; /* Parent port number (origin 1) */
不管是 root hub 还是一般的 hub，你的 USB 设备总归要插在一个
hub 的端口上才能用，portnum 就是那个端口号。当然，对于 root hub 这个 usb 设备来
说它本身没有 portnum 这么一个概念，因为它不插在别的 Hub 的任何一个口上。所以对
于 Root Hub 来说，它的 portnum 在 Host Controller 的驱动程序里给设置成了 0。
365 u8 level; /* Number of USB hub ancestors */
层次，也可以说是级别，表征 usb 设备树的级连关系。Root Hub 的 level
当然就是 0，其下面一层就是 level 1，再下面一层就是 level 2，依此类推。
366
367 unsigned discon_suspended:1; /* Disconnected while suspended */
368 unsigned have_langid:1; /* whether string_langid is valid */
usb 设备里的字符串描述符使用的是
UNICODE 编码，可以支持多种语言，string_langid 就是用来指定使用哪种语言的，
have_langid 用来判断 string_langid 是否有效。
369 int string_langid; /* language ID for strings */
370
371 /* static strings from the device */
372 char *product; /* iProduct string, if present */
373 char *manufacturer; /* iManufacturer string, if present */
374 char *serial; /* iSerialNumber string, if present */
375 分别用来保存产品、厂商
和序列号对应的字符串描述符信息。
376 struct list_head filelist;
377 #ifdef CONFIG_USB_DEVICE_CLASS
378 struct device *usb_classdev;
379 #endif
380 #ifdef CONFIG_USB_DEVICEFS
381 struct dentry *usbfs_dentry; /* usbfs dentry entry for the device
*/
382 #endif
usbfs 相关的，不可知的未来说 usbfs 的时候再聊它们。
383 /*
384 * Child devices - these can be either new devices
385 * (if this is a hub device), or different instances
386 * of this same device. 
 53
387 *
388 * Each instance needs its own set of data structures.
389 */
390
391 int maxchild; /* Number of ports if hub */hub 的端口数，注意可不包括上行端口。
392 struct usb_device *children[USB_MAXCHILDREN];
其实 hub 可以接一共 255 个端口，不过实际上遇到的 usb hub 最多的也就是说自己支持
10 个端口的，所以 31 基本上够用了。
393
394 int pm_usage_cnt; /* usage counter for autosuspend */
395 u32 quirks; /* quirks of the whole device */
396quirks，祭起我们法宝金山词霸看看，怪僻的意思，白了说就是大家的常用语“毛
病”。本来指定 usb spec 就是让大家团结一致好办事，但总是有些厂商不太守规矩，拿出
一些有点毛病的产品给我们用，你说它大毛病吧，也不是，就像俺这儿的厦 X 彩电一样，
绝对能看，只是动不动就罢次工。不说远了，总之这里的 quirk 就是用来判断这些有毛病的
产品啥毛病的。谁去判断？不像咱们的中国足协，把中国足球折腾成这样子，也就是出来声
明一下完事儿，咱们 usb 这儿实行的可是责任制，你的设备接哪儿哪儿负责，也就是说 hub
去判断，就不用咱费心了。
397 #ifdef CONFIG_PM
398 struct delayed_work autosuspend; /* for delayed autosuspends */
399 struct mutex pm_mutex; /* protects PM operations */
400
401 unsigned long last_busy; /* time of last use */
402 int autosuspend_delay; /* in jiffies */
403
404 unsigned auto_pm:1; /* autosuspend/resume in progress */
405 unsigned do_remote_wakeup:1; /* remote wakeup should be enabled */
406 unsigned autosuspend_disabled:1; /* autosuspend and autoresume */
407 unsigned autoresume_disabled:1; /* disabled by the user */
408 #endif
409 }; 
```



##### struct usb_bus

```
268 /* USB device number allocation bitmap */
269 struct usb_devmap {
270 unsigned long devicemap[128 / (8*sizeof(unsigned long))];
271 }; 
```

什么是usb_bus？前面不是已经有了一个struct bus_type类型的 usb_bus_type了么？ 没错，在usb子系统的初始化函数usb_init里已经注册了 usb_bus_type，不过那是让系统 54 知道有这么一个类型的总线。而一个总线类型和一条总线是两码子事儿。从硬件上来讲，一 个host controller就会连出一条usb总线，而从软件上来讲，不管你有多少个host controller，或者说有多少条总线，它们通通属于usb_bus_type这么一个类型，只是每一 条总线对应一个struct usb_bus结构体变量，这个变量在host controller的驱动程序中去 申请。 上面的 devmap 地址映射表就表示了一条总线上设备连接的情况，假设 unsigned long=4bytes，那么 unsigned long devicemap[128/(8*sizeof(unsigned long)]]就 等价于 unsigned long devicemap[128/(8*4)] ，进而等价于 unsigned long devicemap[4]，而 4bytes 就是 32 个 bits，因此这个数组最终表示的就是 128 个 bits。 而这也对应于一条总线可以连接 128 个 usb 设备。之所以这里使用 sizeof(unsigned long)，就是为了跨平台应用，不管 unsigned long 到底是几，总之这个 devicemap 数组 最终可以表示 128 位，也就是说每条总线上最多可以连上 128 个设备。

338 行，devpath [16]，它显然是用来记录一个字符串的，这个字符串啥意思？给你看个 直观的东西

![image-20210406130041015](/home/zhaoyong/.config/Typora/typora-user-images/image-20210406130041015.png)

Sysfs 文件系统下，我们看到这些乱七八糟的东西，它们都是啥？usb1/usb2/usb3/usb4 表示哥们的计算机上接了 4 条 usb 总线，即 4 个 usb 主机控制器，事物多了自然就要编号， 就跟我们中学或大学里面的学号一样，就是用于区分多个个体，而 4-0:1.0 表示什么？4 表示是 4 号总线，或者说 4 号 Root Hub，0 就是这里我们说的 devpath，1 表示配置为 1 号，0 表示接口号为 0。也即是说，4 号总线的 0 号端口的设备，使用的是 1 号配置，接口 号为0。那么devpath是否就是端口号呢？显然不是，这里我列出来的这个例子是只有Root Hub 没有级联 Hub 的情况，如果在 Root Hub 上又接了别的 Hub，然后一级一级连下去， 子又生孙，孙又生子，子又有子，子又有孙。子子孙孙，无穷匮也。那么如何在 sysfs 里面 来表征这整个大家族呢？这就是devpath的作用，顶级的设备其devpath就是其连在Root Hub 上的端口号，而次级的设备就是其父 Hub 的 devpath 后面加上其端口号，即如果 4-0:1.0 如果是一个 Hub，那么它下面的 1 号端口的设备就可以是 4-0.1:1.0，2 号端口 的设备就可以是 4-0.2:1.0，3 号端口就可以是 4-0.3:1.0。总的来说，就是端口号一级一 级往下加。这个思想是很简单的，也是很朴实的。

339 行，state，设备的状态。这是个枚举类型

```
557 enum usb_device_state {
558 /* NOTATTACHED isn't in the USB spec, and this state acts
559 * the same as ATTACHED ... but it's clearer this way.
560 */
561 USB_STATE_NOTATTACHED = 0,
562 
 55
563 /* chapter 9 and authentication (wireless) device states */
564 USB_STATE_ATTACHED,
565 USB_STATE_POWERED, /* wired */
566 USB_STATE_UNAUTHENTICATED, /* auth */
567 USB_STATE_RECONNECTING, /* auth */
568 USB_STATE_DEFAULT, /* limited function */
569 USB_STATE_ADDRESS,
570 USB_STATE_CONFIGURED, /* most functions */
571
572 USB_STATE_SUSPENDED
573
574 /* NOTE: there are actually four different SUSPENDED
575 * states, returning to POWERED, DEFAULT, ADDRESS, or
576 * CONFIGURED respectively when SOF tokens flow again.
577 */
578 }; 
```

##### 设备状态

上面定义了 9 种状态，spec 里只定义了 6 种，Attached，Powered，Default，Address， Configured，Suspended，对应于 Table 9.1。 Attached 表示设备已经连接到 usb 接口上了，是 hub 检测到设备时的初始状态。那么这 里所谓的 USB_STATE_NOTATTACHED 就是表示设备并没有 Attached。 Powered 是加电状态。USB 设备的电源可以来自外部电源，协议里叫做 self-powered， 也可以来自 hub，叫 bus-powered。尽管 self-powered 的 USB 设备可能在连接上 USB 接口以前已经上电，但它们直到连上 USB 接口后才能被看作是 Powered 的，你觉得它已 经上电了那是站在你的角度看，可是现在你看的是 usbcore，所以要放弃个人的成见，团 结在 core 的周围。 Default 缺省状态，在 Powered 之后，设备必须在收到一个复位（reset）信号并成功复 位后，才能使用缺省地址回应主机发过来的设备和配置描述符的请求。 Address 状态表示主机分配了一个唯一的地址给设备，此时设备可以使用缺省管道响应主 机的请求。真羡慕这些 usb 设备，住的地方都是包分配的，哪像咱们辛辛苦苦一路小跑着 也不一定能达到 Address 状态。 Configured 状态表示设备已经被主机配置过了，也就是协议里说的处理了一个带有非 0 值 的 SetConfiguration()请求，此时主机可以使用设备提供的所有功能。 Suspended 挂起状态，为了省电，设备在指定的时间内，3ms 吧，如果没有发生总线传 输，就要进入挂起状态。此时，usb 设备要自己维护包括地址、配置在内的信息。

USB 设备从生到死都要按照这么几个状态，遵循这么一个过程。它不可能像咱们的房价， 林志伶的胸部一样跳跃式的发展。 340 行，speed，设备的速度，这也是个枚举变量

```
548 /* USB 2.0 defines three speeds, here's how Linux identifies them */
549
550 enum usb_device_speed {
551 USB_SPEED_UNKNOWN = 0, /* enumerating */
552 USB_SPEED_LOW, USB_SPEED_FULL, /* usb 1.1 */
553 USB_SPEED_HIGH, /* usb 2.0 */
554 USB_SPEED_VARIABLE, /* wireless (usb 2.5) */
555 }; 
```

地球人都知道，USB 设备有三种速度，低速，全速，高速。USB1.1 那会儿只有低速，全 速，后来才出现了高速，就是所谓的 480Mbps/s。这里还有个 USB_SPEED_VARIABLE， 是无线 USB 的，号称 usb 2.5，还在发展中，据说小黑的 T61 已经支持了，向往 ing。 USB_SPEED_UNKNOWN 只是表示现阶段还不知道这个设备究竟什么速度。

##### transaction translator

342 行，tt，343 行，ttport。知道 tt 干嘛的吗？tt 叫做 transaction translator。你可 以把它想成一块特殊的电路，是 hub 里面的电路，确切的说是高速 hub 中的电路，我们知 道 usb 设备有三种速度的，分别是 low speed，full speed，high speed。即所谓的低速 /全速/高速，抗日战争那会儿，这个世界上只有 low speed/full speed 的设备，没有 high speed 的设备，后来解放后，国民生产力的大幅度提升催生了一种 high speed 的设备， 包括主机控制器，以前只有两种接口的，OHCI/UHCI，这都是在 usb spec 1.0 的时候， 后来 2.0 推出了 EHCI，高速设备应运而生。Hub 也有高速 hub 和过去的 hub，但是这里 就有一个兼容性问题了，高速的 hub 是否能够支持低速/全速的设备呢？一般来说是不支持 的，于是有了一个叫做 TT 的电路，它就负责高速和低速/全速的数据转换，于是，如果一 个高速设备里有这么一个 TT，那么就可以连接低速/全速设备，如不然，那低速/全速设备 没法用，只能连接到 OHCI/UHCI 那边出来的 hub 口里

##### toggle[2]

这个数组只有两个元素，分别对应 IN 和 OUT 端点，每一个端点占 一位。似乎这么说仍是在雾中看花，黑格尔告诉我们，存在就是有价值的，那么这个数组存 在的价值是什么？一言难尽，说来话长，那就长话长说好了。

咱们凄苦的人生是从第一声哭开始，所有的 packets 都从一个 SYNC 同步字段开始，SYNC 是一个 8 位长的二进制串，只是用来同步用的，它的最后两位标志了 SYNC 的结束和 PID （Packet Identifer）的开始，就像咱们的大四标志了梦想的结束和现实的开始。PID 也是 一个 8 位的二进制串，前四位用来区分不同的 packet 类型，后面四位只是前四位的反码， 校验用的。packet 的类型主要有四种，在 spec 中的 Table 8-1 里有说明

##### PID type

![image-20210406130822880](/home/zhaoyong/.config/Typora/typora-user-images/image-20210406130822880.png)

##### packet包

![image-20210406132652949](/home/zhaoyong/.config/Typora/typora-user-images/image-20210406132652949.png)

usb 总线最多只能有 128 个设备，即使是高速设备除了 0 号端点也最多只能有 15 个 in 端 点和 15 个 out 端点。地址字段再往后是 11 位的帧号（frame number），值达到 7FFH 时归零，像一个个无聊的夜晚一样循环往复。这个帧号并不是每一个 packet 都会有，它只 在每帧或微帧（Mircoframe）开始的 SOF Token 包里发送。帧是对于低速和全速模式来 说的，一帧就是 1ms，对于高速模式的称呼是微帧，一个微帧为 125 微妙，每帧或微帧当 然不会只能传一个 packet。帧号再往后就是千呼万唤始出来的 Data 字段了，它可以有 0 到 1024 个字节不等。最后还有 CRC 校验字段来做扫尾工作。 咱们要学习 packet，做一个有理想有目标的人，所以这里只看看 Data 类型的 packet。前 面的 Table 8-1 里显示，有四种类型的 Data 包，DATA0，DATA1，DATA2 和 MDATA。 存在就是有价值的，这里分成 4 种数据包自然有里面的道理，其中 DATA0 和 DATA1 就可 以用来实现 data toggle 同步，看到 toggle，好像有点接近不久之前留下的疑问了。 对于批量传输、控制传输和中断传输来说，数据包最开始都是被初始化为 DATA0 的，然后 为了传输的正确性，就一次传 DATA0，一次传 DATA1，一旦哪次打破了这种平衡，主机就 可以认为传输出错了。对于等时传输来说，data toggle 并不被支持。USB 就是在使用这 种简单的哲学来判断对于错，而我们的生活中有的只是复杂，即使一个馒头都能引起一个两 亿多的血案。

![image-20210406132852960](/home/zhaoyong/.config/Typora/typora-user-images/image-20210406132852960.png)

我们的 struct usb_device 中的数组 unsigned int toggle[2]就是为了支持这种简单的哲 学而生的，它里面的每一位表示的就是每个端点当前发送或接收的数据包是 DATA0 还是 DATA1。

348 行，parent，struct usb_device 结构体的 parent 自然也是一个 struct usb_device 指针。对于 Root Hub，前面说过，它是和 Host Controller 是绑定在一起的，它的 parent 指针在 Host Controller 的驱动程序中就已经赋了值，这个值就是 NULL，换句话说，对于 Root Hub，它不需要再有父指针了，这个父指针就是给从 Root Hub 连出来的节点用的。 USB 设备是从 Root Hub 开始，一个一个往外面连的，比如 Root Hub 有 4 个口，每个口 连一个 USB 设备，比如其中有一个是 Hub，那么这个 Hub 有可以继续有多个口，于是一 级一级的往下连，最终连成了一棵树。 349 行，bus，没什么说的，设备所在的那条总线。 350 行，ep0，端点 0 的特殊地位决定了她必将受到特殊的待遇，在 struct usb_device 对象产生的时候它就要初始化。

353 行，dev，嵌入到 struct usb_device 结构里的 struct device 结构。

354 行，desc，设备描述符，四大描述符的第三个姗姗而来。它在 include/linux/usb/ch9.h 里定义

##### 设备描述符

```
203 /* USB_DT_DEVICE: Device descriptor */
204 struct usb_device_descriptor {
205 __u8 bLength;
206 __u8 bDescriptorType;
207
208 __le16 bcdUSB;
209 __u8 bDeviceClass;
210 __u8 bDeviceSubClass;
211 __u8 bDeviceProtocol;
212 __u8 bMaxPacketSize0;
213 __le16 idVendor;
214 __le16 idProduct;
215 __le16 bcdDevice;
216 __u8 iManufacturer;
217 __u8 iProduct;
218 __u8 iSerialNumber;
219 __u8 bNumConfigurations;
220 } __attribute__ ((packed));
221
222 #define USB_DT_DEVICE_SIZE 18 
```

205 行 ， bLength ，描述符的长度，可以自己数数，或者看紧接着的定义 USB_DT_DEVICE_SIZE。

206 行，bDescriptorType，这里对于设备描述符应该是 USB_DT_DEVICE，0x01。

208 行，bcdUSB，USB spec 的版本号，一个设备如果能够进行高速传输，那么它设备描 述符里的 bcdUSB 这一项就应该为 0200H。

209 行，bDeviceClass，210 行，bDeviceSubClass，211 行，bDeviceProtocol，和 接口描述符的意义差不多，前面说了这里就不再罗唆了。

212 行，bMaxPacketSize0，端点 0 一次可以处理的最大字节数，端点 0 的属性却放到 设备描述符里去了，更加彰显了它突出的江湖地位，它和机器人公敌里的机器人 Sonny， 交大南门外的老赵烤肉一样特别一样独一无二。

前面说端点的时候说了端点 0 并没有一个专门的端点描述符，因为不需要，基本上它所有 的特性都在 spec 里规定好了的，然而，别忘了这里说的是“基本上”，有一个特性则是不 60 一样的，这叫做 maximum packet size，每个端点都有这么一个特性，即告诉你该端点 能够发送或者接收的包的最大值。对于通常的端点来说，这个值被保存在该端点描述符中的 wMaxPacketSize 这一个 field，而对于端点 0 就不一样了，由于它自己没有一个描述符， 而每个设备又都有这么一个端点，所以这个信息被保存在了设备描述符里，所以我们在设备 描述符里可以看到这么一项，bMaxPacketSize0。而且 spec 还规定了，这个值只能是 8， 16，32 或者 64 这四者之一，如果一个设备工作在高速模式，这个值还只能是 64，如果 是工作在低速模式，则只能是 8，取别的值都不行。

213 行，idVendor，214 行，idProduct，分别是厂商和产品的 ID。

215 行，bcdDevice，设备的版本号。

216 行，iManufacturer，217 行，iProduct，218 行，iSerialNumber，分别是厂商， 产品和序列号对应的字符串描述符的索引值。

219 行，bNumConfigurations，设备当前速度模式下支持的配置数量。有的设备可以在 多个速度模式下操作，这里包括的只是当前速度模式下的配置数目，不是总的配置数目。



##### USB 的设备请求

这里提到了 GET_DESCRIPTOR 请求，就顺便简单提一下 USB 的设备请求（device request）。协议里说了，所有的设备通过缺省的控制管道来响应主机的请求，既然使用的 是控制管道，那当然就是控制传输了，这些请求的底层 packet 属于 Setup 类型，前面的 那张表里也可以看到它，在 Setup 包里包括了请求的各种参数。协议里同时也定义了一些 标准的设备请求，并规定所有的设备必须响应它们，即使它们还处于 Default 或 Address 61 状态。这些标准的设备请求里，GET_DESCRIPTOR 就赫然在列。

SET_CONFIGURATION 请求

##### USB配置

```
244 struct usb_host_config {
245 struct usb_config_descriptor desc;
246
247 char *string; /* iConfiguration string, if present */
这个字符串保存了配置描述符iConfiguration字段对应的字符串描述
符信息。
248 /* the interfaces associated with this configuration,
249 * stored in no particular order */
250 struct usb_interface *interface[USB_MAXINTERFACES];
251
配置所包含的接口。注释里说的很明确，
这个数组的顺序未必是按照配置里接口号的顺序，所以你要想得到某个接口号对应的struct
usb_interface 结构对象，就必须使用 usb.c 里定义的 usb_ifnum_to_if 函数。
252 /* Interface information available even when this is not the
253 * active configuration */
254 struct usb_interface_cache *intf_cache[USB_MAXINTERFACES];
255
256 unsigned char *extra; /* Extra descriptors */
257 int extralen;
258 }; 
```

```
84 struct usb_interface *usb_ifnum_to_if(const struct usb_device *dev,
85 unsigned ifnum)
86 {
87 struct usb_host_config *config = dev->actconfig;
88 int i;
89
90 if (!config)
91 return NULL;
92 for (i = 0; i < config->desc.bNumInterfaces; i++)
93 if (config->interface[i]->altsetting[0]
94 .desc.bInterfaceNumber == ifnum)
95 return config->interface[i];
96
97 return NULL;
98 } 
```

这个函数的道理很简单，就是拿你指定的接口号，和当前配置的每一个接口可选设置 0 里 的接口描述符的 bInterfaceNumber 字段做比较，相等了，那个接口就是你要寻找的，都 不相等，那对不起，不能满足你的要求，虽然它已经尽力了。

这个函数的道理很简单，就是拿你指定的接口号，和当前配置的每一个接口可选设置 0 里 的接口描述符的 bInterfaceNumber 字段做比较，相等了，那个接口就是你要寻找的，都 不相等，那对不起，不能满足你的要求，虽然它已经尽力了。 如果你看了协议，可能会在 9.6.5 里看到，请求配置描述符时，配置里的所有接口描述符 是按照顺序一个一个返回的。那为什么这里又明确说明，让咱们不要期待它就会是接口号的 顺序那？其实很久很久以前这里并不是这么说地，它就说这个数组是按照 0..desc.bNumInterfaces的顺序，但同时又说需要通过 usb_ifnum_to_if函数来获得指 定接口号的接口对象，Alan Stern大侠质疑了这种有些矛盾的说法，于是David Brownell 大侠就把它改成现在这个样子了，为什么改？因为协议归协议，厂商归厂商，有些厂商就是 有不遵守协议的癖好，它非要先返回接口 1 再返回接口 0，你也没辙，所以就不得不增加 usb_ifnum_to_if函数。

USB_MAXINTERFACES 是 include/linux/usb.h 里定义的一个宏，值为 32，不要说不 够用，谁见过有很多接口的设备？ 176 /* this maximum is arbitrary */ 177 #define USB_MAXINTERFACES 32 

 254 行，intf_cache[USB_MAXINTERFACES]，cache 是什么？缓存。答对了，不然 大学四年岂不是光去吃老赵烤肉了。这是个 struct usb_interface_cache 对象的结构数 组，usb_interface，usb 接口，cache，缓存，所以 usb_interface_cache 就是 usb 接 口的缓存。缓存些什么？看看 include/linux/usb/usb.h 里的定义

```
193 struct usb_interface_cache {
194 unsigned num_altsetting; /* number of alternate settings */
195 struct kref ref; /* reference counter */
196
197 /* variable-length array of alternate settings for this interface,
198 * stored in no particular order */
199 struct usb_host_interface altsetting[0];
200 }; 
```

199 行 的 altsetting[0] 是一个可变长数组，按需分配的那种，你对设备说 GET_DESCRIPTOR 的时候，内核就根据返回的每个接口可选设置的数目分配给 intf_cache数组相应的空间，有多少需要多少分配多少，在咱们还在为拥有一套房而奋斗 终生的时候，这里已经提前步入了共产主义。 为什么要缓存这些东东？房价在变，物价在变，设备的配置也在变，此时这个配置可能还在 欢快的被宠幸着，彼时它可能就躲在冷宫里写《后宫回忆录》，漫长的等待之后，哪个导演 慧眼识剧本发现了它，她就又迎来了自己的第二春。这就叫此一时彼一时。为了在配置被取 代之后仍然能够获取它的一些信息，就把日后可能会需要的一些东东放在了intf_cache数 组的struct usb_interface_cache对象里。谁会需要？这么说吧，你通过sysfs这个窗口 只能看到设备当前配置的一些信息，即使是这个配置下面的接口，也只能看到接口正在使用 的那个可选设置的信息，可是你希望能够看到更多的，怎么办，窗户太小了，可以趴门口看， usbfs就是这个门，里面显示有你的系统中所有usb设备的可选配置和端点信息，它就是利 用intf_cache这个数组里缓存的东东实现的。

256 行 ， extra ， 257 行 ， extralen ，有关额外扩展的描述符的，和 struct usb_host_interface 里的差不多，只是这里的是针对配置的，如果你使用 GET_DESCRIPTOR请求从设备里获得配置描述符信息，它们会紧跟在标准的配置描述符 后面返回给你。



###### 配置描述符

245 行 ， desc ，四大描述符里最后的一个终于出现了，同样是在它们的老巢 include/linux/usb/ch9.h 里定义

```
258 struct usb_config_descriptor {
259 __u8 bLength;
bLength，描述符的长度，值为 USB_DT_CONFIG_SIZE
260 __u8 bDescriptorType;
bDescriptorType，描述符的类型，值为 USB_DT_CONFIG，0x02。这么说对
不对？按照前面接口描述符、端点描述符和设备描述符的习惯来说，应该是没问题。但是，
生活总是会在我们已经习惯它的时候来个转折，这里的值却并不仅仅可以为
USB_DT_CONFIG，还可以为USB_DT_OTHER_SPEED_CONFIG，0x07。这里说的
OTHER_SPEED_CONFIG描述符描述的是高速设备操作在低速或全速模式时的配置信息，
和配置描述符的结构完全相同，区别只是描述符的类型不同，是只有名字不同的孪生兄弟。
261
262 __le16 wTotalLength;
使用 GET_DESCRIPTOR 请求从设备里获得配置描述符信息时，
返回的数据长度，也就是说对包括配置描述符、接口描述符、端点描述符，class-或
vendor-specific 描述符在内的所有描述符算了个总帐。
263 __u8 bNumInterfaces;
这个配置包含的接口数目。
bConfigurationValue，对于拥有多个配置的幸运设备来说，可以拿这个值为参
数，使用 SET_CONFIGURATION 请求来改变正在被使用的 USB 配置，
bConfigurationValue 就指明了将要激活哪个配置。咱们的设备虽然可以有多个配置，但
 65
同一时间却也只能有一个配置被激活。捎带着提一下，SET_CONFIGURATION 请求也是
标准的设备请求之一，专门用来设置设备的配置。
264 __u8 bConfigurationValue;
265 __u8 iConfiguration;
描述配置信息的字符串描述符的索引值。
266 __u8 bmAttributes;
这个字段表征了配置的一些特点，比如 bit 6 为 1 表示
self-powered，bit 5 为 1 表示这个配置支持远程唤醒。另外，它的 bit 7 必须为 1，为什
么？协议里就这么说的，我也不知道，这个世界上并不是什么事情都找得到原因的。ch9.h
里有几个相关的定义
267 __u8 bMaxPower;
bMaxPower，设备正常运转时，从总线那里分得的最大电流值，以 2mA 为单位。
设备可以使用这个字段向 hub 表明自己需要的的电流，但如果设备需求过于旺盛，请求的
超出了 hub 所能给予的，hub 就会直接拒绝，不会心软。你去请求她给你多一点点爱，可
她心系天下人，没有多的分到你身上，于是怎么办？拒绝你呗，不要说爱情是多么残酷，这
个世界就是很无奈。还记得 struct usb_device 结构里的 bus_mA 吗？它就表示 hub 所
能够给予的。Alan Stern 大侠告诉我们
268 } __attribute__ ((packed));
269
270 #define USB_DT_CONFIG_SIZE 9 
```

```
272 /* from config descriptor bmAttributes */
273 #define USB_CONFIG_ATT_ONE (1 << 7) /* must be set */
274 #define USB_CONFIG_ATT_SELFPOWER (1 << 6) /* self powered */
275 #define USB_CONFIG_ATT_WAKEUP (1 << 5) /* can wakeup */
276 #define USB_CONFIG_ATT_BATTERY (1 << 4) /* battery powered */ 
```

266 行，bmAttributes，这个字段表征了配置的一些特点，比如 bit 6 为 1 表示 self-powered，bit 5 为 1 表示这个配置支持远程唤醒。另外，它的 bit 7 必须为 1，为什 么？协议里就这么说的，我也不知道，这个世界上并不是什么事情都找得到原因的。ch9.h 里有几个相关的定义

##### usb描述符

usb 描述符，设备描述符，配置描述符， 接口描述符和端点描述符，协议里规定一个 usb 设备是必须支持这四大描述符的，当然也 有其**它一些描述符来让设备可以显得个性些，**但这四大描述符是一个都不能少的。

usb 设备里都会有一个叫 EEPROM 的东东，没错，就是放在它那儿，它就是用来存储设备 本身信息的。如果你的脑海里还残存着一些大学里的美好时光的话，应该还会记得 EEPROM 45 就是电可擦写的可编程 ROM，它与 Flash 虽说都是要电擦除的，但它可以按字节擦除，Flash 只能一次擦除一个 block，所以如果要改动比较少的数据的话，使用它还是比较合适的，但 是世界上没有完美的东西，此物成本相对 Flash 比较高，所以一般来说 usb 设备里只拿它 来存储一些本身特有的信息，要想存储数据，还是用 Flash 吧。



## 设备的生命线（一）

BH 的人生有 BH 的活法，设备的人生有设备的过法。设备也有它自己的生命线，自你把它 插到 hub 上始，自你把它从 hub 上拔下来终，它的一生是勤勉努力、朴实无华的一生，它 的一生是埋头苦干、默默奉献的一生。BH 的人生不需要解释，设备的人生值得我们去分析。

1.首先当然是你将 usb 设备连接在 hub 的某个端口上，hub 检测到有设备连接了进来，它也 知道有朋自远方来不亦乐乎，于是精神头儿就上来了，就觉得有必要为设备做点什么。

2.它会 为设备分配一个 struct usb_device 结构的对象并初始化，并调用设备模型提供的接口将 设备添加到 usb 总线的设备列表里

3.然后 usb 总线会遍历驱动列表里的每个驱动，调用自 己的 match 函数看它们和你的设备或接口是否匹配。



hub 检测到自己的某个端口有设备连接了进来后，它会调用 core 里的 usb_alloc_dev 函 数为 struct usb_device 结构的对象申请内存，这个函数在 usb.c 文件里定义

### usb_alloc_dev

```
238 struct usb_device *
239 usb_alloc_dev(struct usb_device *parent, struct usb_bus *bus, unsigned port1)
240 {
241 struct usb_device *dev;
242
243 dev = kzalloc(sizeof(*dev), GFP_KERNEL);
244 if (!dev)
245 return NULL;
246
247 if (!usb_get_hcd(bus_to_hcd(bus))) {
248 kfree(dev);
249 return NULL;
250 }
251
252 device_initialize(&dev->dev);
device_initialize 是设备模型里的函数，这里就是将 struct usb_device 结构里
嵌入的那个 struct device 结构体初始化掉，以后好方便用，初始化些什么？以前资本家宁
可把牛奶倒掉也不给穷人喝，现在房产商宁可把房子空着也不给百姓住，而我宁可自己不知
道也不告诉你。
253 dev->dev.bus = &usb_bus_type;
253 行，将设备所在的总线类型设置为 usb_bus_type。usb_bus_type 咱们很早很早就
遇到它了，usb 子系统的初始化函数 usb_init 里就把它给注册掉了，还记得聊到模型的时
候说过的那个著名的三角关系不，这里就是把设备和总线这条边儿给搭上了。如果您忘了，
那就回头看看吧。
254 dev->dev.type = &usb_device_type;
将设备的设备类型初始化为usb_device_type，这是咱们上节第二次遇到
usb_device_match函数，走设备那条路，使用 is_usb_device判断是不是usb设备时留
下的疑问，就是在这儿把设备的类型给初始化成usb_device_type了。
255 dev->dev.dma_mask = bus->controller->dma_mask;
这个就是与 DMA 传输相关的了，设备能不能进行 dma 传输，得看主机控制器的
脸色，主机控制器不支持的话设备自作多情那也没有用。所以这里 dma_mask 被设置为
host controller 的 dma_mask。
256 dev->state = USB_STATE_ATTACHED;
，将 usb 设备的状态设置为 Attached，表示设备已经连接到 usb 接口上了，是 hub
检测到设备时的初始状态。咱们前面说了，USB 设备从生到死都要按照那么几个状态，这
里随着设备生命线的逐渐深入，咱们会看到设备的状态也在逐渐的变化。
257
258 INIT_LIST_HEAD(&dev->ep0.urb_list);
端点 0 实在是太太太特殊了，这个咱们是一而再再而三的感叹，struct usb_device
里直接就有这么一个成员 ep0，这行就将 ep0 的 urb_list 给初始化掉。因为接下来遇到的
那些主要角色的成员前面集中都说过了，咱们就不再说它们是嘛意思了，忘了的话可以到前
面看看。
259 dev->ep0.desc.bLength = USB_DT_ENDPOINT_SIZE;
260 dev->ep0.desc.bDescriptorType = USB_DT_ENDPOINT;
分别初始化了端点 0 的描述符长度和描述符类型。
261 /* ep0 maxpacket comes later, from device descriptor */
262 dev->ep_in[0] = dev->ep_out[0] = &dev->ep0;
263使 struct usb_device 结构里的 ep_in 和 ep_out 指针数组的第一个成员指向
ep0，ep_in[0]和 ep_out[0]本来表示的就是端点 0。
264 /* Save readable and stable topology id, distinguishing devices
265 * by location for diagnostics, tools, driver model, etc. The
266 * string is a path along hub ports, from the root. Each device's
267 * dev->devpath will be stable until USB is re-cabled, and hubs
268 * are often labeled with these port numbers. The bus_id isn't
269 * as stable: bus->busnum changes easily from modprobe order,
270 * cardbus or pci hotplugging, and so on.
271 */
272 if (unlikely(!parent)) {
273 dev->devpath[0] = '';
274使用的时候还是很简单的，就是，if 语句你照用，只是如果你觉得 if 条件为 1 的可能性非
常大的时候，可以在条件表达式外面包装一个 likely()，如果可能性非常小，则用 unlikely()
包装。那么这里 272 行的意思就很明显了，就是说写内核的哥们儿觉得你的 usb 设备直接
连接到 root hub 上的可能性比较小，因为 parent 指的就是你的设备连接的那个 hub。
275 dev->dev.parent = bus->controller;
276 sprintf(&dev->dev.bus_id[0], "usb%d", bus->busnum);
277 } else {
278 /* match any labeling on the hubs; it's one-based */
279 if (parent->devpath[0] == '') 
 79
280 snprintf(dev->devpath, sizeof dev->devpath,
281 "%d", port1);
282 else
283 snprintf(dev->devpath, sizeof dev->devpath,
284 "%s.%d", parent->devpath, port1);
285
286 dev->dev.parent = &parent->dev;
287 sprintf(&dev->dev.bus_id[0], "%d-%s",
288 bus->busnum, dev->devpath);
289
290 /* hub driver sets up TT records */
291 }
292
293 dev->portnum = port1;
294 dev->bus = bus;
295 dev->parent = parent;
296 INIT_LIST_HEAD(&dev->filelist);
297
298 #ifdef CONFIG_PM
299 mutex_init(&dev->pm_mutex);
300 INIT_DELAYED_WORK(&dev->autosuspend, usb_autosuspend_work);
301 dev->autosuspend_delay = usb_autosuspend_delay * HZ;
302 #endif
303 return dev;
304 } 
```

usb_alloc_dev 函数就相当于 usb 设备的构造函数，参数里边儿，parent 是设备连接的 那个 hub，bus 是设备连接的那条总线，port1 就是设备连接在 hub 上的那个端口。

243 行，为一个 struct usb_device 结构的对象申请内存并初始化为 0。直到在看到这一 行的前一天，我还仍在使用 kmalloc 加 memset 这对儿最佳拍档来申请内存和初始化，但 是在看到 kzalloc 之后，我知道了江山代代有人出，眉先生，须后生，先生不及后生长的道 理，没看那些 90 后的都在嘲笑咱们 80 后的不知道 kzalloc 了么。kzalloc 直接取代了 kmalloc/memset，一个函数起到了两个函数的作用。这种角色当然是最讨人喜欢的了， 所以说现在那些简历上搞什么艺术照片儿性感写真什么的完全毫无必要，只要写上个能够做 牛做马，白天做牛晚上做马，那些老板就乐翻天了，哪还用玩儿那些虚头。

然后是判断内存申请成功了没，不成功就不用往下走了。那么通过这么几行，咱们应该记住 两个凡是，凡是你想用 kmalloc/memset 组合申请内存的时候，就使用 kzalloc 代替吧， 凡是申请内存的，不要忘了判断是不是申请成功了。我们的心里应该把这两个凡是提高到和 四项基本原则一样的地位。

里一个主机控制器对应着一条usb总线，主机控制器驱动用struct usb_hcd结构表示，一 条总线用struct usb_bus结构表示，函数bus_to_hcd是为了获得总线对应的主机控制器 驱动，也就是struct usb_hcd结构对象，函数usb_get_hcd只是将得到的这个usb_hcd 结构对象的引用计数加 1，为什么？因为总线上多了一个设备，设备在主机控制器的数据结 构就得在，当然得为它增加引用计数。如果这俩函数没有很好的完成自己的任务，那整个 usb_alloc_dev函数也就没有继续执行下去的必要了，将之前为struct usb_device结构对 象申请的内存释放掉就稍息去吧。

272~291 行整个的代码就是首先判断你的设备是不是直接连到 root hub 上的，如果是， 将 dev->devpath[0]赋值为‘0’，以示特殊，然后父设备设为 controller，同时把 dev->bus_id[]设置为像 usb1/usb2/usb3/usb4 这样的字符串。如果你的设备不是直接 连到 root hub 上的，有两种情况，如果你的设备连接的那个 hub 是直接连到 root hub 上 的，则 dev->devpath 就等于端口号，否则 dev->devpath 就等于在父 hub 的 devpath 基础上加一个‘.’再加一个端口号，最后把 bus_id[]设置成 1-/2-/3-/4-这样的字符串后 面连接上 devpath。

293~295 行，没什么说的，轻轻的飘过。 

296 行，初始化一个队列，usbfs 用的。

 298~302 行，电源管理的，仍然飘过。

## likely and unlikely

```
60 #define likely(x) __builtin_expect(!!(x), 1)
61 #define unlikely(x) __builtin_expect(!!(x), 0) 
```

unlikely 不是一个人在奋斗，还有个 likely。定义里那个怪怪的__builtin_expect 是 GCC 里内建的一个函数，具体是做嘛用的可以看看 GCC 的手册 long __

```
builtin_expect (long exp, long c) 
```

大致意思就是由于大部分写代码的在分支预测方面做的比较的糟糕，所以 GCC 提供了这个 内建的函数来帮助处理分支预测，优化程序，它的第一个参数 exp 为一个整型的表达式， 返回值也是这个 exp，它的第二个参数 c 的值必须是一个编译期的常量，那这个内建函数 的意思就是 exp 的预期值为 c，编译器可以根据这个信息适当的重排条件语句块的顺序， 将符合这个条件的分支放在合适的地方。具体到 unlikely(x)就是告诉编译器条件 x 发生的 可能性不大，那么这个条件块儿里语句的目标码可能就会被放在一个比较远的为止，以保证 经常执行的目标码更紧凑。likely 则相反。在一个月高风黑的夜晚，你向你从穿开裆裤就开 始暗恋的 mm 表白，她回复你 if(unlikely(你以后会有房子，不是按揭的，会有车子，不是奥拓)) 咱们明天可以去领证儿 82 你觉得你是应该高兴还是不应该高兴？她这明摆着就是告你别做梦了，你有房子车子的可能 性太小了，往远处排吧，她要先照顾有房有车的。如果没看过内核，你说不定还兴高采烈觉 得成了那，你会觉得以后房子车子还不是小 case。所以，这里要感叹一下，读内核是多么 重要啊。



## 设备的生命线（二）

现在设备的 struct usb_device 结构体已经准备好了，只是还不怎么饱满，hub 接下来就 会给它做做整容手术，往里边儿塞点什么，充实一些内容，比如：将设备的状态设置为 Powered，也就是加电状态；因为此时还不知道设备支持的速度，于是将设备的 speed 成 员暂时先设置为 USB_SPEED_UNKNOWN；设备的级别 level 当然会被设置为 hub 的 level 加上 1 了；还有为设备能够从 hub 那里获得的电流赋值；为了保证通信畅通，hub 还会为设备在总上选择一个独一无二的地址。

经历了几千里路的云和月，属于设备的那个 struct usb_device 结构体现在又有了多少的 功名尘与土？给张表吧，集中列了下到目前为止，设备结构体里成员的状况，这里不用藏着 掖着，整就整了，不都是为了生活么。里面的 taken 只是表示赋过值了，好像期末考试前 你去突击自习，这时一个 ppmm 走到你旁边，用一个美妙的声音问你“Is this seat taken?”，你怎么回答？当然是“No, No, please.”，那如果星爷片子里面的如花过来问你 83 那？你回答什么？对头，taken 就是这个意思。那赋了什么值？没必要知道的那么详细， 给别人留点自尊，歌里唱的好，人人都需要一点隐私.

![image-20210406193444519](/home/zhaoyong/.config/Typora/typora-user-images/image-20210406193444519.png)

那即使是傻强过来瞅瞅，也知道你的设备现在已经处在了 Powered 状态。前 面讲过的，设备要想从 Powered 状态发展到下一个状态 Default，必须收到一个复位信号 并成功复位。就好像你要想和你 mm 从暧昧关系发展到你梦寐以求的以身相许的地步，不 是随随便便就可以的，情调、money 一个都不能少。那 hub 接下来的动作就很明显了，复 位设备，复位成功后，设备就会进入 Default 状态。

现在就算设备成功复位了，大步迈进了 Default 状态，同时，h**ub 也会获得设备真正的速 度，低速、全速也好，高速也罢，总算是浮出水面了**，speed 也终于知道了自己的真正身 份，不用再是 UNKNOWN 了。那根据这个速度，咱们能知道些什么？**起码能够知道端点 0 一次能够处理的最大数据长度啊**，协议里说，对于高速设备，这个值为为 64 字节，对于低 速设备为 8 字节，而对于全速设备可能为 8，16，32，64 其中的一个。遇到这种模棱两 84 可的答案，写代码的哥们儿不会满足，咱们也不会满足，所以 hub 还要通过一个蜿蜒曲折 的过程去获得这个确定的值，至于怎么个曲折法儿，此处省略 2008 字。



咱们的设备要想进入 **Address 状态**没那么费劲儿，只要 hub 使用 core 里定义的一个函数 usb_control_msg，发送 SET_ADDRESS 请求给设备，设备就兴高采烈的迈进 Address 了。那么设备的这个 address 是什么，就是上面的 devnum 啊，说过的。不用羡慕它们太 久，经过了 10 年，百年，千年，咱们也会 Address 的。

### usb_control_msg

这个函数主要目的是创建一个控制 urb，并把它发送给 usb 设备，然后等待它完成。u

```
120 int usb_control_msg(struct usb_device *dev, unsigned int pipe, __u8 request,
__u8 requesttype,
121 __u16 value, __u16 index, void *data, __u16 size, int
timeout)
122 {
123 struct usb_ctrlrequest *dr = kmalloc(sizeof(struct usb_ctrlrequest),
GFP_NOIO);123 行，为一个 struct usb_ctrlrequest 结构体申请了内存，这里又出现了一个新生事物，
郭天王早提醒我们了，这里的结构说不完。它在 include/linux/usb/ch9.h 文件里定义
124 int ret;
125
126 if (!dr)
127 return -ENOMEM;
128
129 dr->bRequestType= requesttype;
130 dr->bRequest = request;
131 dr->wValue = cpu_to_le16p(&value);
132 dr->wIndex = cpu_to_le16p(&index);
133 dr->wLength = cpu_to_le16p(&size);
 struct usb_ctrlrequest 的约会暂时就到这里，回到 usb_control_msg 函数里。很明
显要进行控制传输，得首先创建一个 struct usb_ctrlrequest 结构体，填上请求的内容。
129 到 133 行就是来使用传递过来的参数初始化这个结构体的。对于刚开始提到的
SET_ADDRESS 来说，bRequest 的值就是 USB_REQ_SET_ADDRESS，标准请求之一，
ch9.h 里定义有。因为 SET_ADDRESS 请求并不需要 DATA 阶段，所以 wLength 为 0，
而且这个请求是针对设备的，所以 wIndex 也为 0。这么一来，bRequestType 的值也只
能为 0 了。因为是设置设备地址的，总得把要设置的地址发给设备，不然设备会比咱们还
一头雾水不知道主机是嘛个意思，所以请求的参数 wValue 就是之前 hub 已经你的设备指
定好的 devnum。其实 SET_ADDRESS 请求各个部分的值 spec 9.4.6 里都有规定，就和
我这里说的一样，不信你去看看。

134
135 //dbg("usb_control_msg");
136
137 ret = usb_internal_control_msg(dev, pipe, dr, data, size, timeout);
138
139 kfree(dr);
140
141 return ret;
142 } 
```

#### 主机通过控制传输发送给设备的请求

```
140 struct usb_ctrlrequest {
141 __u8 bRequestType;
142 __u8 bRequest;
143 __le16 wValue;//这个字段是 request 的参数，request 不同，wValue 就不同。
144 __le16 wIndex;//也是 request 的参数，bRequestType 指明 request 针对的是设备上
的某个接口或端点的时候，wIndex 就用来指明是哪个接口或端点。
145 __le16 wLength;//，控制传输中 DATA transaction 阶段的长度，方向已经在
bRequestType 那儿指明了。如果这个值为 0，就表示没有 DATA transaction 阶段，
146 } __attribute__ ((packed)); 
```

这个结构描述的 request 都在 Setup 包里发送，Setup 包是前面某处说到的 Token PID 类型中的一种，为了你好理解，这里细说一下控制传输底层的 packet 情况。控制传输最少 要有两个阶段的 transaction，SETUP 和 STATUS，SETUP 和 STATUS 中间的那个 DATA 阶段是可有可无的。Transaction 这个词儿在很多地方都有，也算是个跨地区跨学科的热门 词汇了，在这里你称它为事务也好，会话也罢，我还是直呼它的原名 transaction，可以理 解为主机和设备之间形成的一次完整的交流。

usb 的 transaction 要比上面的对话复杂，起码要过过脑子， 它可以包括一个 Token 包、一个 Data 包和一个 Handshake 包。

Token、Data 和 Handshake 都属于四种 PID 类型中的，前面说到时提到的一个包里的那 些部分，如 SYNC、PID、地址域、DATA、CRC，并不是所有 PID 类型的包都会全部包括 的。Token 包只包括 SYNC、PID、地址域、CRC，并没有 DATA 字段，它的名字起的很 形象，就是用来标记所在 transaction 里接下来动作的，对于 Out 和 Setup Token 包， 里面的地址域指明了接下来要接收 Data 包的端点，对于 In Token 包，地址域指明了接下 来哪个端点要发送 Data 包。还有，只有主机才有权利发送 Token 包，协议里就这么规定 的。

与 Token 包相比，Data 包里没了地址域，多了 Data 字段，这个 Data 字段对于低速设备 最大为 8 字节，对于全速设备最大为 1023 字节，对于高速设备最大为 1024 字节。里里 外外看过去，它就是躲在 Token 后边儿用来传输数据的。Handshake 包的成分就非常的 简单了，简直和那位姚佳雯的回答一样简单，除了 SYNC，它就只包含了一个 PID，通过 PID 取不同的值来报告一个 transaction 的状态，比**如数据已经成功接收**了等。

俗话说，兔有三窟，人有三急，控制传输的 SETUP transaction 一般来说也有三个阶段， 就是主机向设备发送 Setup Token 包、然后发送 Data0 包，如果一切顺利，设备回应 ACK Handshake 包表示 OK，为什么加上一般？如果中间的那个 Data0 包由于某种不可知因素 被损坏了，设备就什么都不会回应，这时就成俩阶段了。SETUP transaction 之后，接下 来如果控制传输有 DATA transaction 的话，那就 Data0、Data1 这样交叉的发送数据包， 前面说过这是为了实现 data toggle。最后是 STATUS transaction，向主机汇报前面 SETUP 和 DATA 阶段的结果，比如表示主机下达的命令已经完成了，或者主机下达的命令 没有完成，或者设备正忙着那没功夫去理会主机的那些命令。

这样经过 SETUP、DATA、STATUS 这三个 transaction 阶段，一个完整的控制传输完成 了。主机接下来可以规划下一次的控制传输。

现在对隐藏在控制传输背后的是是非非摸了个底儿，群众的眼睛是雪亮的，咱们现在应该可 以看出之前说 requests 都在 Setup 包里发送是有问题的，因为 Setup 包本身并没有数据 字段，严格来说它们应该都是在 SETUP transaction 阶段里 Setup 包后的 Data0 包里发 送的。还有点糊涂？唉，女人之美，在于蠢得无怨无悔；男人之美，在于说得白日见鬼；诗 歌之美，在于煽动男女出轨；学问之美，在于使人一头雾水。那就带着雾水向下看吧，可能 会有那么云开雾明的一天。

141 行，bRequestType，这个字段别看就一个字节，内容很丰富的，大道理往往都包含 88 这种在小地方。它的 bit7 就表示了控制传输中 DATA transaction 阶段的方向，当然，如 果有 DATA 阶段的话。bit5~6 表示 request 的类型，是标准的，class-specific 的还是 vendor-specific 的。bit0~4 表示了这个请求针对的是设备，接口，还是端点。内核为它 们专门量身定做了一批掩码，也在 ch9.h 文件里，

```
42 /*
43 * USB directions
44 *
45 * This bit flag is used in endpoint descriptors' bEndpointAddress field.
46 * It's also one of three fields in control requests bRequestType.
47 */
48 #define USB_DIR_OUT 0 /* to device */
49 #define USB_DIR_IN 0x80 /* to host */
50
51 /*
52 * USB types, the second of three bRequestType fields
53 */
54 #define USB_TYPE_MASK (0x03 << 5)
55 #define USB_TYPE_STANDARD (0x00 << 5)
56 #define USB_TYPE_CLASS (0x01 << 5)
57 #define USB_TYPE_VENDOR (0x02 << 5)
58 #define USB_TYPE_RESERVED (0x03 << 5)
59
60 /*
61 * USB recipients, the third of three bRequestType fields
62 */
63 #define USB_RECIP_MASK 0x1f
64 #define USB_RECIP_DEVICE 0x00
65 #define USB_RECIP_INTERFACE 0x01
66 #define USB_RECIP_ENDPOINT 0x02
67 #define USB_RECIP_OTHER 0x03
68 /* From Wireless USB 1.0 */
69 #define USB_RECIP_PORT 0x04
70 #define USB_RECIP_RPIPE 0x05 
```

142 行，bRequest，表示具体是哪个 request。

## 设备的生命线（三）

#### usb_internal_control_msg

函数usb_control_msg调用了 usb_internal_control_msg之后就去一边儿睡大觉了，脏 活儿累活儿，全部留给 usb_internal_control_msg去做了，这才叫骨干啊，俺一华为的 哥们儿如是说。那么咱们接下来就给这个骨干多点儿关注，了解一下它背后的真实生活，现 在是焦点访谈时间，要用事实说话。

```
71 static int usb_internal_control_msg(struct usb_device *usb_dev,
72 unsigned int pipe,
73 struct usb_ctrlrequest *cmd,
74 void *data, int len, int timeout)
75 {
76 struct urb *urb;
77 int retv;
78 int length;
79
80 urb = usb_alloc_urb(0, GFP_NOIO);
81 if (!urb)
82 return -ENOMEM;
83 
 90
84 usb_fill_control_urb(urb, usb_dev, pipe, (unsigned char *)cmd, data,
85 len, usb_api_blocking_completion, NULL);
86
87 retv = usb_start_wait_urb(urb, timeout, &length);
88 if (retv < 0)
89 return retv;
90 else
91 return length;
92 } 
```

这个函数粗看过去，可以概括为一个中心，三个基本点，以一个struct urb结构体为中心， 以 usb_alloc_urb、usb_fill_control_urb、usb_start_wait_urb三个函数为基本点。

#### struct urb

一个中心：struct urb结构体，就是咱们前面多次提到又多次飘过，只闻其名不见其形的传 说中的urb，全称usb request block，站在咱们的角度看，usb通信靠的就是它这张脸。

```
1126 struct urb
1127 {
                        1128 /* private: usb core and host controller only fields in the urb */
                        1129 struct kref kref; /* reference count of the URB */
                        1130 spinlock_t lock; /* lock for the URB */
                        lock，一把自旋锁。韦唯早就唱了，每个 urb 都有一把自旋锁。
                        1131 void *hcpriv; /* private data for host controller
                        */
                        走到今天，你应该明白这个 urb 最终还是要提交给主机控制器驱动的，
这个字段就是 urb 里主机控制器驱动的自留地，咱们就不插手了。
                        1132 atomic_t use_count; /* concurrent submissions counter */
                        1133 u8 reject; /* submissions will fail */
                        1134
                        1135 /* public: documented fields in the urb that can be used by drivers */
                        1136 struct list_head urb_list; /* list head for use by the urb's
                        1137 * current owner */
                        1138 struct usb_device *dev; /* (in) pointer to associated device */
                        1139 unsigned int pipe; /* (in) pipe information */
                        1140 int status; /* (return) non-ISO status */
                        1141 unsigned int transfer_flags; /* (in) URB_SHORT_NOT_OK | ...*/
                        1142 void *transfer_buffer; /* (in) associated data buffer */
                        1143 dma_addr_t transfer_dma; /* (in) dma addr for transfer_buffer */
                        1144 int transfer_buffer_length; /* (in) data buffer length */
                        1145 int actual_length; /* (return) actual transfer length */
                        1146 unsigned char *setup_packet; /* (in) setup packet (control only) */
                        1147 dma_addr_t setup_dma; /* (in) dma addr for setup_packet */
                        1148 int start_frame; /* (modify) start frame (ISO) */
                        1149 int number_of_packets; /* (in) number of ISO packets */
                        1150 int interval; /* (modify) transfer interval
                        1151 * (INT/ISO) */
                        1152 int error_count; /* (return) number of ISO errors */
                        1153 void *context; /* (in) context for completion */
                        1154 usb_complete_t complete; /* (in) completion routine */
                        1155 struct usb_iso_packet_descriptor iso_frame_desc[0];
                        1156 /* (in) ISO ONLY */ 
                         95
1157 };
```

1129 行，kref，urb 的引用计数。甭看它是隐藏在 urb 内部的一个不起眼的小角色，但小 角色做大事情，它决定了一个 urb 的生死存亡。一个 urb 有用没用，是继续委以重任还是 无情销毁都要看它的脸色。那第一个问题就来了，为什么 urb 的生死要掌握在这个小小的 引用计数手里边儿？

很早很早很早很早以前就说过，主机与设备之间通过管道来传输数据，管道的一端是主机上 的一个缓冲区，另一端是设备上的端点。管道之中流动的数据，在主机控制器和设备看来是 一个个 packets，在咱们看来就是 urb。因而，端点之中就有那么一个队列，叫 urb 队列。 不过，这并不代表一个 urb 只能发配给一个端点，它可能通过不同的管道发配给不同的端 点，那么这样一来，我们如何知道这个 urb 正在被多少个端点使用，如何判断这个 urb 的 生命已经 over？如果没有任何一个端点在使用它，而我们又无法判断这种情况，它就会永 远的飘荡在 usb 的世界里，犹如飘荡在人冥两届的冤魂。我们需要寻求某种办法在这种情 况下给它们一个好的归宿，这就是引用计数。每多一个使用者，它的这个引用计数就加 1， 每减少一个使用者，引用计数就减一，如果连最后一个使用者都释放了这个 urb，宣称不再 使用它了，那它的生命周期就走到了尽头，会自动的销毁。

###### urb 的引用计数 kref

```
23 struct kref {
24 atomic_t refcount;
25 };
```

这个结构与 struct urb 相比简约到极致了，简直就是迎着咱们的口味来的。不过别看它简 单，内核里就是使用它来判断一个对象还有没有用的。它里边儿只包括了一个原子变量，为 什么是原子变量？既然都使用引用计数了，那就说明可能同时有多个地方在使用这个对象， 总要考虑一下它们同时修改这个计数的可能性吧，也就是俗称的并发访问，那怎么办？加个 锁？就这么一个整数值专门加个锁未免也忒大材小用了些，所以就使用了原子变量。围绕这 个结构，内核里还定义了几个专门操作引用计数的函数，它们在 lib/kref.c 里定义

这个结构与 struct urb 相比简约到极致了，简直就是迎着咱们的口味来的。不过别看它简 单，内核里就是使用它来判断一个对象还有没有用的。它里边儿只包括了一个原子变量，为 什么是原子变量？既然都使用引用计数了，那就说明可能同时有多个地方在使用这个对象， 总要考虑一下它们同时修改这个计数的可能性吧，也就是俗称的并发访问，那怎么办？加个 锁？就这么一个整数值专门加个锁未免也忒大材小用了些，所以就使用了原子变量。围绕这 个结构，内核里还定义了几个专门操作引用计数的函数，它们在 lib/kref.c 里定义

```
21 void kref_init(struct kref *kref)
22 { 
 96
23 atomic_set(&kref->refcount,1);
24 smp_mb();
25 } 

31 void kref_get(struct kref *kref)
32 {
33 WARN_ON(!atomic_read(&kref->refcount));
34 atomic_inc(&kref->refcount);
35 smp_mb__after_atomic_inc();
36 } 

52 int kref_put(struct kref *kref, void (*release)(struct kref *kref))
53 {
54 WARN_ON(release == NULL);
55 WARN_ON(release == (void (*)(struct kref *))kfree);
56
57 if (atomic_dec_and_test(&kref->refcount)) {
58 release(kref);
59 return 1;
60 }
61 return 0;
62 } 
```

整个kref.c文件就定义了这么三个函数，kref_init初始化，kref_get将引用计数加 1， kref_put将引用计数减一并判断是不是为 0，为 0 的话就调用参数里release函数指针指向 的函数把对象销毁掉。它们对独苗儿refcount的操作都是通过原子变量特有的操作函数， 97 其实这句话可以当选当日最大废话，原子变量当然要使用专门的操作函数了，编译器还能做 些优化，否则直接使用一般的变量就可以了干吗还要使用原子变量，不是没事找事儿么，再 说如果你直接像对待一般整型值一样对待它，编译器也会看不过去你的行为，直接给你个 error的。友情提醒一下，kref_init初始化时，是把refcount的值初始化为 1 了的，不是 0。 还有一点要说的是 kref_put参数里的那个函数指针，你不能传递一个NULL过去，否则这 个引用计数就只是计数，而背离了最初的目的，要记住我们需要在这个计数减为 0 的时候 将嵌入这个引用计数struct kref结构体的对象给销毁掉，所以这个函数指针也不能为 kfree，因为这样的话就只是把这个struct kref结构体给销毁了，而不是整个对象。

```
31 void usb_init_urb(struct urb *urb)
32 {
33 if (urb) {
34 memset(urb, 0, sizeof(*urb));
35 kref_init(&urb->kref);
36 spin_lock_init(&urb->lock);
37 }
38 }
39 

81 void usb_free_urb(struct urb *urb)
82 {
83 if (urb)
84 kref_put(&urb->kref, urb_destroy);
85 }
86 

97 struct urb * usb_get_urb(struct urb *urb)
98 {
99 if (urb)
100 kref_get(&urb->kref);
101 return urb;
102 } 
```



usb_init_urb、usb_get_urb、usb_free_urb 这三个函数分别调用了前面看到的 struct kref 结构的三个操作函数来进行引用计数的初始化、加 1、减一。什么叫封装？这就叫封 装。usb_init_urb 和 usb_get_urb 都没什么好说的，比较感兴趣的是 usb_free_urb 里 给 kref_put 传递的那个函数 urb_destroy，它也在 urb.c 里定义

```
21#ifndef offsetof
22#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
23#endif
```



```
25#ifndef container_of
26/**
27 * container_of - cast a member of a structure out to the containing structure
28 * @ptr:	the pointer to the member.
29 * @type:	the type of the container struct this is embedded in.
30 * @member:	the name of the member within the struct.
31 *
32 */
33#define container_of(ptr, type, member) ({			\
34	const typeof(((type *)0)->member) * __mptr = (ptr);	\
35	(type *)((char *)__mptr - offsetof(type, member)); })
36#endif
```

```
9 #define to_urb(d) container_of(d, struct urb, kref)
10
11 static void urb_destroy(struct kref *kref)
12 {
13 struct urb *urb = to_urb(kref);
14 kfree(urb);
15 } 
```

这个 urb_destroy 首先调用了 to_urb，实际上就是一个 container_of 来获得引用计数关 99 联的那个 urb，然后使用 kfree 将它销毁。



1132 行，use_count，这里又是一个使用计数，不过此计数非彼计数，它与上面那个用来 追踪 urb 生命周期的 kref 一点儿血缘关系也没有，连远亲都不是。那它是用来做什么的， 凭什么在臃肿的 struct urb 不断喊着要瘦身的时候还仍有一席之地







##### 完整的 usb 通信

驱动程序发现自 己有与 usb 设备通信的需要，于是创建一个 urb，并指定它的目的地是设备上的哪个端点， 然后提交给 usb core，usb core 将它修修补补的做些美化之后再移交给主机控制器的驱 动程序 HCD，HCD 会去解析这个 urb，了解它的目的是什么，并与 usb 设备进行相应的 交流，在交流结束，urb 的目的达到之后，HCD 再把这个 urb 的所有权移交回驱动程序。

这里的 use_count 就是在 usb core 将 urb 移交给 HCD，办理移交手续的时候，插上了 那么一脚，每当走到这一步，它的值就会加 1。什么时候减 1？在 HCD 重新将 urb 的所有 权移交回驱动程序的时候。这样说吧，只要 HCD 拥有这个 urb 的所有权，那么该 urb 的 use_count 就不会为 0。这么一说，似乎 use_count 也有点追踪 urb 生命周期的味道了， 当它的值大于 0 时，就表示当前有 HCD 正在处理它，和上面的 kref 概念上有部分的重叠， 不过，显然它们之间是有区别的，没区别的话这儿干吗要用两个计数，不是没事儿找抽么。 上面的那个 kref 实现方式是内核里统一的引用计数机制，当计数减为 0 时，urb 对象就被 urb_destroy 给销毁了。这里的 use_count 只是用来统计当前这个 urb 是不是正在被哪 个 HCD 处理，即使它的值为 0，也只是说明没有 HCD 在使用它而已，并不代表就得把它 给销毁掉。比方说，HCD 利用完了 urb，把它还给了驱动，这时驱动还可以对这个 urb 检 修检修，再提交给哪个 HCD 去使用。

下面的问题就是既然它不会平白无故的多出来，那它究竟是用来干啥的？还要从刚提到的那 几个阶段说起。urb 驱动也创建了，提交也提交了，HCD 正处理着那，可驱动反悔了，它 不想再继续这次通信了，想将这个 urb 给终止掉，善解任意的 usb core 当然会给驱动提供 这样的接口来满足这样的需要。不过这个需要还被写代码的哥们儿细分为两种，一种是驱动 只想通过 usb core 告诉 HCD 一声，说这个 urb 我想终止掉，您就别费心再处理了，然后 它不想在那里等着 HCD 的处理，想忙别的事去，这就是俗称的异步，对应的是 usb_unlink_urb 函数。当然对应的还有种同步的，驱动会在那里苦苦等候着 HCD 的处理 结果，等待着 urb 被终止，对应的是 usb_kill_urb 函数。而 HCD 将这次通信终止后，同 样会将 urb 的所有权移交回驱动。那么驱动通过什么判断 HCD 已经终止了这次通信？就是 通过这里的 use_count，驱动会在 usb_kill_urb 里面一直等待着这个值变为 0。



1133 行，reject，拒绝，拒绝什么？不是邀请 ppmm 共进晚餐被拒绝，也不是你要求老 板多给点薪水被拒绝，那又是被谁拒绝？ 在目前版本的内核里，只有 usb_kill_urb 函数有特权对它进行修改，那么，显然 reject 就与上面说的 urb 终止有关了。那就看看 urb.c 里定义的这个函数

```
464 void usb_kill_urb(struct urb *urb)
465 {
466 might_sleep();
467 if (!(urb && urb->dev && urb->dev->bus))
468 return;
469 spin_lock_irq(&urb->lock);
470 ++urb->reject;
471 spin_unlock_irq(&urb->lock);
472
473 usb_hcd_unlink_urb(urb, -ENOENT);
474 wait_event(usb_kill_urb_queue, atomic_read(&urb->use_count) == 0);
475
476 spin_lock_irq(&urb->lock);
477 --urb->reject;
478 spin_unlock_irq(&urb->lock);
479 } 
```

466 行，因为 usb_kill_urb函数要一直等候着HCD将urb终止掉，它必须是可以休眠的， 101 不然就太可恶了，就像那些脚踩多只船的，占着本来就稀有的ppmm资源，让大批男同志 们找不到另一半来关爱。而历史上，当大批男性无法结婚时，他们就会聚到一起，要么成为 和尚，要么结为匪帮，所以说这可是一个严重的社会问题、治安问题。所以说 usb_kill_urb 不能用在中断上下文，必须能够休眠将自己占的资源给让出来。 写代码的哥们儿也都是忧国忧民的主儿，也深刻体会到广大男同胞们的无奈，于是提供了 might_sleep函数，用它来判断一下这个函数是不是处在能够休眠的情况，如果不是，就 会打印出一大堆的堆栈信息，比如你在中断上下文调用了这个函数时。不过，它也就是基于 调试的目的用一用，方便日后找错，并不能强制哪个函数改变自己的上下文。法律也规定了 要一夫一妻制，但也就是用来警示警示，如果以为实际上真能禁止些什么，纯粹就是扯淡

467 行，这里就是判断一下 urb，urb 要去的那个设备，还有那个设备在的总线有没有，如 果哪个不存在，就还是返回吧。

469 行，去获得每个urb都有的那把锁，然后将 reject加 1。加 1 有什么用？其实目前版本 的内核里只有两个地方用到了这个值进行判断。第一个地方是在usb core将urb提交给 HCD，正在办移交手续的时候，如果reject大于 0，就不再接着移交了，也就是说这个urb 被HCD给拒绝了。这是为了防止这边儿正在终止这个urb，那边儿某个地方却又妄想将这个 urb重新提交给HCD。

473 行，这里告诉HCD驱动要终止这个urb了，usb_hcd_unlink_urb函数也只是告诉HCD 一声，然后不管HCD怎么处理就返回了。

474 行，上面的 usb_hcd_unlink_urb是返回了，但并不代表HCD已经将urb给终止了， HCD可能没那么快，所以这里 usb_kill_urb要休息休息，等人通知它。这里使用了 wait_event宏来实现休眠，usb_kill_urb_queue是在hcd.h里定义的一个等待队列，专 门给 usb_kill_urb休息用的。需要注意的是这里的唤醒条件，use_count必须等于 0，终 于看到 use_count实战的地方了。

那在哪里唤醒正在睡大觉的 usb_kill_urb？这牵扯到了第二个使用reject来做判断的地 方。在HCD将urb的所有权返还给驱动的时候，会对reject进行判断，如果reject大于 0， 就调用wake_up唤醒在 usb_kill_urb_queue上休息的 usb_kill_urb。也好理解，HCD 都要将urb的所有权返回给驱动了，那当然就是已经处理完了，放在这里就是已经将这个urb 终止了，usb_kill_urb等的就是这一天的到来，当然就要醒过来继续往下走了。

476 行，再次获得 urb 的那把锁，将 reject 刚才增加的那个 1 给减掉。urb 都已经终止了， 也没人再会去拒绝它了，reject 还是开始什么样儿结束的时候就什么样吧。

```
435 int usb_unlink_urb(struct urb *urb)
436 { 
 102
437 if (!urb)
438 return -EINVAL;
439 if (!(urb->dev && urb->dev->bus))
440 return -ENODEV;
441 return usb_hcd_unlink_urb(urb, -ECONNRESET);
442 } 
```

看 usb_unlink_urb这儿就简单多了，只是把自己的意愿告诉HCD，然后就非常洒脱的返 回了。

struct urb 结构里的前边儿这几个，只是 usb core 和主机控制器驱动需要关心的，实际的 驱动里根本用不着也管不着，它们就是 usb 和 HCD 的后花园，想种点什么不种点什么都由 写这块儿代码的哥们儿决定，他们在里面怎么为所欲为都不关你写驱动的啥事。usb 在 linux 里起起伏伏这么多年，前边儿的这些内容早就变过多少次，说不定你今天还看到谁谁，到接 下来的哪天就看不到了，不过，变化的是形式，不变的是道理。驱**动要做的只是创建一个 urb，然后初始化，再把它提交给 usb core 就可以了，使用不使用引用计数，加不加锁之 类的一点都不用去操心。感谢 David Brownell，感谢 Alan Stern，感谢……，没有他们 就没有 usb 在 linux 里的今天。**



## 设备的生命线（四）

1136 行，urb_list，还记得每个端点都会有的那个 urb 队列么？那个队列就是由这里的 urb_list 一个一个的链接起来的。HCD 每收到一个 urb，就会将它添加到这个 urb 指定的 那个端点的 urb 队列里去。这个链表的头儿在哪儿？当然是在端点里，就是端点里的那个 struct list_head 结构体成员。

1138 行，dev，傻强都知道，它表示的是 urb 要去的那个 usb 设备。

1139 行，pipe，urb 到达端点之前，需要经过一个通往端点的管道，就是这个 pipe。那 第一个问题，怎么表示一个 pipe？人生有两极，管道有两端，一端是主机上的缓冲区，一 103 端是设备上的端点，既然有两端，总要有个方向吧，不然 urb 要待在管道里无所适从的仰 天长叹，我从哪里来，又该往哪里去？而且早先说过，端点有四种类型，那么与端点相生相 依的管道也应该不只一种吧。这么说来，确定一条管道至少要知道两端的地址、方向和类型 了，不过这两端里主机是确定的，需要确定的只是另一端设备的地址和端点的地址。那怎么 将这些内容揉合起来表示成一个管道？一个包含了各种成员属性的结构再加上一些操作函 数？多么完美的封装，但是不需要这么搞，写代码的哥们儿和俺的人生信条差不多，复杂简 单化，一个整型值再加上一些宏就够了。

###### 管道

先看看管道，也就是这个整型值的构成，bit7 用来表示方向，bit8~14 表示设备地址， bit15~18 表示端点号，早先说过，设备地址用 7 位来表示，端点号用 4 位来表示，剩下 来的 bit30~31 表示管道类型。再看看围绕管道的一些宏，在 include/linux/usb.h 里定 义

```
1407 #define PIPE_ISOCHRONOUS 0
1408 #define PIPE_INTERRUPT 1
1409 #define PIPE_CONTROL 2
1410 #define PIPE_BULK 3
1411
1412 #define usb_pipein(pipe) ((pipe) & USB_DIR_IN)
1413 #define usb_pipeout(pipe) (!usb_pipein(pipe))
1414
1415 #define usb_pipedevice(pipe) (((pipe) >> 8) & 0x7f)
1416 #define usb_pipeendpoint(pipe) (((pipe) >> 15) & 0xf)
1417
1418 #define usb_pipetype(pipe) (((pipe) >> 30) & 3) 
 104
1419 #define usb_pipeisoc(pipe) (usb_pipetype((pipe)) == PIPE_ISOCHRONOUS)
1420 #define usb_pipeint(pipe) (usb_pipetype((pipe)) == PIPE_INTERRUPT)
1421 #define usb_pipecontrol(pipe) (usb_pipetype((pipe)) == PIPE_CONTROL)
1422 #define usb_pipebulk(pipe) (usb_pipetype((pipe)) == PIPE_BULK) 
```

现在看第二个问题，如何创建一个管道？主机和设备不是练家子，没练过千里传音什么的绝 世神功，要交流必须通过管道，你必须得创建一个管道给 urb，它才知道路怎么走。不过在 说怎么创建一个管道前，先说个有关管道的故事，咱们也知道知道管道这么热门儿的词汇是 咋来的

###### 创建管道

```
1432 static inline unsigned int __create_pipe(struct usb_device *dev,
1433 unsigned int endpoint)
1434 {
1435 return (dev->devnum << 8) | (endpoint << 15);
1436 }
1437
1438 /* Create various pipes... */
1439 #define usb_sndctrlpipe(dev,endpoint) \
1440 ((PIPE_CONTROL << 30) | __create_pipe(dev,endpoint))
1441 #define usb_rcvctrlpipe(dev,endpoint) \
1442 ((PIPE_CONTROL << 30) | __create_pipe(dev,endpoint) | USB_DIR_IN)
1443 #define usb_sndisocpipe(dev,endpoint) \ 
 105
1444 ((PIPE_ISOCHRONOUS << 30) | __create_pipe(dev,endpoint))
1445 #define usb_rcvisocpipe(dev,endpoint) \
1446 ((PIPE_ISOCHRONOUS << 30) | __create_pipe(dev,endpoint) | USB_DIR_IN)
1447 #define usb_sndbulkpipe(dev,endpoint) \
1448 ((PIPE_BULK << 30) | __create_pipe(dev,endpoint))
1449 #define usb_rcvbulkpipe(dev,endpoint) \
1450 ((PIPE_BULK << 30) | __create_pipe(dev,endpoint) | USB_DIR_IN)
1451 #define usb_sndintpipe(dev,endpoint) \
1452 ((PIPE_INTERRUPT << 30) | __create_pipe(dev,endpoint))
1453 #define usb_rcvintpipe(dev,endpoint) \
1454 ((PIPE_INTERRUPT << 30) | __create_pipe(dev,endpoint) | USB_DIR_IN) 
```

端点是有四种的，对应着管道也就有四种，同时端点是有IN也有OUT的，相应的管道也就 有两个方向，于是二四得八，上面就出现了八个创建管道的宏。有了struct usb_device 结构体，也就是说知道了设备地址，再加上端点号，你就可以需要什么管道就创建什么管道。 __create_pipe宏只是一个幕后的角色，用来将设备地址和端点号放在管道正确的位置上。

1140 行，status，urb 的当前状态。urb 当然是可以有多种状态的，像咱们这种除了三点 一线就是三点一线的小角色都被码上了各种各样的状态，何况 usb 通信的顶梁柱 urb。咱 们的状态是别人了解自己的窗口，同时也是意味着自己要尽的责任，你已经有老婆了就不要 再去追求人家小 mm 了，不然你从未婚状态转到已婚状态只是户口档案里改了一个字？不 过如果人家小 mm 乐意那也就只好祝福你了，人的事情就是这么的复杂，还是咱们的 urb 简单，它当前什么状态就是让咱们了解出了什么事情。至于各种具体的状态代表了什么意思， 碰到了再说

1141 行，transfer_flags，一些标记，可用的值都在 include/linux/usb.h 里有定义

```
939 /*
940 * urb->transfer_flags:
941 */
942 #define URB_SHORT_NOT_OK 0x0001 /* report short reads as errors */
943 #define URB_ISO_ASAP 0x0002 /* iso-only, urb->start_frame
944 * ignored */
945 #define URB_NO_TRANSFER_DMA_MAP 0x0004 /* urb->transfer_dma valid on submit
*/
946 #define URB_NO_SETUP_DMA_MAP 0x0008 /* urb->setup_dma valid on submit */
947 #define URB_NO_FSBR 0x0020 /* UHCI-specific */
948 #define URB_ZERO_PACKET 0x0040 /* Finish bulk OUT with short packet
*/
949 #define URB_NO_INTERRUPT 0x0080 /* HINT: no non-error interrupt
950 
```

URB_SHORT_NOT_OK，这个标记只对用来从 IN 端点读取数据的 urb 有效，意思就是说 106 如果从一个 IN 端点那里读取了一个比较短的数据包，就可以认为是错误的。那么这里的 short 究竟 short 到什么程度？

wMaxPacketSize 与 short 有什么关系？关系还不小， short 不 short 就是与 wMaxPacketSize 相比的，如果从 IN 端点那儿收到了一个比 wMaxPacketSize 要短的包， 同时也设置了 URB_SHORT_NOT_OK 这个标志，那么就可以认为传输出错了。本来如果 收到一个比较短的包是意味着这次传输到此为止就结束了，你想想 data payload 的长度最 大必须为 wMaxPacketSize 这个规定是不可违背的了，但是如果端点想给你的数据不止那 么多，怎么办？就需要分成多个 wMaxPacketSize 大小的 data payload 来传输，事情有 时不会那么凑巧，刚好能平分成多个整份，这时，最后一个 data payload 的长度就会比 wMaxPacketSize 要小，这种情况本来意味着端点已经传完了它想传的，释放完了自己的 需求，这次传输就该结束了，不过如果你设置了 URB_SHORT_NOT_OK 标志，HCD 这 边就会认为错误发生了。

URB_ISO_ASAP，这个标志只是为了方便等时传输用的。等时传输和中断传输在 spec 里 都被认为是 periodic transfers，也就是周期传输，咱们都知道在 usb 的世界里都是主机 占主导地位，设备是没多少发言权的，但是对于等时传输和中断传输，端点可以对主机表达 自己一种美好的期望，希望主机能够隔多长时间访问自己一次，这个期望的时间就是这里说 的周期。当然，期望与现实是有一段距离的，如果期望的都能成为现实，咱们还研究 usb 干吗，中国足球也不用整天喊冲出亚洲了，阎世铎在国青赴德时放的那句 “中国足球缺的 是一块金牌，希望北京奥运会能够实现这个目标！”就够了。端点的这个期望能不能得到满 足，要看主机控制器答应不答应。对于等时传输，一般来说也就一帧（微帧）一次，主机那 儿也很忙，再多也抽不出空儿来。那么如果你有个用于等时传输的 urb，你提交给 HCD 的 时候，就得告诉 HCD 它应该从哪一帧开始的，就要对下面要说的那个 start_frame 赋值， 也就是说告诉 HCD 等时传输开始的那一帧（微帧）的帧号，如果你留心，应该还会记得前 面说过在每帧或微帧（Mircoframe）的开始都会有个 SOF Token 包，这个包里就含有个 帧号字段，记录了那一帧的编号。这样的话，一是比较烦，作为一个男人，烦心的事儿已经 够多了，钞票一天比一天难赚，前方怎么也看不到岸，还要去设置这个 start_frame，你 说烦不烦，二是到你设置的那一帧的时候，如果主机控制器没空开始等时传输，你说怎么办， 107 要知道 usb 的世界里它可是老大。于是，就出现了 URB_ISO_ASAP，它的意思就是告诉 HCD 啥时候不忙就啥时候开始，就不用指定什么开始的帧号了，是不是感觉特轻松？所以 说，你如果想进行等时传输，又不想标新立异的话，就还是把它给设置了吧。

###### DMA

URB_NO_TRANSFER_DMA_MAP，还有 URB_NO_SETUP_DMA_MAP，这两个标志 都是有关 DMA 的，什么是 DMA？就是外设，比如咱们的 usb 摄像头，和内存之间直接进 行数据交换，把 CPU 给撇一边儿了，本来，在咱们的电脑里，CPU 自认为是老大，什么事 都要去插一脚，都要经过它去协调处理，和地球对面的那个美利坚合众国差不多。可是这样 的话就影响了数据传输的速度，就像革命青年上山下乡那会儿，谁对哪个 mm 有意思了， 要先向自己的老爸老妈汇报思想动态，说想和哪家姑娘处对象，等着老爸老妈经过高层协商 说可以交往了，然后才能和人家姑娘见面，这多慢啊，哪像现在，老爸老妈都不知道那，下 一代可能都已经培育出来了，有 DMA 和没有 DMA 区别就是这么大。 usb 的世界里也是要与时俱进，要创建和谐社会的，所以 dma 也是少不了的。一般来说， 都是驱动里提供了 kmalloc 等分配的缓冲区，HCD 做一定的 DMA 映射处理，DMA 映射 是干吗的？外设和内存之间进行数据交换，总要互相认识吧，难不成是在衡山路的酒吧啊， 会有不认识的 mm 过来和你答腔，外设是通过各种总线连到主机里边儿的，使用的是总线 地址，而内存使用的是虚拟地址，它们之间本来就是两条互不相交的平行线，要让它们中间 产生连接点，必须得将一个地址转化为另一个地址，这样才能找得到对方，才能互通有无， 而 DMA 映射就是干这个的。这只是轻描淡写三言两语的粗略说法，实际上即使千言万语也 道不完的。它可是高技术含量的活儿，不是看看报纸喝喝茶，开开会泡泡 mm 就能搞定的， 所以在某些平台上非常的费时费力，为了分担点 HCD 的压力，于是就有了这里的两个标志， 告诉 HCD 不要再自己做 DMA 映射了，驱动提供的 urb 里已经提供有 DMA 缓冲区地址， 为领导分忧解难是咱们这些小百姓应该做的事情。具体提供了哪些 DMA 缓冲区？就涉及到 下面的 transfer_buffer，transfer_dma，还有 setup_packet，setup_dma 这两对儿 了。

URB_NO_FSBR，这是给 UHCI 用的。

URB_ZERO_PACKET，这个标志表示批量的 OUT 传输必须使用一个 short packet 来结 束。批量传输的数据大于批量端点的 wMaxPacketSize 时，需要分成多个 Data 包来传输， 最后一个 data payload 的长度可能等于 wMaxPacketSize，也可能小于，当等于 wMaxPacketSize 时，如果同时设置了 URB_ZERO_PACKET 标志，就需要再发送一个长 度为 0 的数据包来结束这次传输，如果小于 wMaxPacketSize 就没必要多此一举了。你要 问，当批量传输的数据小于 wMaxPacketSize 时那？也没必要再发送 0 长的数据包，因为 此时发送的这个数据包本身就是一个 short packet。

URB_NO_INTERRUPT，这个标志用来告诉 HCD，在 URB 完成后，不要请求一个硬件中 断，当然这就意味着你的结束处理函数可能不会在 urb 完成后立即被调用，而是在之后的 某个时间被调用，咱们的 usb core 会保证为每个 urb 调用一次结束处理函数。

1142~1144 行，transfer_buffer，transfer_dma，transfer_buffer_length，前面说 过管道的一端是主机上的缓冲区，一端是设备上的端点，这三个家伙就是描述主机上的那个 缓冲区的。transfer_buffer 是使用 kmalloc 分配的缓冲区，transfer_dma 是使用 usb_buffer_alloc 分配的 dma 缓冲区，HCD 不会同时使用它们两个，如果你的 urb 自带 了 transfer_dma，就要同时设置 URB_NO_TRANSFER_DMA_MAP 来告诉 HCD 一声， 不用它再费心做 DMA 映射了。transfer_buffer 是必须要设置的，因为不是所有的主机控 制器都能够使用 DMA 的，万一遇到这样的情况，也好有个备用。transfer_buffer_length 指的就是 transfer_buffer 或 transfer_dma 的长度。

1145 行，actual_length，urb 结束之后，会用这个字段告诉你实际上传输了多少数据。

1146~1147 行，setup_packet，setup_dma，同样是两个缓冲区，一个是kmalloc分 配的，一个是用usb_buffer_alloc分配的，不过，这两个缓冲区是控制传输专用的，记得 struct usb_ctrlrequest不？它们保存的就是一个struct usb_ctrlrequest结构体，如果 你的urb设置了setup_dma，同样要设置URB_NO_SETUP_DMA_MAP标志来告诉HCD。 如果进行的是控制传输，setup_packet是必须要设置的，也是为了防止出现主机控制器不 能使用DMA的情况。

1148 行，start_frame，如果你没有指定 URB_ISO_ASAP 标志，就必须自己设置 start_frame，指定等时传输在哪帧或微帧开始。如果指定了 URB_ISO_ASAP，urb 结束 时会使用这个值返回实际的开始帧号。

1150 行，interval，等时和中断传输专用。interval 间隔时间的意思，什么的间隔时间？ 就是上面说的端点希望主机轮询自己的时间间隔。这个值和端点描述符里的 bInterval 是一 样的，你不能随便儿的指定一个，然后就去做春秋大梦，以为到时间了梦里的名车美女都会 跑出来，协议里对你能指定的值是有范围限制的，对于中断传输，全速时，这个范围为 1~255ms，低速是为 10~255ms，高速时为 1~16，这个 1~16 只是 bInterval 可以取 的值，实际的间隔时间需要计算一下，为 2 的（bInterval-1）次方乘以 125 微妙，也就 是 2 的（bInterval-1）次方个微帧。对于等时传输，没有低速了，等时传输根本就不是低 速端点负担得起的，有多大能耐就做多大事，人有多大胆地有多大产的时代早就已经过去了， 对于全速和高速，这个范围也是为 1~16，间隔时间由 2 的（bInterval-1）次方算出来， 单位为帧或微帧。这样看来，每一帧或微帧里，你最多只能期望有一次等时和中断传输，不 能再多了，你只能期望房价涨的慢点，要是希望它跌下去，那要求就太过分了，它可是经济 的柱子，要是倒了，那不是陷国人的生活于困境么，所以咱们要爱国啊，要送钱给 ZF 还有 任小强们啊同志们。

不过即使完全按照上面的范围来取，你的期望也并不是就肯定可以实现的，因为对于高速来 说，最多有 80%的总线时间给这两种传输用，对于低速和全速要多点儿，达到 90%，这 个时间怎么分配，都由主机控制器掌握着，所以你的期望能不能实现还要看主机控制器的脸 色，没办法，它就有这种权力。在咱们这个官本位的历史悠久的民族里，什么最重要？权力， 而且它还无处不在，个中滋味大家自有体会了。

1153 行，context，驱动设置了给下面的结束处理函数用的。比如可以将自己驱动里描述 自己设备的结构体放在里边儿，在结束处理函数里就可以取出来。

1154 行，complete，一个指向结束处理函数的指针，传输成功完成，或者中间发生错误 的时候就会调用它，驱动可以在这里边儿检查 urb 的状态，并做一些处理，比如可以释放 这个 urb，或者重新提交给 HCD。就说摄像头吧，你向 HCD 提交了个等时的 urb 从摄像 头那里读取视频数据，传输完成的时候调用了你指定的这个结束处理函数，并在里面取出了 urb 里面获得的数据进行解码等处理，然后怎么着？总不会这一个 urb 读取的数据就够你 向 mm 表白了吧，你的爱慕之情可是犹如滔滔江水连绵不绝，所以需要获得更多的数据， 那你也总不会再去创建、初始化一个等时的 urb 吧，即使再穷极无聊的人也不会那么做， 明显刚刚的那个可以继续用的，只要将它再次提交给 HCD 就可以了。这个函数指针的定义 在 include/linux/usb.h 

```
961 typedef void (*usb_complete_t)(struct urb *); 
```

还有三个，都是等时传输专用的，等时传输与其它传输不一样，可以指定传输多少个packet， 每 个 packet 使 用 struct usb_iso_packet_descriptor 结构来描述。 1155 行 的 iso_frame_desc就表示了一个变长的struct usb_iso_packet_descriptor结构体数组， 而 1149 行的number_of_packets指定了要这个结构体数组的大小，也就是要传输多少个 packet。

要说明的是这里说的packet不是说你在一次等时传输里传输了多个Data packet，而是说 你在一个urb里指定了多次的等时传输，每个struct usb_iso_packet_descriptor结构体 都代表了一次等时传输。这里说一下等时传输底层的packet情况。不像控制传输最少要有 SETUP和STATUS两个阶段的transaction，等时传输只有Isochronous transaction，即 等时transaction一个阶段，一次等时传输就是一次等时transaction的过程。而等时 transaction也只有两个阶段，就是主机向设备发送OUT Token包，然后发送一个Data包， 或者是主机向设备发送IN Token包，然后设备向主机发送一个Data包，这个Data包里data payload 的长度只能小于或者等于等时端点的 wMaxPacketSize 值。这里没有了 Handshake包，因为不需要，等时传输是不保证数据完全正确无误的到达的，没有什么错 误重传机制，也就不需要使用Handshake包来汇报OK不OK。对它来说实时要比正确性重 要的多，你的摄像头一秒钟少给你一帧多给你一帧，没什么本质的区别，如果给你延迟个几 秒，就明显的感觉不爽了。

所以对于等时传输来说，在完成了 number_of_packets 次传输之后，会去调用你的结束 处理函数，在里面对数据做处理，而 1152 行的 error_count 记录了这么多次传输中发生 错误的次数。

现在看看struct usb_iso_packet_descriptor结构的定义，在include/linux/usb.h里定 义

```
952 struct usb_iso_packet_descriptor { 
 110
953 unsigned int offset;
954 unsigned int length; /* expected length */
955 unsigned int actual_length;
956 int status;
957 }; 
```

offset表示transfer_buffer里的偏移位置，你不是指定了要进行number_of_packets次 等时传输么，那么也要准备够这么多次传输用的缓冲区吧，当然不是说让你准备多个缓冲区， 没必要，都放transfer_buffer或者transfer_dma里面好了，只要记着每次传输对应的数 据偏移就可以。length是预期的这次等时传输Data包里数据的长度，注意这里说的是预期， 因为实际传输时因为种种原因可能不会有那么多数据，urb结束时，每个struct usb_iso_packet_descriptor结构体的actual_length就表示了各次等时传输实际传输的 数据长度，而 status分别记录了它们的状态。



## 设备的生命线（五）

##### usb_alloc_urb

第一个基本点：usb_alloc_urb函数，创建一个urb，struct urb结构体只能使用它来创建， 它是urb在usb世界里的独家代理，和天盛一样的角色。

第一个基本点，usb_alloc_urb函数，创建urb的专用函数，为一个urb申请内存并做初始 化，在urb.c里定义。

```
56 struct urb *usb_alloc_urb(int iso_packets, gfp_t mem_flags)
57 {
58 struct urb *urb;
59
60 urb = kmalloc(sizeof(struct urb) +
61 iso_packets * sizeof(struct usb_iso_packet_descriptor),
62 mem_flags);
63 if (!urb) {
64 err("alloc_urb: kmalloc failed");
65 return NULL;
66 }
67 usb_init_urb(urb);
68 return urb;
69 } 
```

这函数长的很让人提神，是个亲民的角色。它只做了两件事情，拿kmalloc来为urb申请内 存，然后调用 usb_init_urb进行初始化。usb_init_urb函数在前面说struct urb中的引用 计数的时候已经贴过了，它主要的作用就是初始化urb的引用计数，还用memset顺便把这 里给urb申请的内存清零。

没什么说的了么？usb_alloc_urb说：别看我简单，我也是很有内涵的。先看第一个问题， 它的第一个参数 iso_packets ，表示的是 struct urb 结构最后那个变长数组 iso_frame_desc的元素数目，也就是应该与number_of_packets的值相同，所以对于控 制/中断/批量传输，这个参数都应该为 0。这也算是给咱们示范了下变长数组咋个用法，内 核里到处都是C的示范工程。

第二个问题是参数mem_flags的类型 gfp_t，早几个版本的内核，这里还是int，当然这里 变成 gfp_t是因为 kmalloc参数里的那个标志参数的类型从int变成 gfp_t了，你要用 kmalloc来申请内存，就得遵守它的规则。不过这里要说的不是kmalloc，而是gfp_t，它 在江湖上也没出现多久，名号还没打出来，很多人不了解，咱们来调查一下它的背景。它在 include/linux/types.h里定义

```
193 typedef unsigned __bitwise__ gfp_t; 
```

很显然，要了解 gfp_t，关键是要了解__bitwise__，它也在 types.h 里定义

```
170 #ifdef __CHECKER__
171 #define __bitwise__ __attribute__((bitwise))
172 #else
173 #define __bitwise__
174 #endif 
```

__bitwise__ 的含义又取决于是否定义了 __CHECKER__ ，如果没有定义 __CHECKER__，那__bitwise__就啥也不是。哪里定义了__CHECKER__？穿别人的鞋， 走自己的路，让他们去找吧，咱们不找，因为内核代码里就没有哪个地方定义了 __CHECKER__，它是有关 Sparse 工具的，内核编译时的参数决定了是不是使用 Sparse 工具来做类型检查。那 Sparse 又是什么？它是一种静态分析工具(static analysis tool), 用于在 linux 内核源代码中发现各种类型的漏洞，一直都是比较神秘的角色，最初由 Linus Torvalds 写的，后来 linus 没有继续维护，直到去年的冬天，它才又有了新的维护者 Josh Triplett。有关 Sparse 再多的东东，咱们还是各自去研究吧，这里不多说了。

可能还会有第三个问题，usb_alloc_urb也没做多少事啊，它做的那些咱们自己很容易就能 做了，为什么还说驱动里一定要使用它来创建urb那？按照C++的说法，它就是urb的构造 函数，构造函数是创建对象的唯一方式，你抬杠说C++里面儿使用位拷贝去复制一个简单 对象给新对象就没使用构造函数，那是你不知道，C++的ARM里将这时的构造函数称为 trivial copy constructor。再说，现在它做这些事儿，以后还是做这些么？它将创建urb 的工作给包装了，咱们只管调用就是了，孙子兵法里有，以不变应万变

对应的，当然还会有个析构函数，销毁 urb 的，也在 urb.c 里定义

```
71 /**
72 * usb_free_urb - frees the memory used by a urb when all users of it are finished
73 * @urb: pointer to the urb to free, may be NULL
74 *
75 * Must be called when a user of a urb is finished with it. When the last user
76 * of the urb calls this function, the memory of the urb is freed.
77 *
78 * Note: The transfer buffer associated with the urb is not freed, that must be
79 * done elsewhere.
80 */
81 void usb_free_urb(struct urb *urb)
82 {
83 if (urb)
84 kref_put(&urb->kref, urb_destroy);
85 } 
```

usb_free_urb更潇洒，只调用 kref_put将urb的引用计数减一，减了之后如果变为 0，也 就是没人再用它了，就调用 urb_destroy将它销毁掉。

##### usb_fill_control_urb

第二个基本点：usb_fill_control_urb函数，初始化一个控制urb，urb被创建之后，使用 之前必须要正确的初始化。

```
1175 static inline void usb_fill_control_urb (struct urb *urb,
1176 struct usb_device *dev,
1177 unsigned int pipe,
1178 unsigned char *setup_packet,
1179 void *transfer_buffer,
1180 int buffer_length,
1181 usb_complete_t complete_fn,
1182 void *context)
1183 {
1184 spin_lock_init(&urb->lock);
1185 urb->dev = dev;
1186 urb->pipe = pipe;
1187 urb->setup_packet = setup_packet;
1188 urb->transfer_buffer = transfer_buffer;
1189 urb->transfer_buffer_length = buffer_length;
1190 urb->complete = complete_fn;
1191 urb->context = context;
1192 } 
```

这个函数长的就让人兴奋，纯粹是来增长咱们自信的，自信多一分，成功就多十分，你就能 搞懂内核，你就能成为一个成功的男人。这个函数基本上都是赋值语句，把你在参数里指定 的值充实给刚刚创建的 urb，urb 的元素有很多，这里只是填充了一部分，剩下那些不是控 制传输管不着的，就是自有安排可以不用去管的。

你想想，一个struct urb结构要应付四种传输类型，每种传输类型总会有点自己特别的要求，总会有些元素专属于某种传输类型，而其它传输类型不用管的。如果按C++的做法，这称 不上是一个好的设计思想，应该有个基类urb，里面放点儿四种传输类型公用的，比如pipe， transfer_buffer等，再搞几个子类，control_urb，bulk_urb等等，专门应付具体的传输 类型，如果不用什么虚函数，实际的时间空间消耗也不会增加什么。但是实在没必要这么搞， 这年头儿内核的结构已经够多了，你创建什么类型的urb，就填充相关的一些字段好了，况 且写usb core的哥们儿已经给咱们提供了不同传输类型的初始化函数，就像上面的 usb_fill_control_urb ，对于批量传输有 usb_fill_bulk_urb ，对于中断传输有 usb_fill_int_urb，一般来说这也就够了，下面就看看 usb_fill_control_urb函数的这俩 孪生兄弟。

```
1207 static inline void usb_fill_bulk_urb (struct urb *urb,
1208 struct usb_device *dev,
1209 unsigned int pipe,
1210 void *transfer_buffer,
1211 int buffer_length,
1212 usb_complete_t complete_fn,
1213 void *context)
1214 {
1215 spin_lock_init(&urb->lock);
1216 urb->dev = dev;
1217 urb->pipe = pipe;
1218 urb->transfer_buffer = transfer_buffer;
1219 urb->transfer_buffer_length = buffer_length;
1220 urb->complete = complete_fn;
1221 urb->context = context;
1222 } 

1242 static inline void usb_fill_int_urb (struct urb *urb,
1243 struct usb_device *dev,
1244 unsigned int pipe,
1245 void *transfer_buffer,
1246 int buffer_length,
1247 usb_complete_t complete_fn,
1248 void *context,
1249 int interval)
1250 {
1251 spin_lock_init(&urb->lock);
1252 urb->dev = dev;
1253 urb->pipe = pipe;
1254 urb->transfer_buffer = transfer_buffer;
1255 urb->transfer_buffer_length = buffer_length;
1256 urb->complete = complete_fn;
1257 urb->context = context;
1258 if (dev->speed == USB_SPEED_HIGH)
1259 urb->interval = 1 << (interval - 1);
1260 else
1261 urb->interval = interval;
1262 urb->start_frame = -1;
1263 } 
```

负责批量传输的usb_fill_bulk_urb和负责控制传输的 usb_fill_control_urb的相比，只是 少初始化了一个 setup_packet，因为批量传输里没有Setup包的概念，中断传输里也没有， 所以usb_fill_int_urb里也没有初始化 setup_packet这一说。不过usb_fill_int_urb比那 两个还是多了点儿内容的，因为它有个 interval，比控制传输和批量传输多了个表达自己期 望的权利，1258 行还做了次判断，如果是高速就怎么怎么着，否则又怎么怎么着，主要是 高速和低速/全速的间隔时间单位不一样，低速/全速的单位为帧，高速的单位为微帧，还要 经过 2 的（bInterval-1）次方这么算一下。至于 1262 行start_frame，它是给等时传输 116 用的，这里自然就设置为-1，关于为什么既然start_frame是等时传输用的这里还要设置那 么一下，你往后看吧，现在我也不知道。

作为一个共产主义接班人，我们很快就能发现 usb_fill_control_urb的孪生兄弟里，少了 等时传输对应的那个初始化函数，三缺一啊，在哪里都会是个遗憾。不是不想有，而是没办 法，对于等时传输，urb里是可以指定进行多次传输的，你必须一个一个的对那个变长数组 iso_frame_desc里的内容进行初始化，没人帮得了你。难道你能想出一个办法搞出一个适 用于各种情况等时传输的初始化函数？我是不能。如果想不出来，使用urb进行等时传输的 时候，还是老老实实的对里面相关的字段一个一个的填内容吧。如果想找个例子旁观一下别 人是咋初始化的，可以去找个摄像头驱动，或者其它usb音视频设备的驱动看看，内核里也 有一些的。

现在，你应该还记得咱们是因为要设置设备的地址，让设备进入Address状态，调用了 usb_control_msg，才走到这里遇到 usb_fill_control_urb的，参数里的 setup_packet 就是之前创建和赋好值的struct usb_ctrlrequest结构体，设备的地址已经在struct usb_ctrlrequest结构体wValue字段里了，这次控制传输并没有DATA transaction阶段， 也并不需要urb提供什么transfer_buffer缓冲区，所以transfer_buffer应该传递一个 NULL，当然transfer_buffer_length也就为 0 了，有意思的是这时候传递进来的结束处 理函数usb_api_blocking_completion，可以看一下当这次控制传输已经完成，设备地址 已经设置好后，接着做了些什么，它的定义在message.c里

```
21 static void usb_api_blocking_completion(struct urb *urb)
22 {
23 complete((struct completion *)urb->context);
24 } 
```

这个函数更简洁，就那么一句，没有前面说的释放 urb，也没有重新提交它，本来就想设置 个地址就完事儿了，没必要再将它提交给 HCD，你就是再提交多少次，设置多少次，也只 能有一个地址，usb 的世界里不提倡囤积居奇，不鼓励一人多个 Address 去炒。那在这仅 仅一句里面都做了些什么？你接着往下看。

然后就是第三个基本点，usb_start_wait_urb函数，将前面历经千辛万苦创建和初始化的 urb提交给咱们的usb core，让它移交给特定的主机控制器驱动进行处理，然后望眼欲穿的 等待HCD回馈的结果，如果等待的时间超过了预期的限度，它不会再等，不会去变成望夫 石。它在message.c里定义

```
33 static int usb_start_wait_urb(struct urb *urb, int timeout, int *actual_length) 
 117
34 {
35 struct completion done;
35 行，定义了一个struct completion结构体。completion是内核里一个比较简单的同步
机制，一个线程可以通过它来通知另外一个线程某件事情已经做完了。你使用某个下载软件
去下载A片，然后撇一边儿不管就去忙着聊QQ泡mm了，下载完了那个软件会通知你，然
后你想怎么做就怎么做，自个看也成，不怕被扁和正在聊的mm一块看也成，没人会去管你。
怎么？你的那个软件下载完了也没通知你？那就紧赶的换个别的吧，写那个软件的也太没职
业道德了，该做的事情不做。completion机制也同样是这么回事儿，你的代码执行到某个
地方，需要再忙点儿其它的，就新开个线程，让它去忙活，然后自个接着忙自己的，想知道
那边儿忙活的结果了，就停在某个地方等着，那边儿忙活完了会通知一下已经有结果了，于
是你的代码又可以继续往下走。

completion机制围绕struct completion结构去实现，有两种使用方式，一种是通过
DECLARE_COMPLETION宏在编译时就创建好struct completion的结构体，另外一种就
是上面的形式，运行时才创建的，先在 35 行定义一个struct completion结构体，然后在
39 行使用 init_completion去初始化。光是创建struct completion的结构体没用，关键
的是如何通知任务已经完成了，和怎么去等候完成的好消息。片子下载完了可能会用声音、
对话框等多种方式来通知你，同样这里用来通知已经完成了的函数也不只一个，
void complete(struct completion *c);
 void complete_all(struct completion *c); 
 complete 只通知一个等候的线程，complete_all 可以通知所有等候的线程，大家都一个
宿舍的好兄弟，你总不好意思自己藏着好东西，不让大家欣赏吧，所以可能会有多个人来等
着片子下完。

你不可能会毫无限度的等下去，21 世纪最缺的是什么？耐心，凡事都有个限度，即使片子
再精彩，多会儿下不完也不等它了，当然会有比我还穷极无聊的人愿意一直在那里等着，毕
竟林子大了什么鸟儿都有，或者说正等着那，一个 ppmm 过来打断你，你赶着花前月下去
了，也不会去继续等了。所以针对不同的情况，等候的方式就有好几种，都在 kernel/sched.c
里定义
 void wait_for_completion(struct completion *c);
 unsigned long wait_for_completion_timeout(struct completion *x, unsigned
long timeout);
 int wait_for_completion_interruptible(struct completion *x);
 unsigned long wait_for_completion_interruptible_timeout(struct completion
*x, unsigned long timeout); 


36 unsigned long expire;
37 int status;
38
39 init_completion(&done);
40 urb->context = &done;
41 urb->actual_length = 0;
42 status = usb_submit_urb(urb, GFP_NOIO);
usb_submit_urb函数将这个控
制urb提交给usb core，它是异步的，也就是说提交了之后不会等到传输完成了才返回。
43 if (unlikely(status))
44 goto out;
45
46 expire = timeout ? msecs_to_jiffies(timeout) : MAX_SCHEDULE_TIMEOUT;
47 if (!wait_for_completion_timeout(&done, expire)) {
48
49 dev_dbg(&urb->dev->dev,
50 "%s timed out on ep%d%s len=%d/%d\n",
51 current->comm,
52 usb_pipeendpoint(urb->pipe),
53 usb_pipein(urb->pipe) ? "in" : "out",
54 urb->actual_length,
55 urb->transfer_buffer_length);
56
57 usb_kill_urb(urb);
58 status = urb->status == -ENOENT ? -ETIMEDOUT : urb->status;
59 } else
60 status = urb->status;
61 out:
62 if (actual_length)
63 *actual_length = urb->actual_length;
64
65 usb_free_urb(urb);
66 return status;
67 }
```



##### usb_start_wait_urb

第三个基本点：usb_start_wait_urb函数，将urb提交给咱们的usb core，以便分配给特 定的主机控制器驱动进行处理，然后默默的等待处理结果，或者超时。

现在就比较清晰了，**usb_start_wait_urb函数将urb提交给usb core去处理后，就停在 47 行等候usb core和HCD的处理结果，而这个urb代表的控制传输完成之后会调用结束处 理函数 usb_api_blocking_completion ，从而调用 complete 来通知 usb_start_wait_urb 说不用再等了，传输已经完成了，当然还有种可能是 usb_start_wait_urb已经等的超过了时间限度仍然没有接到通知，不管是哪种情况， 119 usb_start_wait_urb都可以不用再等，继续往下走了。**

46 行，计算超时值。超时值在参数里不是已经给了么，还计算什么？没错，你是在参数里 是指定了自己能够忍受的最大时间限度，不过那是以 ms 为单位的，作为一个平头小百姓， 咱们的时间概念里也只有分钟啊妙啊毫秒啊什么的，不过作为一个要在 linux 里混的平头小 百姓，咱们的时间概念里必须得加上一个号称 jiffy 的东东，因为函数 wait_for_completion_timeout 里的超时参数是必须以 jiffy 为单位的。 jiffy，金山词霸告诉我们，是瞬间，短暂的时间跨度，短暂到什么程度？linux 里它表示的 是两次时钟中断的间隔，时钟中断是由定时器周期性产生的，关于这个周期，内核里有个巨 直白巨形象的变量来描述，就是 HZ，它是个体系结构相关的常数。内核里还提供了专门的 计数器来记录从系统引导开始所度过的 jiffy 值，每次时钟中断发生时，这个计数器就增加 1。 既然你指定的时间和 wait_for_completion_timeout 需要的时间单位不一致，就需要转 换一下，msecs_to_jiffies 函数可以完成这个工作，它将 ms 值转化为相应的 jiffy 值。这 一行里还剩个 MAX_SCHEDULE_TIMEOUT 比较陌生，在 include/linux/sched.h 里它 被定义为 LONG_MAX，最大的长整型值，我知道你会好奇这个 LONG_MAX 是怎么来的， 好奇就说出来嘛，好奇又不会真的害死猫，我也很好奇，所以咱们到生它养它的 include/linux/kernel.h 里看看

```
23 #define INT_MAX ((int)(~0U>>1))
24 #define INT_MIN (-INT_MAX - 1)
25 #define UINT_MAX (~0U)
26 #define LONG_MAX ((long)(~0UL>>1))
27 #define LONG_MIN (-LONG_MAX - 1)
28 #define ULONG_MAX (~0UL)
29 #define LLONG_MAX ((long long)(~0ULL>>1))
30 #define LLONG_MIN (-LLONG_MAX - 1)
31 #define ULLONG_MAX (~0ULL) 
```

各种整型数的最大值最小值都在这里了，俺现在替谭浩强再普及点基础知识，‘~’是按位 取反，‘UL’是无符号长整型，那么‘ULL’就是 64 位的无符号长整型，‘<<’左移运算 的话就是直接一股脑的把所有位往左边儿移若干位，‘>>’右移运算比较容易搞混，主要 是牵涉到怎么去补缺，有关空缺儿的事情在哪里都会比较的复杂，勾心斗角阶级斗争的根源， 在 C 里主要就是无符号整数和有符号整数的之间的冲突，在你补管三七二十一一直往右移 多少位之后，空出来的那些空缺，对于无符号整数得补 0，对于有符号的，得补上符号位。 120 还是拿 LONG_MAX 来说事儿，上边定义为((long)(~0UL>>1))，0UL 按位取反之后全 为 1 的无符号长整型，向右移 1 位，左边儿空出来的那个补 0，这个数对于无符号的 long 来说不算什么，但是再经过 long 这么强制转化一下变为有符号的长整型，它就是老大了。 每个老大的成长过程都是一部血泪史，都要历经很多曲折。 现在你可以很明白的知道写代码的哥们儿在 46 行都做了些什么，你指定的超时时间被转化 为相应的 jiffy 值，或者直接被设定为最大的 long 值。

47 行，等待通知，我们需要知道的是怎么去判断等待的结果，也就是 wait_for_completion_timeout 的返回值代表什么意思？一般来说，一个函数返回了 0 代表了好消息，一切顺利，如果你这么想那可就错了。wait_for_completion_timeout 返回 0 恰恰表示的是坏消息，表示直到超过了自己的忍耐的极限仍没有接到任何的回馈， 而返回了一个大于 0 的值则表示接到通知了，那边儿不管是完成了还是出错了总归是告诉 这边儿不用再等了，这个值具体的含义就是距你设定的时限提前了多少时间。为什么会这 样？你去看看 wait_for_completion_timeout 的定义就知道了，我就不贴了，它是通过 schedule_timeout 来实现超时的，schedule_timeout 的返回值就是这么个意思。

那么现在就很明显了，如果超时了，就打印一些调试信息提醒一下，然后调用 usb_kill_urb 终止这个 urb，再将返回值设定一下。如果收到了通知，就简单了，直接设定了返回值，就 接着往下走

62 行，actual_length 是用来记录实际传输的数据长度的，是上头儿的上头儿 usb_control_msg 需要的。不要给我说这个值 urb 里本来就有保存，何必再多次一举找 个地儿去放，没看接下来的 65 行就用 usb_free_urb 把这个 urb 给销毁了啊，到那时花 非花树非树，urb 也已经不是 urb，哪里还去找这个值。actual_length 是从上头儿那里传 递过来的一个指针，写内核的哥们儿教导我们，遇到指针一定要小心再小心啊，同志们。所 以这里要先判断一下 actual_length 是不是空的。 现在，只剩一个 usb_submit_urb 在刚才被有意无意的飘过了，咱们下面说。



## 设备的生命线（六）

### usb_submit_urb

```
220 int usb_submit_urb(struct urb *urb, gfp_t mem_flags)
221 {
222 int pipe, temp, max;
223 struct usb_device *dev;
224 int is_out;
225
226 if (!urb || urb->hcpriv || !urb->complete)
227 return -EINVAL;

226 行，一些有关存在性的判断，某主义的哲学告诉我们：存在是检验真理的唯一标准。
所以这个函数的开头儿就要履行一下常规的检验，urb 为空，都没有初始化是不可以提交给
core 的，core 很生气，后果很严重，hcpriv 本来说好了留给 HCD 用的，你得保证送过去
的时候它还是贞洁的，自己不能偷偷先用了，HCD 很生气，后果也会很严重，complete，
每个 urb 结束了都必须的调用一次 complete 代表的函数，这是真理，你必须得让它存在。



228 if (!(dev = urb->dev) || 
229 (dev->state < USB_STATE_DEFAULT) ||
230 (!dev->bus) || (dev->devnum <= 0))
231 return -ENODEV;
这里是对 urb 的目的地 usb 设备的检验。设备所属
于的那条总线不存在,或者设备本身不存在,你 urb 还嚷嚷着要过去要过去,也太苏维埃
了吧?或者设备甚至还没进入 USB_STATE_DEFAULT 状态,管道的另一端还都是堵着的
怎么过去,早先强调多少回了,要想让设备回应你,它起码得达到 Default 状态。设备编
号 devnum 肯定是不能为负的了,那为 0 为什么也不行那?到现在,地球人都知道了,
Token 包的地址域里有 7 位是表示设备地址的,也就是说总共可以有 128 个地址来分配给
设备,但是其中 0 号地址是被保留作为缺省地址用的,任何一个设备处于 Default 状态还
没有进入 Address 状态的时候都需要通过这个缺省地址来响应主机的请求,所以 0 号地址
不能分配给任何一个设备,hub 为设备选择一个地址的时候,只有选择到一个大于 0 的地
址,设备的生命线才会继续,才会走到这里,因此说这里的 devnum 是不可能也不应该为
0 的,如果为 0 的话,那就是中间哪里谁暗地里动了手脚,就没必要往下走了。
因为咱们看到这里是因为要设置设备的地址,让设备进入 Address 状态,所以针对
SET_ADDRESS 请求再看看这个 devnum。主机向设备发送 SET_ADDRESS 请求时,如
果设备处于 Default 状态,就是它现在处的状态,指定一个非 0 值时,设备将进入 Adress
状态,指定 0 值时,设备仍然会处于 Default 状态,所以说即使从这个角度看,这里的
devnum 也是不能为 0 的,不然就是吃饱饭没事干故意找抽。如果设备已经处于 Adress
状态,指定一个非 0 值时,设备仍然会处于 Address 状态,只是将使用新分配的地址,一
个设备只能占用一个地址,是包分配的,真正的居者有其屋,如果指定了一个 0 值,则设
备将离开 Address 状态退回到 Default 状态
232 if (dev->bus->controller->power.power_state.event != PM_EVENT_ON
233 || dev->state == USB_STATE_SUSPENDED)
234 return -EHOSTUNREACH;
235232 行,power,power_state,event,还有 PM_EVENT_ON 都是电源管理核心里的
东西,这里的目的是判断设备所在的那条总线的主机控制器有没有挂起,然后再判断设备本
身是不是处于 Suspended 状态,如果挂起了都不欢迎你,还死皮赖脸去个什么劲儿,回去
得了。
236 urb->status = -EINPROGRESS;
236 行,常规检查都做完了,core 和 HCD 已经认同了这个 urb,就将它的状态设置为
-EINPROGRESS,表示从现在开始 urb 的控制权就在 core 和 HCD 手里边儿了,驱动那
里是看不到这个状态的。
237 urb->actual_length = 0;
这时还没开始传输,实际传输的数据长度当然为 0 了,这里初始化这么一下,也
是为了防止以后哪里出错返回了,驱动里好检查。
238
239 /* Lots of sanity checks, so HCDs can rely on clean data
240 * and don't need to duplicate tests
241 */
242 pipe = urb->pipe;
243 temp = usb_pipetype(pipe);
244 is_out = usb_pipeout(pipe);
245这几行获得管道的类型还有方向。
246 if (!usb_pipecontrol(pipe) && dev->state < USB_STATE_CONFIGURED)
247 return -ENODEV;
248在设备进入 Configured 状态之前,主机只能使用控制传输,通过缺省管道,也
就是管道 0 来和设备进行交流

249 /* FIXME there should be a sharable lock protecting us against
250 * config/altsetting changes and disconnects, kicking in here.
251 * (here == before maxpacket, and eventually endpoint type,
252 * checks get made.)
253 */
254
255 max = usb_maxpacket(dev, pipe, is_out);//获得端点的 wMaxPacketSize
256 if (max <= 0) {
257 dev_dbg(&dev->dev,
258 "bogus endpoint ep%d%s in %s (bad maxpacket %d)\n",
259 usb_pipeendpoint(pipe), is_out ? "out" : "in",
260 __FUNCTION__, max);
261 return -EMSGSIZE;
262 }
263
264 /* periodic transfers limit size per frame/uframe,
265 * but drivers only control those sizes for ISO.
266 * while we're checking, initialize return status.
267 */
268 if (temp == PIPE_ISOCHRONOUS) {
269 int n, len; 
270
271 /* "high bandwidth" mode, 1-3 packets/uframe? */
272 if (dev->speed == USB_SPEED_HIGH) { 
273 int mult = 1 + ((max >> 11) & 0x03);
274 max &= 0x07ff;
275 max *= mult;
276 }
277如果是等时传输要做一些特别的处理。272 到 276 这几行涉及到高速、高带宽端
点(high speed,high bandwidth endpoint)
。前面提到 interval 的时候,说过每一帧
或微帧最多只能有一次等时传输,完成一次等时 transaction,那时这么说主要是因为还没
遇到高速高带宽的等时端点。高速高带宽等时端点每个微帧可以进行 2 到 3 次等时
transaction,它和一般等时端点的主要区别也就在这儿,没必要专门为它搞个描述符类型,
端点描述符 wMaxPacketSize 字段的 bit 11~12 就是用来指定可以额外有几次等时
transaction 的,00 表示没有额外的 transaction,01 表示额外有 1 次,10 表示额外有
两次,11 被保留。wMaxPacketSize 字段的前 10 位就是实际的端点每次能够处理的最大
字节数。所以这几行意思就是如果是高速等时端点,获得它允许的额外的等时 transaction
次数,和每次能够处理的最大字节数,再将它们相乘就得出了该等时端点每个微帧的所能传
输的最大字节数。
278 if (urb->number_of_packets <= 0)
279 return -EINVAL;
number_of_packets 不大于 0 就表示这个等时 urb 没有指定任何一次等时传输,
这就怪哉了,咋混过来的,可以直接返回了。

280 for (n = 0; n < urb->number_of_packets; n++) {
281 len = urb->iso_frame_desc[n].length;
282 if (len < 0 || len > max)
283 return -EMSGSIZE;
284 urb->iso_frame_desc[n].status = -EXDEV;
285 urb->iso_frame_desc[n].actual_length = 0;
286 }
行对等时 urb 里指定的各次等时传输分别做处理。如果它们预期传输的数据长
度比上面算出来的 max 还要大,对不起,要求太过分了,返回吧。然后将它们实际传输的
数据长度先置为 0,状态都先初始化为-EXDEV,表示这次等时传输仅仅部分完成了,因为
走到这里时传输都还没开始那。
287 }
288
289 /* the I/O buffer must be mapped/unmapped, except when length=0 */
290 if (urb->transfer_buffer_length < 0)
291 return -EMSGSIZE;
292
293 #ifdef DEBUG
294 /* stuff that drivers shouldn't do, but which shouldn't
295 * cause problems in HCDs if they get it wrong.
296 */
297 {
298 unsigned int orig_flags = urb->transfer_flags;
299 unsigned int allowed;
300
301 /* enforce simple/standard policy */
302 allowed = (URB_NO_TRANSFER_DMA_MAP | URB_NO_SETUP_DMA_MAP |
303 URB_NO_INTERRUPT);
304 switch (temp) {
305 case PIPE_BULK:
306 if (is_out)
307 allowed |= URB_ZERO_PACKET;
308 /* FALLTHROUGH */
309 case PIPE_CONTROL:
310 allowed |= URB_NO_FSBR; /* only affects UHCI */
311 /* FALLTHROUGH */
312 default: /* all non-iso endpoints */
313 if (!is_out)
314 allowed |= URB_SHORT_NOT_OK;
315 break;
316 case PIPE_ISOCHRONOUS: 
317 allowed |= URB_ISO_ASAP;
318 break;
319 }
320 urb->transfer_flags &= allowed;
321
322 /* fail if submitter gave bogus flags */
323 if (urb->transfer_flags != orig_flags) {
324 err("BOGUS urb flags, %x --> %x",
325 orig_flags, urb->transfer_flags);
326 return -EINVAL;
327 }
328 }
329 #endif
330 /*
331 * Force periodic transfer intervals to be legal values that are
332 * a power of two (so HCDs don't need to).
333 *
334 * FIXME want bus->{intr,iso}_sched_horizon values here. Each HC
335 * supports different values... this uses EHCI/UHCI defaults (and
336 * EHCI can use smaller non-default values).
337 */
338 switch (temp) {
339 case PIPE_ISOCHRONOUS:
340 case PIPE_INTERRUPT:
341 /* too small? */
342 if (urb->interval <= 0)
343 return -EINVAL;
344 /* too big? */
345 switch (dev->speed) {
346 case USB_SPEED_HIGH: /* units are microframes */
347 // NOTE usb handles 2^15
348 if (urb->interval > (1024 * 8))
349 urb->interval = 1024 * 8;
350 temp = 1024 * 8;
351 break;
352 case USB_SPEED_FULL: /* units are frames/msec */
353 case USB_SPEED_LOW:
354 if (temp == PIPE_INTERRUPT) {
355 if (urb->interval > 255)
356 return -EINVAL;
357 // NOTE ohci only handles up to 32
358 temp = 128;
359 } else {
360 if (urb->interval > 1024) 
361 urb->interval = 1024;
362 // NOTE usb and ohci handle up to 2^15
363 temp = 1024;
364 }
365 break;
366 default:
367 return -EINVAL;
368 }
369 /* power of two? */
370 while (temp > urb->interval)
371 temp >>= 1;
372 urb->interval = temp;
373 }
374
375 return usb_hcd_submit_urb(urb, mem_flags);将 urb 扔给 HCD,然后就进入 HCD 的片儿区了。
376 } 
```

看到这个函数之后我深刻的体会到，世界上只有一个地方有真乐：乐土。不过这个函数长归 长，目标却很简单，就是对 urb 做些前期处理后扔给 HCD。

#### int usb_bulk_msg



207 int usb_bulk_msg(struct usb_device *usb_dev, unsigned int pipe,
208
void *data, int len, int *actual_length, int timeout)
209 {
210
struct urb *urb;
211
struct usb_host_endpoint *ep;
212
213
ep = (usb_pipein(pipe) ? usb_dev->ep_in : usb_dev->ep_out)
214
[usb_pipeendpoint(pipe)];
215
if (!ep || len < 0)
216
return -EINVAL;
217
218
urb = usb_alloc_urb(0, GFP_KERNEL);
219
if (!urb)
220
return -ENOMEM;
221
222
if ((ep->desc.bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
223
USB_ENDPOINT_XFER_INT) {
224
pipe = (pipe & ~(3 << 30)) | (PIPE_INTERRUPT << 30);
225
usb_fill_int_urb(urb, usb_dev, pipe, data, len,
226
usb_api_blocking_completion, NULL,
227
ep->desc.bInterval);
228
} else
229
usb_fill_bulk_urb(urb, usb_dev, pipe, data, len,
230
usb_api_blocking_completion, NULL);
231
232
return usb_start_wait_urb(urb, timeout, actual_length);
233 }

都坚持走到这里了,看懂这个函数还是很easy的,首先根据指定的pipe获得端点的方向和
端点号,然后从设备struct usb_device结构体的ep_in或ep_out数组里得道端点对应的
struct usb_host_endpoint结构体,接着调用 usb_alloc_urb创建urb。因为这个函数可
能 是 从 usb_interrupt_msg 那 里 调 用 过 来 的 , 所 以 接 下 来 要 根 据 端 点 描 述 符 的
bmAttributes字段获取传输的类型,判断究竟是中断传输还是批量传输,是中断传输的话
还要修改pipe的类型,防止万一谁谁直接调用usb_bulk_msg来完成中断传输,虽说很少
人会穷极无聊到这种地步,预防一下总归是没错的。不管是中断传输还是批量传输,都要调
用 usb_fill_xxx_urb 来 初 始 化 , 最 后 , 和 usb_control_msg 一 样 , 调 用
usb_start_wait_urb函数。



## 设备的生命线(七)

### struct usb_hcd

```
49 /*
50 * USB Host Controller Driver (usb_hcd) framework
51 *
52 * Since "struct usb_bus" is so thin, you can't share much code in it.
53 * This framework is a layer over that, and should be more sharable.
54 */
55
56 /*------------------------------------------------------------------------*/
57
58 struct usb_hcd {
59
60 /*
61 * housekeeping
62 */
63 struct usb_bus self; /* hcd is-a bus */
63 行，又一个结构体，struct usb_bus，还名曰self，struct usb_hcd里还有self，看来
这家伙是双子座的，以为能再分裂出一个自己，和黄金十二宫里的双子一样。俺也是双子的，
不过俺从来没想再分裂出一个，现在世道艰难，一个就已经存活不易了。
为什么这里会用这么一个戏剧性的词汇self？难道Greg他们都是具有乐观主义精神的无产
阶级革命者？当然不是，他们都是资产阶级，咱们才是无产阶级。俺在前面的某处提到过那
么一下，一个主机控制器就会连出一条usb总线，主机控制器驱动用struct usb_hcd结构
表示，一条总线用struct usb_bus结构表示，它们是白天与黑夜般相生相依的关系，一个
白天只能连着一个黑夜，一个黑夜只能引出一个白天，没听说过谁过了两个白天才到夜里的，
如果谁说了，他不是疯子就是哲学家或经济学家。struct usb_bus在include/linux/usb.h
里定义
64 struct kref kref; /* reference counter */
65
66 const char *product_desc; /* product/vendor string */
67 char irq_descr[24]; /* driver + bus # */
68
69 struct timer_list rh_timer; /* drives root-hub polling */
70 struct urb *status_urb; /* the current status urb */
71 #ifdef CONFIG_PM
72 struct work_struct wakeup_work; /* for remote wakeup */
73 #endif
74
75 /*
76 * hardware info/state
77 */
78 const struct hc_driver *driver; /* hw-specific hooks */
79
80 /* Flags that need to be manipulated atomically */
81 unsigned long flags;
82 #define HCD_FLAG_HW_ACCESSIBLE 0x00000001 
 135
83 #define HCD_FLAG_SAW_IRQ 0x00000002
84
85 unsigned rh_registered:1;/* is root hub registered? */
86
87 /* The next flag is a stopgap, to be removed when all the HCDs
88 * support the new root-hub polling mechanism. */
89 unsigned uses_new_polling:1;
90 unsigned poll_rh:1; /* poll for rh status? */
91 unsigned poll_pending:1; /* status has changed? */
92 unsigned wireless:1; /* Wireless USB HCD */
93
94 int irq; /* irq allocated */
95 void __iomem *regs; /* device memory/io */
96 u64 rsrc_start; /* memory/io resource start */
97 u64 rsrc_len; /* memory/io resource length */
98 unsigned power_budget; /* in mA, 0 = no limit */
99
100 #define HCD_BUFFER_POOLS 4
101 struct dma_pool *pool [HCD_BUFFER_POOLS];
102
103 int state;
104 # define __ACTIVE 0x01
105 # define __SUSPEND 0x04
106 # define __TRANSIENT 0x80
107
108 # define HC_STATE_HALT 0
109 # define HC_STATE_RUNNING (__ACTIVE)
110 # define HC_STATE_QUIESCING (__SUSPEND|__TRANSIENT|__ACTIVE)
111 # define HC_STATE_RESUMING (__SUSPEND|__TRANSIENT)
112 # define HC_STATE_SUSPENDED (__SUSPEND)
113
114 #define HC_IS_RUNNING(state) ((state) & __ACTIVE)
115 #define HC_IS_SUSPENDED(state) ((state) & __SUSPEND)
116
117 /* more shared queuing code would be good; it should support
118 * smarter scheduling, handle transaction translators, etc;
119 * input size of periodic table to an interrupt scheduler.
120 * (ohci 32, uhci 1024, ehci 256/512/1024).
121 */
122
123 /* The HC driver's private data is stored at the end of
124 * this structure.
125 */
126 unsigned long hcd_priv[0] 
127 __attribute__ ((aligned (sizeof(unsigned long))));
128 }; 
```



### struct usb_bus

```
273 /*
274 * Allocated per bus (tree of devices) we have:
275 */
276 struct usb_bus {
277 struct device *controller; /* host/master side hardware */
278 int busnum; /* Bus number (in order of reg) */
279 char *bus_name; /* stable id (PCI slot_name etc) */
280 u8 uses_dma; /* Does the host controller use DMA? */
281 u8 otg_port; /* 0, or number of OTG/HNP port */
282 unsigned is_b_host:1; /* true during some HNP roleswitches */
283 unsigned b_hnp_enable:1; /* OTG: did A-Host enable HNP? */
284
285 int devnum_next; /* Next open device number in
286 * round-robin allocation */
287
288 struct usb_devmap devmap; /* device address allocation map */
289 struct usb_device *root_hub; /* Root hub */
290 struct list_head bus_list; /* list of busses */
291
292 int bandwidth_allocated; /* on this bus: how much of the time
293 * reserved for periodic (intr/iso)
294 * requests is used, on average?
295 * Units: microseconds/frame. 
 137
296 * Limits: Full/low speed reserve 90%,
297 * while high speed reserves 80%.
298 */
299 int bandwidth_int_reqs; /* number of Interrupt requests */
300 int bandwidth_isoc_reqs; /* number of Isoc. requests */
301
302 #ifdef CONFIG_USB_DEVICEFS
303 struct dentry *usbfs_dentry; /* usbfs dentry entry for the bus */
304 #endif
305 struct class_device *class_dev; /* class device for this bus */
306
307 #if defined(CONFIG_USB_MON)
308 struct mon_bus *mon_bus; /* non-null when associated */
309 int monitored; /* non-zero when monitored */
310 #endif
311 }; 
```

277 行，controller，struct usb_hcd那里含了个usb_bus，这里就回应了个controller， 你山西来个黑砖窑，我唐山就回应个黑军车，黑对黑，遥相呼应。那现在通过struct usb_hcd 里的self和struct usb_bus里的controller这两个很有乐观主义精神的词儿，你能不能说下 它们到底是什么关系？你当然可以说是一个对应主机控制器，一个描述一条总线，但其实对 于写代码的来说一个主机控制器和一条总线差不多是一码事，不用分的那么清，可以简单的 说它们都是用来描述主机控制器的，那为什么又分成了两个结构，难道Greg他们现在又不 信奉简约主义了？

这个问题的答案我也很想知道，但知道了又能怎么样？知道了你就能明白为什么美女都不喜 欢你，为什么她们身上衣服件数越多反而露得越多？你不能明白，所以也不用去知道了。不 过思索了一杯茶的时间，还是有那么点儿线索。不要小看这杯茶，按日里万鸡来换算，这点 儿时间都能理多少鸡了？

前面说过 linux 里和小李飞刀齐名的就是设备模型了，usb 主机控制器当然也是一个设备， 而且更多的时候它还是一个 PCI 设备，那它就应该纳入这个设备模型范畴之内，struct usb_hcd 结构里就得嵌入类似 struct device 或 struct pci_dev 这样的一个结构体，但是 你仔细瞅瞅，能不能在它里面发现这么一个成员？不能，对于一个设备来说，这可是大逆不 道的。但是你再瞅瞅 struct usb_bus，第一个就是一个 struct device 结构体。好，第一 条线索就先到这儿。

再利用这杯茶的时间挑个具体的主机控制器驱动程序快速的走一下，就 UHCI 吧，都在 host 目录下的 uhci-族文件里，首先它是个 pci 设备，要使用 pci_register_driver 注册一个 struct pci_driver 结构体 uhci_pci_driver，uhci_pci_driver 里又有个熟悉的 probe， 在这个 probe 里，它调用 usb_create_hcd 来创建一个 usb_hcd，初始化里面的 self， 还将这个 self 里的 controller 设定为描述主机控制器的那个 pci_dev 里的 struct device 138 结构体，从而将 usb_hcd、usb_bus 和 pci_dev，甚至设备模型都连接起来了。

这杯茶应该还没有这么快就喝的完，那就再接着巡视一下 uhci-文件里定义的那些函数，只 用看它们的参数，你会发现参数里不是 struct usb_hcd 就是 struct uhci_hcd，如果你和 我一样无聊愿意多看点的话，你会看到那些函数的前面几行常常会有 hcd_to_uhci 或者 uhci_to_hcd 这样的函数在 struct usb_hcd 和 struct uhci_hcd 之间做着转换。struct uhci_hcd 是什么？它是 uhci 自己私有的一个结构体，就像每个成功的男人背后都有一个 女人一样，每个具体的主机控制器都有这么一个类似的结构体。如果你再无聊一下，顺便瞧 了下 hcd_to_uhci 或者 uhci_to_hcd 的定义，你就会明白，每个主机控制器的这个私有 结构体都藏在 struct usb_hcd 结构最后的那个 hcd_priv 变长数组里。

通过这杯茶，你能悟出什么？如果说镜头闪的太快，让你看的不太明白，那就只管听俺说好 了。对于具体的主机控制器驱动来说，它们的眼里只有 struct usb_hcd，struct usb_hcd 结构之于主机控制器驱动，就如同 struct usb_device 或 struct usb_interface 之于 usb 驱动。没有 usb_create_hcd 去创建 usb_hcd，就不会有 usb_bus 的存在。而对于 linux 设备模型来说，struct usb_bus 无疑要更亲切一些。总之，你可以把 struct usb_bus 当 作只是嵌入到 struct usb_hcd 里面的一个结构体，它将 struct usb_hcd 要完成的一部分 工作进行了封装，因为要描述一个主机控制器太复杂太难，于是就开了 struct usb_bus 这 么一个窗户去专门面对设备模型、sysfs 等等。这也就是俺开头儿就说这个片儿区，struct usb_hcd 才是王中之王的原因。

你知道 Greg 他们是怎么描述这种奇妙的关系么？他们把这个叫作 HCD bus-glue layer， 并致力于 flatten out it. 这个关系早先是比较混沌的，现在要清晰些，以后只会更清晰， struct usb_hcd 越来越走上台前，struct usb_bus 越来越走向幕后。就好像我们一开始 是天地混沌，然后是女娲造人，有了社会有了阶级，再然后有了 GCD 才有了新中国一样。

278 行，busnum，总线编号，你的机子里总可以有多个主机控制器吧，自然也就可以有 多条 usb 总线了，既然可以有多条，就要编个号方便确认了。有关总线编号，可以看看定 义在 drivesr/usb/core/hcd.c 里的这几行

```
88 /* used when allocating bus numbers */
89 #define USB_MAXBUS 64
90 struct usb_busmap {
91 unsigned long busmap [USB_MAXBUS / (8*sizeof (unsigned long))];
92 };
93 static struct usb_busmap busmap; 
```

讲struct usb_device的devnum时候，说到过一个devicemap，这里又有个 busmap， 当时分析说devicemap一共有 128 位，同理可知，这里的 busmap一共有 64 位，也就是 说最多可以有 64 条usb总线，如果你还觉得不够，言一声，我可以躲你远远的。

279 行，bus_name，bus 总线，name 名字，bus_name 总线的名字，什么样的名字？

要知道大多数情况下主机控制器都是一个 PCI 设备，那么 bus_name 应该就是用来在 PCI 总线上标识 usb 主机控制器的名字，PCI 总线使用标准的 PCI ID 来标识 PCI 设备，所以 bus_name 里保存的应该就是主机控制器对应的 PCI ID。UHCI 等调用 usb_create_hcd 创建 usb_hcd 的时候确实是将它们对应 PCI ID 赋给了 bus_name。

现在简单说说这个 PCI ID。PCI spec 允许单个系统可以最多有 256 条 PCI 总线，对咱们 当然是太多了，但是对于一些极变态，需求极为旺盛的系统，它可能还觉得这满足不了要求， 于是所有的 PCI 总线又被划分为 domain，每个 PCI domain 又可以最多拥有 256 条总线， 这下总该够了吧，而每条总线上又可以支持 32 个设备，这些设备里边儿还都可以是多功能 板，它们还都可以最多支持 8 种功能。那系统怎么来区分每种功能？总要知道它在哪个 domain，哪条总线，哪个设备板上吧。这么说还是太笼统了，你可以用 lspci 命令看一下

```
00:00.0 Host bridge: Intel Corporation 440BX/ZX/DX - 82443BX/ZX/DX Host bridge (rev 01)
00:01.0 PCI bridge: Intel Corporation 440BX/ZX/DX - 82443BX/ZX/DX AGP bridge (rev 01)
00:07.0 ISA bridge: Intel Corporation 82371AB/EB/MB PIIX4 ISA (rev 08)
00:07.1 IDE interface: Intel Corporation 82371AB/EB/MB PIIX4 IDE (rev 01)
00:07.2 USB Controller: Intel Corporation 82371AB/EB/MB PIIX4 USB
00:07.3 Bridge: Intel Corporation 82371AB/EB/MB PIIX4 ACPI (rev 08)
00:0f.0 VGA compatible controller: VMware Inc [VMware SVGA II] PCI Display Adapter
00:10.0 SCSI storage controller: LSI Logic / Symbios Logic 53c1030 PCI-X Fusion-MPT Dual Ultra320
SCSI (rev 01)
00:11.0 Ethernet controller: Advanced Micro Devices [AMD] 79c970 [PCnet32 LANCE] (rev 10)
00:12.0 Multimedia audio controller: Ensoniq ES1371 [AudioPCI-97] (rev 02) 
```

每行前面的数字就是所谓的 PCI ID，每个 PCI ID 由 domain 号（16 位），总线编号（8 位），设备号（5 位），功能号（3 位）组成，不过这里 lspci 没有标明 domain 号，但对于 一台普通 PC 而言，一般也就只有一个 domain，0x0000。

280 行，uses_dma，表明这个主机控制器支持不支持 DMA。主机控制器的一项重要工作 就是在内存和 USB 总线之间传输数据，这个过程可以使用 DMA 或者不使用 DMA，不使用 DMA 的方式即所谓的 PIO 方式。DMA 代表着 Direct Memory Access，即直接内存访问， 不需要 CPU 去干预。具体的去看看 PCI DMA 的东东吧，因为一般来说主机控制器都是 PCI 设备，uses_dma 都在它们自己的 probe 函数里设置了。

285 行，devnum_next，288 行，devmap，早就说过 devmap 这张表了，devnum_next 中记录的就是这张表里下一个为 0 的位，里面为 1 的位都是已经被这条总线上的 usb 设备 占据了的，名花有主的。

289 行，root_hub，就好像端点 0 在所有设备的端点里面那么的鹤立鸡群一样，root hub 在所有的 hub 里面也是那么的特殊，还记得 usb 的那颗树么，它就是那颗树的根，和 usb 140 主机控制器绑定在一起，其它的 hub 和设备都必须从它这儿延伸出去。正是因为这种特殊 的关系，写代码的哥们儿也素有成人之心，就直接将它放在了 struct usb_bus 结构里，让 他们永不分离。usb 主机控制器，usb 总线，root hub，1 比 1 比 1

290 行，bus_list，在 hcd.c 中定义有一个全局队列 usb_bus_list 

```
84 /* host controllers we manage */
85 LIST_HEAD (usb_bus_list);
86 EXPORT_SYMBOL_GPL (usb_bus_list);
```

它就是所有 usb 总线的组织。每次一条总线新添加进来，都要向这个组织靠拢，都要使用 bus_list 字段链接在这个队列上。

292 行，bandwidth_allocated，表明总线为中断传输和等时传输预留了多少带宽，协议 里说了，对于高速来说，最多可以有 80%，对于低速和全速要多点儿，可以达到 90%。 它的单位是微妙，表示一帧或微帧内有多少微妙可以留给中断/等时传输用。

299 行，bandwidth_int_reqs，300 行，bandwidth_isoc_reqs，分别表示当前中断传 输和等时传输的数量。

302~304 行，是 usbfs 的，每条总线都对应于/proc/bus/usb 下的一个目录。你无聊的 话可以去瞅瞅。

305 行，class_dev，这里又牵涉到设备模型中的一个概念，设备的 class，即设备的类。 像前面提到的设备模型里的总线、设备、驱动三个核心概念，纯粹是从写驱动的角度看的， 而这里的类则是面向于 linux 的广大用户的，它不管你是用什么接口，怎么去连接，它只管 你对用户来说提供了什么功能，一个 SCSI 硬盘和一个 ATA 硬盘对驱动来说是八杆子打不 着的两个东西，但是对于用户来说，它们都是硬盘，都是用来备份文件，备份各种小电影的， 这也就是所谓的物以类聚人以群分。

设备模型与 sysfs 是分不开的，class 在 sysfs 里的体现就在/sys/class 下面，可以去看看 atm dma graphics hwmon i2c-adapter input mem misc net pci_bus scsi_device scsi_disk scsi_host sound spi_host spi_master spi_transport tty usb_device usb_endpoint usb_host vc vtconsole 

看到里面的 usb_host 了吧，它就是所有 usb 主机控制器的类，这些目录都是怎么来的那？ 咱们还要追溯一下 usb 子系统的初始化函数 usb_init，它里面有这么一段

```
877 retval = usb_host_init();
878 if (retval)
879 goto host_init_failed;
```

当时只是简单说这是用来初始化 host controller 的，现在鼓气勇气进去看看，在 hcd.c 里

```
671 static struct class *usb_host_class;
672
673 int usb_host_init(void)
674 {
675 int retval = 0;
676
677 usb_host_class = class_create(THIS_MODULE, "usb_host");
678 if (IS_ERR(usb_host_class))
679 retval = PTR_ERR(usb_host_class);
680 return retval;
681 } 
```

usb_host_init所作的一切就是调用 class_create创建了一个usb_host这样的类，你只要 加载了usbcore模块就能在/sys/class下面看到有usb_host目录出现。既然usb_host目录 表示的是usb主机控制器的类，那么它下面应该就对应各个具体的主机控制器了，你用ls 命 令look一下就能看到usb_host1、usb_host2 等等这样的目录，它们每个都对应一个在你 系统里实际存在的主机控制器，实际上在hcd.c里的usb_register_bus函数有这么几行

```
735 bus->class_dev = class_device_create(usb_host_class, NULL, MKDEV(0,0),
736 bus->controller, "usb_host%d", busnum); 
```

这两行就是使用 class_device_create在/sys/class/usb_host下面为每条总线创建了一 个目录，目录名里的数字代表的就是每条总线的编号，usb_register_bus函数是每个主机 控制器驱动在probe里调用的，向usb core注册一条总线，也可以说是注册一个主机控制 器。

307~310 行，CONFIG_USB_MON 是干吗用的？这要看看 drivers/usb/mon 目录下的 Kconfig 

```
1 #
2 # USB Monitor configuration
3 #
4
5 config USB_MON
6 bool "USB Monitor"
7 depends on USB!=n
8 default y
9 help
10 If you say Y here, a component which captures the USB traffic
11 between peripheral-specific drivers and HC drivers will be built.
12 For more information, see <file:Documentation/usb/usbmon.txt>.
13
14 This is somewhat experimental at this time, but it should be safe. 
15
16 If unsure, say Y. 
```

文件里就这么多内容，从里面咱们可以知道，如果定义了 CONFIG_USB_MON，一个所谓 的 usb Monitor，也就是 usb 监视器的东东就会编进内核。这个 Monitor 是用来监视 usb 总线上的底层通信流的，相关的文件都在 drivers/usb/mon 下面。2005 年的阳春三月， Greg 大侠春心思动，于是就孕育出了这个 usb Monitor。

​	

## 设备的生命线（八）

这个世界上不需要努力就能得到的东西只有一样，那就是年龄。所以要不怕苦不怕累，回到 struct usb_hcd，继续努力的往下看。 64 行，又见 kref，usb 主机控制器的引用计数。struct usb_hcd 也有自己专用的引用计 数函数，看 hcd.c 文件

```
1526 static void hcd_release (struct kref *kref)
1527 {
1528 struct usb_hcd *hcd = container_of (kref, struct usb_hcd, kref);
1529
1530 kfree(hcd);
1531 }
1532
1533 struct usb_hcd *usb_get_hcd (struct usb_hcd *hcd)
1534 {
1535 if (hcd)
1536 kref_get (&hcd->kref);
1537 return hcd;
1538 }
1539 EXPORT_SYMBOL (usb_get_hcd);
1540
1541 void usb_put_hcd (struct usb_hcd *hcd)
1542 {
1543 if (hcd)
1544 kref_put (&hcd->kref, hcd_release);
1545 }
1546 EXPORT_SYMBOL (usb_put_hcd); 
```

 和struct urb 的那几个长的也忒像了，像的俺都不好意思多介绍它们了，如果不明白就回 去看看聊 struct urb 的时候怎么说的吧。

66 行，product_desc，主机控制器的产品描述字符串，对于 UHCI，它为“UHCI Host Controller”，对于 EHCI，它为“EHCI Host Controller”。 67 行，irq_descr[24]，这里边儿保存的是“ehci-hcd:usb1”之类的字符串，也就是驱 动的大名再加上总线编号。 71~73 行，电源管理的，飘过。 78 行，driver，每个男人心中都有一个狐狸精，每个女人心里都有一个洛丽塔，每个主机 控制器驱动都有一个 struct hc_driver 结构体。看看它在 hcd.h 里的定义

## USB设备逻辑结构

![image-20210407141814638](/home/zhaoyong/.config/Typora/typora-user-images/image-20210407141814638.png)

![image-20210407141844149](/home/zhaoyong/.config/Typora/typora-user-images/image-20210407141844149.png)

https://blog.csdn.net/qq_27522735/article/details/56673783

=======
又看到了 __attribute__，不过这里改头换面成了 __attribute__ ((packed))，意思就是 告诉编译器，这个结构的元素都是 1 字节对齐的，不要再添加填充位了。因为这个结构和 spec里的Table 9.12 是完全一致的，包括字段的长度，如果不给编译器这么个暗示，编译 器就会依据你平台的类型在结构的每个元素之间添加一定的填充位，如果你拿这个添加了填 充位的结构去向设备请求描述符，你想想会是什么结果。

p45
>>>>>>> 904f55db4800469b8842ddfad4a225adc6224139
