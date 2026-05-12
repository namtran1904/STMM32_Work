# BUGS LOG - Labs42 I2C Mastery

Trong quá trình phát triển Mini-lib I2C, các lỗi sau đây đã được phát hiện và xử lý thành công:

### Lỗi 1: I2C Init bị treo hoặc sai tần số
- **Triệu chứng:** Cảm biến không phản hồi hoặc I2C đo được xung clock sai lệch.
- **Nguyên nhân:** Dùng toán tử `|=` trên thanh ghi CCR và TRISE dẫn đến dính giá trị rác từ lần khởi động trước. Chưa tắt I2C (PE=0) khi cấu hình.
- **Giải pháp:** Dùng toán tử `=` để ghi đè giá trị sạch. Luôn tắt I2C trước khi nạp thông số tốc độ.

### Lỗi 2: Treo Bus khi nhận 1 Byte
- **Triệu chứng:** Sau khi đọc xong thanh ghi WHO_AM_I, chương trình đứng im hoặc bus SDA bị kéo thấp mãi mãi.
- **Nguyên nhân:** Quy trình NACK và STOP sai thứ tự.
- **Giải pháp:** Phải thực hiện đúng trình tự: `Gửi NACK` -> `Xóa cờ ADDR` -> `Gửi lệnh STOP` -> `Đợi RXNE` -> `Đọc DR`.

### Lỗi 3: Vi điều khiển bị Reset liên tục (FPU Fault)
- **Triệu chứng:** In ra tiêu đề xong chip tự Reset (lặp lại vô hạn).
- **Nguyên nhân:** MPU6050 trả về số thực (float), nhưng bộ tính toán số thực FPU của Cortex-M4 chưa được kích hoạt.
- **Giải pháp:** Bật FPU thông qua thanh ghi `CPACR` ngay đầu hàm `main`.

### Lỗi 4: Lỗi cờ ADDR không xóa được
- **Triệu chứng:** Code kẹt tại vòng lặp đợi dữ liệu về.
- **Nguyên nhân:** Đọc SR1 hai lần liên tiếp khiến trình tự xóa cờ (SR1 -> SR2) bị hỏng.
- **Giải pháp:** Chỉ đọc SR2 một lần duy nhất sau khi hàm `WaitForFlag` (đã đọc SR1) báo thành công.
