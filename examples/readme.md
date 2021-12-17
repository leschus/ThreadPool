# 测试结果记录与分析

## thread_entrance.cc

### 以全局函数作为线程入口函数

1. 参数既不是指针, 也不是引用类型.

子线程会使用传入参数的副本, 因此需要进行参数拷贝.

2. 参数中有指针

指针指向调用线程的内存区域. 当该片内存被释放后, 若子线程继续访问这片区域, 预期结果是: 程序将崩溃.

实际测试结果:
* MinGW G++: 对于`int*`指针, 无异常情况; 对于用户自定义类型指针(`Foo*`), 同样无异常;
```text
Out of block 1
In ThreadFuncWithPtr: 10
End of ThreadFuncWithPtr
Foo's c'tor
Foo's d'tor
Out of block 2
In ThreadFuncWithPtr: Mike
End of ThreadFuncWithPtr
```

* Microsoft C/C++: 对于`int*`指针, 无异常情况; 对于用户自定义类型指针(`Foo*`), 获取到空内容, 程序继续运行. (见下)
```text
Out of block 1
In ThreadFuncWithPtr: 10
End of ThreadFuncWithPtr
Foo's c'tor
Foo's d'tor
Out of block 2
In ThreadFuncWithPtr:
End of ThreadFuncWithPtr
```

3. 参数中有引用

测试结果和参数中有指针是一样的. 即在MinGW G++中表现正常, 在Microsoft C/C++中也仅用户自定义类型的引用出现异常.

* MinGW G++
```text
Out of block 1
In ThreadFuncWithReference: 10
End of ThreadFuncWithReference
Foo's c'tor
Foo's d'tor
Out of block 2
In ThreadFuncWithReference: Nancy
End of ThreadFuncWithReference
```

* Microsoft C/C++
```text
Out of block 1
In ThreadFuncWithReference: 10
End of ThreadFuncWithReference
Foo's c'tor
Foo's d'tor
Out of block 2
In ThreadFuncWithReference:
End of ThreadFuncWithReference
```