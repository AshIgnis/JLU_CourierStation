# JLU Courier Station Management System

## 项目简介

吉林大学驿站管理系统是一个用于管理用户、包裹和管理员操作的综合性系统。该系统支持用户登录、包裹管理、数据统计与预测等功能，旨在提高驿站的运营效率和用户体验。

---

## 功能模块

### 1. 用户操作系统

- **用户登录**：
  - 验证手机号和密码。
  - 登录成功后显示用户信息和待领取包裹。
  - 包裹状态为违禁品时，禁止取件并提示联系管理员。
- **用户注册**：
  - 支持新用户注册，手机号重复时要求重新输入。
- **包裹管理**：
  - 查询待领取包裹。
  - 支持取件操作，显示包裹费用和状态。
  - 包裹状态异常（如损坏或违禁品）时，提供相应提示。

---

### 2. 管理员操作系统

- **管理员登录**：
  - 验证管理员账号和密码。
  - 限制登录尝试次数，确保系统安全。
- **用户管理**：
  - 添加、查询、修改和删除用户信息。
- **包裹管理**：
  - 收件包裹和寄件包裹的添加、查询和删除。
- **数据统计与预测**：
  - 支持日报、周报、月报等统计功能。
  - 基于历史数据进行预测，优化存储空间利用率。
- **文件管理**：
  - 删除预测文件或清空所有预测文件。

---

## 文件结构

JLU_CourierStation/ ├── src/ │ ├── main.c # 主程序入口 │ ├── user.c # 用户操作相关功能 │ ├── admin.c # 管理员操作相关功能 │ ├── customer.c # 用户数据管理 │ ├── receivedPackage.c # 收件包裹管理 │ ├── sendPackage.c # 寄件包裹管理 │ ├── Data_prediction_and_statistics.py # 数据统计与预测脚本 │ ├── huise.c # 数据存储与加载 │ ├── warehouse_explosion_warning.c # 仓库预警功能 ├── include/ │ ├── structure.h # 数据结构定义 │ ├── user.h # 用户功能头文件 │ ├── admin.h # 管理员功能头文件 │ ├── fileOperations.h # 文件操作头文件 │ ├── receivedPackage.h # 收件包裹头文件 │ ├── sendPackage.h # 寄件包裹头文件 ├── output/ │ ├── received_packages.txt # 收件包裹数据文件 │ ├── customers.txt # 用户数据文件 │ ├── admin.txt # 管理员数据文件 ├── README.md # 项目说明文件  

## 使用说明  

### 1. 编译与运行  

- 使用 GCC 编译器：  
bash  
  gcc -o JLU_CourierStation src/*.c -I include/  
  运行程序：
  ./JLU_CourierStation  
  2. 数据文件  
  用户数据：output/customers.txt  

收件包裹数据：output/received_packages.txt  

管理员数据：output/admin.txt  
2. Python 脚本  
数据统计与预测功能依赖 Python 脚本 Data_prediction_and_statistics.py。  
确保已安装 Python 3.12 或更高版本。  
注意事项  
输入合法性：  
系统会验证用户输入的合法性（如手机号、密码长度等）。  
输入无效时会提示重新输入。  
文件操作：  
所有数据操作均会实时保存到文件中，确保数据持久化。  
违禁品处理：  
包裹状态为违禁品时，禁止用户取件，并提示联系管理员。  
贡献者  
组长：蓝镇 @haliludaxrfg  
组员：  
操歌 @caoge5524  
林钰博 @lin23333331  
屈熙宸 @AshIgnis  
许可证  
本项目仅供吉林大学课程设计使用，未经许可不得用于商业用途。  

欢迎贡献!  
2025.4.22
