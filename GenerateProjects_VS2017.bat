@echo off
cd /D "%~dp0"
call "Scripts\generate_project_files.bat" vs2017 d3d11
exit
