  @echo off
:start
cls
color 1a
echo -----------------------------------------
echo     欢迎使用月之暗面温度采集系统导航
echo -----------------------------------------
echo            1.进入主系统模块
echo            2.进入采集模块
echo            3.进入前端页面模块
echo            0.退出系统
set /p var=请选择您要进行的操作：
if /i "%var%"=="" goto start
if /i %var%==1 goto a
if /i %var%==2 (goto b)
if /i %var%==0 goto c else (echo 没有这个选项，请重试！~&pause)
goto start
:a
start darkofmoon.exe
pause
goto start
:b
python wd.py
pause
goto start
:c
&exit
@pause