@set CONFIG_TYPE=debug
REM @set CONFIG_TYPE=release

@call "%VS90COMNTOOLS%vsvars32.bat"
@set CL=/D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE
@set LINK=/LARGEADDRESSAWARE

PATH=V:\windows\x32\xip-0.2.2-install\bin\debug;V:/windows/x32/MITK_VC9.0_Qt4.6.2_Bin/bin\%CONFIG_TYPE%;V:/windows/x32/VTK-5.4.2_VC9.0_Bin/bin\%CONFIG_TYPE%;V:/windows/x32/ITK-3.16.0_VC9.0_Bin/bin\%CONFIG_TYPE%;C:/opt/windows/toolkits/windows/x32/Qt-4.6.2/lib\..\bin;V:/windows/x32/MITK_VC9.0_Qt4.6.2_Bin/bin/BlueBerry\org.blueberry.osgi\bin\%CONFIG_TYPE%;%PATH%

"../../binSimpleXIP/SimpleXIP.sln"

