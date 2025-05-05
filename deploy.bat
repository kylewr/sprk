For /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%a-%%b-%%c)
For /f "tokens=1-2 delims=/:" %%a in ('time /t') do (set mytime=%%a:%%b)
echo %mydate% %mytime% > ./robot/time.txt
ssh pi@10.8.62.2 sudo rm -rf /home/pi/robot
scp -r ./robot pi@10.8.62.2:/home/pi/robot
@REM ssh pi@10.8.62.2 r