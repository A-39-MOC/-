import serial
import mysql.connector
from mysql.connector import Error
import time
import keyboard  # 导入键盘监听模块

# 配置串口参数
SERIAL_PORT = 'COM4'  # 根据实际情况修改串口号
BAUD_RATE = 9600  # 根据实际情况修改波特率

# MySQL数据库配置
DB_CONFIG = {
    'host': 'localhost',  # 数据库主机地址
    'port': '3306',
    'database': 'gettemperature',  # 数据库名称
    'user': 'root',  # 数据库用户名
    'password': '1234'  # 数据库密码
}

def parse_temperature(data):
    ascii_str = ''.join(chr(int(byte, 16)) for byte in data.split())
    try:
        return float(ascii_str.strip())  # 移除换行符后转换为浮点数
    except ValueError:
        print(f"无法解析数据: {data}")
        return None

def create_connection():
    """ 创建数据库连接 """
    connection = None
    try:
        connection = mysql.connector.connect(**DB_CONFIG)
        if connection.is_connected():
            print("成功连接到MySQL数据库")
    except Error as e:
        print(f"Error: {e}")
    return connection

def insert_temperature(connection, did, celsius):
    """ 将温度数据和设备ID插入到数据库中 """
    query = "INSERT INTO temp (tid, date, time, celsius, did) VALUES (NULL, NOW(), NOW(), %s, %s);"
    try:
        cursor = connection.cursor()
        cursor.execute(query, (celsius, did))
        connection.commit()
        print(f"记录插入成功: 设备ID={did}, 温度={celsius}°C")
    except Error as e:
        print(f"Error: {e}")

def stop_collection():
    global stop_flag
    stop_flag = True
    print("检测到c键被按下，停止采集...")

def main():
    global stop_flag
    stop_flag = False

    print("欢迎使用月之暗面温度采集模块")
    # 提示用户输入设备ID
    did = input("请输入设备ID: ")

    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    connection = create_connection()

    if connection is not None:
        # 注册热键
        keyboard.add_hotkey('c', stop_collection)

        while not stop_flag:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').strip()
                hex_data = ' '.join(format(b, '02X') for b in line.encode('utf-8'))
                temp = parse_temperature(hex_data)
                if temp is not None:
                    insert_temperature(connection, did, temp)

            # 添加延迟以实现每1秒采集一次
            time.sleep(1)

    # 关闭串口和数据库连接
    ser.close()
    if connection is not None and connection.is_connected():
        connection.close()
        print("关闭MySQL数据库连接")

if __name__ == '__main__':
    main()
