# ThreadPool

这是一个使用ThreadPool进行测试的示例项目。

## 简介

ThreadPool 是一个用于建立线程池的项目。

## 功能

- 可动态分配线程池的大小
- 指定线程是detach还是join模式运行
- 可暂停线程池运行或者恢复停线程池运行
- 可随时停止线程池中所有的线程

## 编译

确保你已经安装了CMake和相应的编译器。然后按照以下步骤编译项目：

1. **创建构建目录**：
   mkdir build
   cd build

2. **运行 CMake**：
    cmake..

3. **编译项目**：
    make

## 运行

编译成功后，你可以运行生成的可执行文件 ThreadPoolTest
./ThreadPoolTest

## 注意事项

确保你已经安装了 CMake 和支持 C++14 标准的编译器。

##贡献

欢迎提交问题和建议，或者通过 Pull Request 贡献代码。

## 许可证

本项目采用 MIT License。
