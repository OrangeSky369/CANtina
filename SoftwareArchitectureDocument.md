# CANtina - 通用嵌入式通信测试平台架构文档

## 1. 项目上下文

CANtina 是一个旨在提供可复用、可扩展、可脚本化的汽车通信测试平台。它支持基础的 CAN/CAN FD 报文收发、Bootloader 刷写、UDS 诊断等典型的嵌入式通信场景。为了实现高度的灵活性和可扩展性，CANtina 采用插件式架构，将每种通信协议或特定测试场景封装为独立的插件。

## 2. 工具选择

-   **硬件适配器**: PCAN 适配器 (用于 CAN/CAN FD 通信)
-   **编程语言**:
    -   Python (用于测试框架、插件和上层逻辑)
    -   C++ (用于性能敏感的通信层)
-   **C++ 构建工具**:
    -   编译器: g++
    -   构建系统: CMake

## 3. 分层架构设计

CANtina 平台采用分层架构，以明确各组件的职责并促进模块化。

```mermaid
graph TD
    A[插件层 (Python模块)] --> B{测试框架层 (Python)};
    B --> C{封装层 (Python模块, 如 .pyd)};
    C --> D[通信层 (C++)];
    D --> E[硬件层 (PCAN 适配器)];

    subgraph 应用与测试逻辑
        A
        B
    end

    subgraph 核心通信与驱动
        C
        D
        E
    end
```

### 3.1. 硬件层

-   **组件**: PCAN 适配器
-   **功能**:
    -   提供物理总线接口。
    -   支持 CAN 和 CAN FD 通信。

### 3.2. 通信层

-   **语言**: C++
-   **功能**:
    -   利用 PCAN 官方库 (`PCANBasic.dll`, `PCAN-ISO-TP.dll`, `PCAN-UDS.dll`) 进行底层报文收发。
    -   处理多帧报文的组装与拆解。
    -   实现流控制管理。
    -   封装通信协议。
-   **设计考虑**: 选用 C++ 是为了解决多帧长报文发送场景下的性能瓶颈。

### 3.3. 封装层

-   **技术**: 使用如 PyBind11 之类的工具。
-   **功能**:
    -   将 C++ 实现的通信层接口封装成 Python 模块 (例如 `.pyd` 文件)。
    -   为上层 Python 应用提供调用底层通信能力的桥梁。

### 3.4. 测试框架层

-   **语言**: Python
-   **功能**:
    -   加载测试用例。
    -   控制测试流程执行。
    -   对通信响应进行判断和断言。
    -   生成详细的测试日志和报告。

### 3.5. 插件层

-   **语言**: Python (以模块形式存在)
-   **功能**:
    -   针对特定的通信协议 (如 UDS, J1939) 或测试场景 (如 Bootloader 刷写) 实现具体逻辑。
    -   每个插件独立开发和维护，可以按需加载。
    -   提供良好的扩展性，方便未来支持新的协议或功能。

## 4. 技术上下文

### 4.1. C++ 编译器与 PCAN DLL 兼容性

-   **挑战**: PCAN 官方提供的 `.dll` 和 `.lib` 文件主要适配 MSVC 编译器，而项目选用 g++ 作为 C++ 编译器。
-   **解决方案**: 根据 PEAK 官方论坛技术支持的回复，可以通过 Windows API 函数 `LoadLibrary()` 和 `GetProcAddress()` 在使用 MinGW (GCC 编译器) 的环境下动态加载和使用 PCANBasic 接口 DLL。这意味着不需要静态链接 `.lib` 文件，从而绕过了编译器兼容性问题。

    ```c++
    // 示例代码片段 (非完整实现)
    #include <windows.h>
    #include <iostream>

    // 假设这是 PCANBasic.h 中某个函数的原型
    // typedef DWORD (*CAN_Initialize_t)(WORD, WORD, WORD, WORD, WORD);

    int main() {
        HINSTANCE hDLL = LoadLibrary("PCANBasic.dll");
        if (hDLL == NULL) {
            std::cerr << "Failed to load PCANBasic.dll. Error code: " << GetLastError() << std::endl;
            return 1;
        }

        // CAN_Initialize_t CAN_Initialize = (CAN_Initialize_t)GetProcAddress(hDLL, "CAN_Initialize");
        // if (CAN_Initialize == NULL) {
        //     std::cerr << "Failed to get address of CAN_Initialize. Error code: " << GetLastError() << std::endl;
        //     FreeLibrary(hDLL);
        //     return 1;
        // }

        // ... 调用其他函数 ...

        // FreeLibrary(hDLL);
        return 0;
    }
    ```
-   **影响**: 通信层的 C++ 代码需要相应地调整，使用动态加载 DLL 的方式来调用 PCAN 提供的 API。

## 5. 未来展望 (可选)

-   支持更多的通信硬件。
-   完善插件市场和发现机制。
-   提供图形化用户界面 (GUI) 进行配置和监控。
