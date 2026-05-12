# Labs42 - Custom I2C Mini-Library for STM32F4

Dự án thực hiện việc tự xây dựng thư viện giao tiếp I2C ở mức thanh ghi (không dùng HAL) để điều khiển cảm biến MPU6050 trên dòng vi điều khiển STM32F411 (Clock 100MHz).

## Thông số kỹ thuật
- **MCU:** STM32F411CEU6 (Blackpill)
- **Clock:** System Clock 100MHz, APB1 50MHz.
- **Giao tiếp:** 
  - I2C1 (SCL: PB8, SDA: PB9) - Tốc độ 100kHz.
  - UART2 (TX: PA2) - Baudrate 115200 (để Debug).
- **Cảm biến:** MPU6050 (Địa chỉ 0x68 - Chân AD0 nối GND).

## Cấu trúc thư mục
- `mcu_i2c.c/h`: Thư viện lõi I2C tự viết.
- `mpu6050.c/h`: Driver điều khiển cảm biến.
- `mcu_rcc.c/h`: Cấu hình Clock hệ thống.
- `main.c`: Chương trình chính thực hiện kiểm tra và in dữ liệu.

## Tính năng nổi bật
- Quản lý cờ trạng thái I2C chuẩn xác theo Reference Manual RM0090.
- Cơ chế Timeout ngăn chặn treo Bus khi phần cứng gặp sự cố.
- Xử lý đặc biệt cho quy trình nhận 1 byte và nhiều byte.
