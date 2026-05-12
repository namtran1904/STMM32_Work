graph TD
    A[Power ON / Reset] --> B[CPU tra cứu 0x08000000]
    B --> C[Chạy code BOOTLOADER]
    C --> D[Nháy LED chậm 3 cái]
    D --> E{Kiểm tra App tại 0x08008000}
    E -- Không hợp lệ --> F[Treo tại while-1 Bootloader]
    E -- Hợp lệ --> G[Khóa ngắt & Nạp MSP mới]
    G --> H[Nhảy vào Reset_Handler của App]
    H --> I[Chạy code APPLICATION]
    I --> J[Thiết lập SCB->VTOR = 0x08008000]
    J --> K[Nháy LED nhanh liên tục]
