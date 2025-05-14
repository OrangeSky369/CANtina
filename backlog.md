# 1 PG3 (仅底层，仅C++)
## 1.1 Initialization
1. Initialization的各种参数比如PCAN_USBBUS1和波特率等都写死
500 + 2M: "f_clock_mhz=80, nom_brp=10, nom_tseg1=12, nom_tseg2=3, nom_sjw=3, data_brp=4, data_tseg1=7, data_tseg2=2, data_sjw=1"

## 1.2 单帧收发
1. 进取版：使用PCAN-ISO-TP将data结构化，并且使用PCAN-UDS进一步结构化
2. 进取版：使用PCAN-ISO-TP将data结构化
3. 保守版：直接发送按照规则重构好的的报文

## 1.3 SecurityAccess
1. 使用.dll

## 1.3 多帧发送
1. 进取版：使用PCAN-ISO-TP及PCAN-UDS
2. 保守版：直接发送按照规则重构好的的报文

## 1.4 Uninitialization

# 2 PG2（with PyBind11 & python）
## 2.1 底层优化
### 2.1.1 Initialization
1. Initialization能够自动识别连接的设备名称，有用户接口可以选择波特率等内容

### 2.1.2 单帧收发
1. 使用PCAN-ISO-TP & PCAN-UDS将数据结构化

### 2.1.3 多帧发送

### 2.1.4 Uninitialization

## 2.2 应用层
### 2.2.1 PyBind11封装.pyd接口文件

### 2.2.2 python用例处理

### 2.2.3 python结果判断

### 2.2.4 python报告生成

