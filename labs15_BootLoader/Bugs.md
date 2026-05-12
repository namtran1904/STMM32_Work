# BUGS LOG: NHẬT KÝ VƯỢT LỖI S15

### Bug 1: Sai chân LED vật lý
- **Hiện tượng:** Nạp code đúng nhưng đèn không nháy.
- **Nguyên nhân:** Dùng PC13 (Blackpill) cho board Nucleo-F411RE (LED nằm ở PA5).
- **Khắc phục:** Chuyển toàn bộ cấu hình Port C sang Port A, Pin 5.

### Bug 2: Hàm Delay bị trình biên dịch xóa
- **Hiện tượng:** LED đứng yên hoặc sáng lờ mờ.
- **Nguyên nhân:** Biến vòng lặp `for` không có từ khóa `volatile`, bị compiler tối ưu hóa xóa bỏ vì "không làm gì".
- **Khắc phục:** Thêm `volatile uint32_t i` vào vòng lặp delay.

### Bug 3: Thao tác thanh ghi BSRR sai cách
- **Hiện tượng:** LED nháy không ổn định.
- **Nguyên nhân:** Dùng `BSRR |= (1<<5)`. BSRR là thanh ghi chỉ ghi (Write-only), không được dùng toán tử `|=` (Read-Modify-Write).
- **Khắc phục:** Dùng toán tử gán trực tiếp `BSRR = (1<<5)`.

### Bug 4: Lỗi nạp chồng vùng nhớ (Flashing Error)
- **Hiện tượng:** Nạp xong cái này thì mất cái kia.
- **Nguyên nhân:** Dùng chế độ "Full Chip Erase" khi nạp Bootloader làm xóa mất Application ở 0x08008000.
- **Khắc phục:** Dùng chế độ "Sector Erase" hoặc nạp thủ công qua file .bin với địa chỉ xác định trong STM32CubeProgrammer.
