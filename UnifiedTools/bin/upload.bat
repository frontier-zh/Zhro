ping 		-n 2 10.10.10.15 > nul
echo		"上传采集客户端"
net 		use \\10.10.10.15\运维部\share\新采集-工具文档\tool\UnifiedTools /user:"opt" "cityre_n_opt."
xcopy  	D:\cygwin\home\root\frontierer_zh\Zhro\UnifiedTools\bin\UnifiedTools.exe  "\\10.10.10.15\运维部\share\新采集-工具文档\tool\UnifiedTools" /Yes
net 		use del /Yes
echo		"上传服务器完成"