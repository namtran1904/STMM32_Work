# I2C FLOW LOGIC - Labs42

Quy trình chuẩn hóa cho các hàm trong Mini-lib I2C.

## 1. I2C_Init (Khởi tạo)
1. Bật Clock ngoại vi (I2C & GPIO).
2. Tắt I2C (`PE = 0`).
3. Ghi tần số vào `CR2` (Tần số APB1).
4. Tính toán và nạp `CCR` (Tốc độ) + `TRISE` (Thời gian nạp tụ).
5. Set bit 14 thanh ghi `OAR1` (Bắt buộc theo RM).
6. Bật I2C (`PE = 1`).
7. Bật `ACK` (Chỉ làm sau khi PE=1).

## 2. I2C_MasterReceiveData (Nhận dữ liệu)

### Trường hợp 1 Byte (Len == 1):
1. Tạo **START**.
2. Gửi **Địa chỉ + Bit Đọc (1)**.
3. Đợi cờ **ADDR = 1**.
4. **Tắt ACK** (NACK).
5. **Xóa cờ ADDR** (Đọc SR1, sau đó đọc SR2).
6. **Gửi lệnh STOP**.
7. Đợi **RXNE = 1**.
8. Đọc dữ liệu từ **DR**.

### Trường hợp Nhiều Byte (Len > 1):
1. Tạo **START**.
2. Gửi **Địa chỉ + Bit Đọc (1)**.
3. Đợi cờ **ADDR = 1**.
4. **Xóa cờ ADDR** (Đọc SR1, sau đó đọc SR2).
5. Vòng lặp `for` nhận dữ liệu:
   - Đợi **RXNE = 1**.
   - **Nếu Len == 2:** (Gửi tín hiệu kết thúc chuẩn bị cho byte cuối)
     - Tắt **ACK**.
     - Gửi lệnh **STOP**.
   - Đọc dữ liệu từ **DR** -> Lưu vào Buffer.
6. Kết thúc vòng lặp, bật lại **ACK** cho hệ thống.

## 3. MPU6050 Wake-up
1. Gửi lệnh kiểm tra thanh ghi định danh (0x75 - WHO_AM_I).
2. Nếu phản hồi đúng 0x68:
   - Gửi giá trị 0x00 vào thanh ghi quản lý nguồn (0x6B) để đánh thức cảm biến khỏi chế độ Sleep.
