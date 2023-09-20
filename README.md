# PixelSimulaDisplay
  Use Qt6 UI to simulate a dot matrix screen (28*16) display function, and update the screen display buffer data through socket.

## View

<div align=left><img width="593" height="123" src="https://github.com/to9/PixelSimulaDisplay/blob/main/images/pixel_simual_display.png"/></div>

## Socket Transmission Protocol

帧结构示意图： 
| 帧头 | 帧编号 | 数据长度 | 数据 | CRC校验 |
1. 帧头字段(4字节)：用于标识帧的起始。固定: 0xAA557d5E
2. 数据长度字段(4字节)：表示数据长度，包括(数据)
3. 帧编号字段(4字节)：用于标识数据传输序号，表示数据在整个数据流中的顺序。无符号0x00 - 0xFFFFFFFF
4. 数据字段(n字节)：实际传输的数据。
5. CRC校验字段(4字节)：计算方式(帧头, 帧编号, 数据长度, 数据)校验值CRC32
6. 一包数据总长度为(帧头(4), 帧编号(4), 数据长度(4), 数据(n), CRC32(4))
