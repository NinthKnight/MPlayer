echo "������Ŀ��ʼ"

echo "��ʼ����binĿ¼"
del /s /q bin
rd bin /s /q 

echo "��ʼ����DebugĿ¼"
del /s /q "MPlayer/Debug/"
rd "MPlayer/Debug/" /s /q 

echo "��ʼ����ReleaseĿ¼"
del /s /q "MPlayer/Release/"
rd "MPlayer/Release/" /s /q 

echo "��ʼ����GeneratedFilesĿ¼"
del /s /q "MPlayer/GeneratedFiles/"
rd "MPlayer/GeneratedFiles/" /s /q 


echo "�������"

pause