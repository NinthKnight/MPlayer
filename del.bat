echo "清理项目开始"

echo "开始清理bin目录"
del /s /q bin
rd bin /s /q 

echo "开始清理Debug目录"
del /s /q "MPlayer/Debug/"
rd "MPlayer/Debug/" /s /q 

echo "开始清理Release目录"
del /s /q "MPlayer/Release/"
rd "MPlayer/Release/" /s /q 

echo "开始清理GeneratedFiles目录"
del /s /q "MPlayer/GeneratedFiles/"
rd "MPlayer/GeneratedFiles/" /s /q 


echo "清理结束"

pause