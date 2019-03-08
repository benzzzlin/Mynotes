> GNU、Linux和GNU/Linux之间的关系
相信很多人看到了这个标题就会产生疑问，这篇文章到底要讲什么东西？在回答这个问题之前，我先提出几个问题？

什么是linux？

什么是gnu？

gnu/linux是什么玩意儿？

在回答了以上问题后，大家自然也清楚我想说明的问题了。

什么是linux

相信了解一些it知识的人都听过linux这个名词。维基百科上的linux词条是最经典和最准确的说明（http://zh.wikipedia.org/wiki/linux），在此直接应用，免去班门弄斧之嫌。

linux操作系统（linux），是一种计算机操作系统。linux操作系统的内核的名字也是“linux”。linux操作系统也是自由软件和开放源代码发展中最著名的例子。

linux内核最初是为英特尔386微处理器设计的。现在linux内核支持从个人电脑到大型主机甚至包括嵌入式系统在内的各种硬件设备。

在开始的时候，linux只是个人狂热爱好的一种产物。但是现在，linux已经成为了一种受到广泛关注和支持的一种操作系统。包括ibm和惠普在内的一些计算机业巨头也开始支持linux。很多人认为，和其他的商用unix系统以及微软 windows相比，作为自由软件的linux具有低成本，安全性高，更加可信赖的优势。

linux内核最初只是由芬兰人林纳斯•托瓦兹(linus torvalds)在赫尔辛基大学上学时出于个人爱好而编写的。最初的设想中，linux是一种类似minix这样的一种操作系统。linux的第一个版本在1991年9月被发布在internet上，随后在10月份第二个版本就发布了。

linux的历史是和gnu紧密联系在一起的。从1983年开始的gnu计划致力于开发一个自由并且完整的类unix操作系统，包括软件开发工具和各种应用程序。到1991年linux内核发布的时候，gnu已经几乎完成了除了系统内核之外的各种必备软件的开发。在linus torvalds和其他开发人员的努力下，gnu组件可以运行于linux内核之上。整个内核是基于gnu通用公共许可，也就是gpl(general pubic license)的，但是linux内核并不是gnu 计划的一部分。

linux的标志和吉祥物是一只名字叫做tux的企鹅，linux的注册商标是linus torvalds所有的。这是由于在1996年，一个名字叫做william r. della croce的律师开始向各个linux发布商发信，声明他拥有linux商标的所有权，并且要求各个发布商支付版税，这些发行商集体进行上诉，要求将该注册商标重新分配给linus torvalds。

linux的某些版本是不需要安装就能使用的版本，他们称为livecd

什么是gnu

gnu计划，又称革奴计划，是由richard stallman在1983年9月27日公开发起的。它的目标是创建一套完全自由的操作系统。richard stallman最早是在net.unix-wizards新闻组上公布该消息，并附带一份《gnu宣言》等解释为何发起该计划的文章，其中一个理由就是要“重现当年软件界合作互助的团结精神”。

gnu是“gnu's not unix”的递归缩写。stallman宣布gnu应当发音为guh-noo，与canoe发音相同，以避免与gnu（非洲牛羚，发音与new相同）这个单词混淆。unix是一种广泛使用的商业操作系统的名称。由于gnu将要实现unix系统的接口标准，因此gnu计划可以分别开发不同的操作系统部件。gnu计划采用了部分当时已经可自由使用的软件，例如tex排版系统和x window视窗系统等。不过gnu计划也开发了大批其他的自由软件。

为保证gnu软件可以自由地“使用、复制、修改和发布”，所有gnu软件都在一份在禁止其他人添加任何限制的情况下授权所有权利给任何人的协议条款，gnu通用公共许可证（gnu general public license，gpl）。这个就是被称为“反版权”（或称copyleft）的概念。

1985年richard stallman又创立了自由软件基金会（free software foundati hat兼并的cygnus soluti

到了1990年，gnu计划已经开发出的软件包括了一个功能强大的文字编辑器emacs，c语言编译器gcc，以及大部分unix系统的程序库和工具。唯一依然没有完成的重要组件就是操作系统的内核(称为hurd)。

1991年linus torvalds编写出了与unix兼容的linux操作系统内核并在gpl条款下发布。linux之后在网上广泛流传，许多程序员参与了开发与修改。1992年linux与其他gnu软件结合，完全自由的操作系统正式诞生。（尽管如此gnu计划自己的内核hurd依然在开发中，目前已经发布beta版本。）

许多unix系统上也安装了gnu软件，因为gnu软件的质量比之前unix的软件还要好。gnu工具还被广泛地移植到windows和mac os上。

注：以上内容来自维基百科gnu词条（http://zh.wikipedia.org/wiki/gnu）

gnu/linux是什么玩意儿

通过回答上面两个问题，我们很明显可以注意到linux只是一个操作系统内核而已，而gnu提供了大量的自由软件来丰富在其之上各种应用程序。

因此，严格来讲，linux这个词本身只表示linux内核，但在实际上人们已经习惯了用linux来形容整个基于linux内核，并且使用gnu 工程各种工具和数据库的操作系统。基于这些组件的linux软件被称为linux发行版。一般来讲，一个linux发行套件包含大量的软件，比如软件开发工具，数据库，web服务器（例如apache)，x window，桌面环境（比如gnome和kde），办公套件（比如openoffice.org），等等。

绝大多数基于linux内核的操作系统使用了大量的gnu软件，包括了一个shell程序、工具、程序库、编译器及工具，还有许多其他程序，例如emacs。正是由于linux使用了许多gnu程序，gnu计划的开创者richard stallman博士提议将linux操作系统改名为gnu/linux。但有些人只把操作系统叫做"linux"。

有部分linux套件，包括了debian，采用了“gnu/linux”的称呼。但大多数商业linux套件依然将操作系统称为linux。有些人也认为“操作系统”一词指的应该只是系统的内核，其他程序都只能算是应用软件，这么一来，该操作系统的内核应叫linux，而linux套件是在linux内核的基础上加入各种gnu工具。

一些人拒绝使用“gnu/linux”作为操作系统名称的人认为：

linux朗朗上口；名字较短又好记；

richard stallman博士直到1990年代中期，linux开始流行后才要求更名。

结语

我相信，坚持看完这些枯涩文字的读者一定明白了我需要探讨的问题，我们常说的linux，准确地来讲，应该是叫“gnu/linux”。虽然，我们没有为gnu和linux的开发做出什么贡献，但是我们可以为gnu和linux的宣传和应用做出微薄的努力，至少我们能够准确地去向其他人解释清楚gnu、linux以及gnu/linux之间的区别。让我们一起为gnu/linux的推广贡献出自己的力量！

作者：浩杰ee
链接：https://www.jianshu.com/p/d561105321ae
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。