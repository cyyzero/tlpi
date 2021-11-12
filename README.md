# 《linux/unix系统编程手册》学习记录

## 文件夹组织结构

* tlpi-dist：从[这本书的官网](http://www.man7.org/tlpi/)下载的源码
* chapter-x：第x章节
	* example：书中例子
	* exercise：书后习题

第三章介绍了后面章节要用到的头文件以及实现，如下
* ename.c.inc          定义了一个字符串数组，用于对应错误码的名称
* error_functions.h    声明了本书自定义的错误处理函数
* get_num.h            声明了本书自定义的数值提取函数
* tlpi_hdr.h           包含了后续需用到的系统调用头文件

由于存在3个头文件以及2个实现，每次编译时必须对实现也进行编译，为方便后续学习，将头文件和静态库复制到默认的编译器寻找目录下。

- 第一步：下载本书所给的源码文件

	```
	$ wget "http://man7.org/tlpi/code/download/tlpi-161214-dist.tar.gz"
	```

- 第二步：解压后，make编译（可以跳过下载和解压的过程，repo已经包含tlpi-dist目录）

	```
	$ tar -zxvf tlpi-161214-dist.tar.gz
	$ cd tlpi-dist/
	$ make -j8
	```

- 第三步：拷贝头文件和静态库至系统目录

	```
	$ cd lib/
	$ sudo cp tlpi_hdr.h /usr/local/include/
	$ sudo cp get_num.h /usr/local/include/
	$ sudo cp error_functions.h /usr/local/include/
	$ sudo cp ename.c.inc /usr/local/include/
	$ sudo cp ../libtlpi.a /usr/local/lib
	```

以后每次编译包含上述四个头文件的代码时，需要链接静态库。
以编译 example.c 为例：

```
$ gcc example.c -o example -ltlpi
```

希望以后能坚持学习吧。

