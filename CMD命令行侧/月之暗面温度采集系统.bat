  @echo off
:start
cls
color 1a
echo -----------------------------------------
echo     ��ӭʹ����֮�����¶Ȳɼ�ϵͳ����
echo -----------------------------------------
echo            1.������ϵͳģ��
echo            2.����ɼ�ģ��
echo            3.����ǰ��ҳ��ģ��
echo            0.�˳�ϵͳ
set /p var=��ѡ����Ҫ���еĲ�����
if /i "%var%"=="" goto start
if /i %var%==1 goto a
if /i %var%==2 (goto b)
if /i %var%==0 goto c else (echo û�����ѡ������ԣ�~&pause)
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