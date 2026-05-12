# LAB S15: BOOTLOADER FUNDAMENTALS

## 1. Giới thiệu
Bài Lab này thực hiện cơ chế nhảy vùng nhớ từ Bootloader sang Application trên STM32F411RE (Nucleo). Đây là nền tảng cho tính năng tự cập nhật phần mềm (OTA) của Robot NDT.

## 2. Cấu trúc bộ nhớ
- **Bootloader:** 0x0800 0000 -> 0x0800 7FFF (32KB)
- **Application:** 0x0800 8000 -> End of Flash

## 3. Cách nạp và chạy
1. Biên dịch Project Application (Nhớ chỉnh Linker Script dời địa chỉ về 0x08008000).
2. Biên dịch Project Bootloader (Để địa chỉ mặc định).
3. Nạp Application.bin vào 0x08008000 bằng STM32CubeProgrammer.
4. Nạp Bootloader.bin vào 0x08000000.
5. Nhấn Reset và quan sát sự thay đổi tốc độ nháy LED trên chân PA5.

## 4. Công cụ sử dụng
- VS Code + STM32 Extension (Biên dịch).
- STM32CubeProgrammer (Nạp code cưỡng bức).
- Ngôn ngữ: C Bare-metal (Thanh ghi).
