# Arduino_X033
## Giới thiệu
Board Arduino nhỏ gọn, sử dụng vi điều khiển CH32X033F8P6
* MCU: RISC-V 32 bit, 48MHz
* Flash: 62KB (bootloader: 8KB, appz: 54KB)
* RAM: 20KB
* SPI
* UART
* ...
  
USB bulk, tự nhận driver, không cần cài đặt (từ windows 10 trở lên không cần cài đặt driver, từ windows 7 trở xuống cần cài driver winusb)
## Sử dụng
### Hoạt động của bootloader:
Khi cấp nguồn nếu nút user được nhấn, mạch vào chế độ nạp chương trình, nếu nút không được nhấn thì chạy ứng dụng. Vì vậy muốn nạp lại chương trình có 2 cách:
* Cách 1: Rút nguồn, ấn và giữ nút user, cấp nguồn, nhả nút user.
* Cách 2: Ấn và giữ nút reset, ấn và giữ nút user, nhả nút reset, nhả nút user.
### Cài đặt board Arduino X033:
Thêm chuỗi json vào arduino https://raw.githubusercontent.com/NgoHungCuong/Arduino_X033/main/board/package_ngohungcuong_index.json

Đối với Linux:

Cần thêm rules để cho phép quyền truy cập USB.

https://github.com/NgoHungCuong/Arduino_X033/tree/main/driver
