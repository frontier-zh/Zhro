ping 		-n 2 10.10.10.15 > nul
echo		"�ϴ��ɼ��ͻ���"
net 		use \\10.10.10.15\��ά��\share\�²ɼ�-�����ĵ�\tool\UnifiedTools /user:"opt" "cityre_n_opt."
xcopy  	D:\cygwin\home\root\frontierer_zh\Zhro\UnifiedTools\bin\UnifiedTools.exe  "\\10.10.10.15\��ά��\share\�²ɼ�-�����ĵ�\tool\UnifiedTools" /Yes
net 		use del /Yes
echo		"�ϴ����������"