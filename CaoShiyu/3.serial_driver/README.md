# 串口接收解耦练习

在原始 `serial_driver` 资料包上重构 `RMSerialDriver::receiveData()`，保留发送逻辑和原有接收功能。

## 修改内容

- 把协议解析移到 `DefaultReceiveProtocol`：检查长度、帧头、CRC，再转换成普通数据。
- 用 `ReceivePipeline` 统一解析和分发，接收循环不再直接访问 `ReceivePacket` 的成员。
- 拆出控制指令、状态发布、云台变换三个处理器，分别依赖自己的小接口。
- 保留颜色参数同步的重试与版本判断、重置跟踪器、切换目标、四个状态话题和两段 TF 变换。
- 给 `fromVector()` 增加长度检查；补上新增源文件的构建配置和 `ament_cmake_auto` 依赖。

| 文件 | 作用 |
| --- | --- |
| `receive_data.hpp` | 与通信包布局无关的控制、状态和云台数据 |
| `receive_protocol.hpp` | 接收协议接口和解析结果 |
| `default_receive_protocol.hpp` / `.cpp` | 原始 `ReceivePacket` 协议的实现 |
| `receive_handlers.hpp` | 三个处理器和三个输出接口 |
| `receive_pipeline.hpp` | 解析成功后按注册顺序调用处理器 |
| `rm_serial_driver.hpp` / `.cpp` | 串口读取与 ROS 参数、服务、话题、TF 的实际操作 |
| `tests/receive_core_test.cpp` | 不依赖 ROS 的核心测试 |

协议字段或顺序变化时，主要修改协议实现；增加独立功能时可以添加处理器。当前读取仍限于单字节帧头、固定长度帧，不是任意协议都能直接替换。

## 验证情况

2026-10-06：在 Linux 环境用 GCC、C++14 编译核心测试，`16/16 tests passed`。
覆盖正常字段、独立字节样例、256 种标志位组合、错误长度/帧头/CRC、失败后不分发、处理顺序和替换协议。
另外运行了 AddressSanitizer（关闭环境不支持的泄漏扫描）及 UndefinedBehaviorSanitizer，未发现被测核心的越界或未定义行为。

**以上仅验证纯 C++ 核心，不表示整个 ROS 2 包已编译或机器人已联调。**
当前验证环境没有 ROS 2、战队的 `rm_interfaces` / `rm_tools` 和真实串口设备；颜色服务回调、实际 ROS 发布和 TF 仍需在战队环境验证。
`vitural_serial.cpp` 保留原样，未完成虚拟串口加分项。

## 本地核心测试

Windows PowerShell：进入本目录后执行 `./test_core.ps1`，需要现有 `g++` 或 `clang++` 已在 PATH 中。
脚本会重新编译再运行；输出程序位于被忽略的 `build/core-test`，不用上传。
如果脚本被执行策略阻止，不必修改安全策略，可以使用下面的 CMake 方式。
该脚本本身尚未在 Windows PowerShell 环境运行验证。

独立 CMake 测试（Windows / Linux，需 CMake 和兼容原包布局的 GCC/Clang）：

```sh
cmake -S tests -B build/core-test
cmake --build build/core-test
ctest --test-dir build/core-test -C Debug --output-on-failure
```

也可以在本目录用 Linux 命令直接复现已执行的测试：

```sh
mkdir -p build/core-test
g++ -std=c++14 -Wall -Wextra -Werror -pedantic -I include tests/receive_core_test.cpp src/default_receive_protocol.cpp src/crc.cpp -o build/core-test/receive_core_test
./build/core-test/receive_core_test
```

## 后续 ROS 联调

使用战队确认的 Ubuntu / ROS 2 版本，不直接照搬旧任务文档中的版本号。
需要在 ROS 工作空间里同时准备 `rm_interfaces`、`rm_tools` 及原包的其他依赖。
目录名可为 `3.serial_driver`，ROS 包名仍为 `rm_serial_driver`。

```sh
# 在准备好依赖并加载 ROS 环境的工作空间根目录运行
colcon build --packages-up-to rm_serial_driver
source install/setup.bash
colcon test --packages-select rm_serial_driver
colcon test-result --verbose
```

需要连接真实设备并确认 `config/serial_driver.yaml` 中的串口参数之后，才能启动真实串口节点。
原有阻塞读取、重连和线程退出流程未在本次重构中重新设计，相关异常场景也未实机验证。
协议沿用原包的压缩结构体、位域、浮点数布局，仍要求收发两端约定一致；未改成跨平台显式序列化。
