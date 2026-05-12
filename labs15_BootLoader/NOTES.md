## 1. Bản chất hệ thống (Tư duy thực chiến)

- **Nguyên lý chia đất:** Bộ nhớ Flash được chia thành 2 vùng độc lập: 
    - **Vùng 1 (Bootloader):** Bắt đầu tại địa chỉ "khóa cứng" `0x0800 0000`. CPU luôn bị ép phải đáp xuống đây đầu tiên khi vừa Reset.
    - **Vùng 2 (Application):** Bắt đầu tại địa chỉ `0x0800 8000` (Địa chỉ này chọn dựa theo phân vùng Sector của chip).

- **Sở hữu thông tin:** 
    - Mỗi chương trình đều có "Căn cước công dân" riêng nằm ở 8 byte đầu tiên, bao gồm: **MSP** (Ví tiền/Ngăn xếp) và **Reset Handler** (Điểm bắt đầu/Cửa chính). 
    - MSP và Reset của App nằm sẵn trong vùng nhớ của App (do Linker Script quy định lúc biên dịch).

- **Hành động của Bootloader (Người dẫn đường):**
    - Bootloader không làm việc cho App mãi mãi. Nhiệm vụ của nó là **"Đọc trộm"** 8 byte thông tin (CCCD) tại địa chỉ khởi đầu của App.
    - Sau khi đọc xong, Bootloader **"Ra lệnh"** cho CPU: 
        1. "Nạp ví tiền này vào!" (Ghi MSP của App vào thanh ghi MSP của CPU).
        2. "Chạy vào cửa này!" (Ghi Reset của App vào thanh ghi PC của CPU).

- **Kết quả:** Ngay sau khi ra lệnh, CPU thoát xác khỏi Bootloader và biến thành "linh hồn" của Application để bắt đầu làm việc.

## 2. Cấu trúc Vector Table (32-bit)
Mỗi thành phần chiếm đúng **4 bytes**.
- **Offset 0x00:** Initial Main Stack Pointer (MSP) - Địa chỉ vùng RAM.
- **Offset 0x04:** Reset Handler - Địa chỉ lệnh thực thi đầu tiên.

## 3. Các bước nhảy an toàn
1. Đọc MSP và Reset Handler của Application tại địa chỉ đích (0x0800 8000).
2. Kiểm tra tính hợp lệ (MSP phải thuộc vùng 0x20xxxxxx).
3. Khóa ngắt toàn cục (`cpsid i`) để tránh xung đột khi nhảy.
4. Thiết lập MSP mới bằng lệnh Assembly `MSR msp, reg`.
5. Nhảy vào địa chỉ Reset Handler bằng con trỏ hàm.

## 4. Nhiệm vụ của Application
- Phải tự định vị lại bảng Vector Table bằng thanh ghi `SCB->VTOR`.
- Nếu không có `SCB->VTOR = 0x08008000;`, mọi ngắt xảy ra sẽ làm chip treo vì nó quay về tìm bảng cũ của Bootloader.
