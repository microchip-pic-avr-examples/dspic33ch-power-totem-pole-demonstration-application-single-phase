@echo off

:start
cls
echo.
echo ######################  DOXYGEN OUTPUT GENERATION  ######################
echo.
doxygen -v
echo.

set startdir=%~dp0
cd %startdir%
cd ..\..\
set rootdir=%cd%
set rootimg=%rootdir%\images

set workdir=%rootdir%\.mchp_private\doxygen
set usrimg=%workdir%\config\images
set mdimg=%workdir%\config\markdown-files\images
set doxydir=%workdir%\templates
set doxyimg=%workdir%\templates\images
set doxysearch=%workdir%\templates\search

set outputdir=%rootdir%\docs
set searchdir=%outputdir%\search
set outputimg=%outputdir%\images
set logfile=%workdir%\doxylog.txt
set doxytemplate=%startdir%doxytemplate
set layouttemplate=%startdir%layout.xml

set autoclose=1

if exist %workdir% goto doxconfig_present
set workdir=%rootdir%\.mchp-private\doxygen
if exist %workdir% goto doxconfig_present
set workdir=%rootdir%\mchp_private\doxygen
if exist %workdir% goto doxconfig_present
set workdir=%rootdir%\mchp-private\doxygen
if exist %workdir% goto doxconfig_present

echo.
echo Critical Error: 
echo No Doxygen configuration could be found.
echo.
echo Doygen configuration and templates are expected to be located in one of 
echo the following sub-directory name examples:
echo.
echo   - .mchp_private\doxygen
echo   - .mchp-private\doxygen
echo   -  mchp_private\doxygen
echo   -  mchp-private\doxygen
echo.
echo Script execution aborted.
echo.
goto end

:doxconfig_present
echo Working Directories: 
echo ====================
echo.
echo Start Directory:               %startdir%
echo Source directory:              %workdir%
echo Doxyfile location:             %doxydir%
echo Content images directory:      %doxyimg%
echo Markdown images directory:     %mdimg%
echo Main images directory:         %rootimg%
echo Output directory:              %outputdir%
echo Output images directory:       %outputimg%
echo Output search directory:       %searchdir%
echo Doxygen generator log file:    %logfile%
echo. 

echo Toolchain Information:
echo ======================
echo.
echo Doxygen Version:
doxygen -v
echo. 

rem To generate graphics like call graphs and flow charts, the Graphviz package needs to be installed.
rem You can find this package here: https://www.graphviz.org/download/
rem Please make sure the installation directory of Graphviz is registered as PATH variable
rem Should doxygen still have trouble with calling the Dot tool, execute the following line 
rem to make sure all library packages are properly registered 
echo Graphviz Version:
dot -V
echo.

:list_options
echo User Options:
echo =============
echo.
echo Press [L] to generate a layout template file
echo Press [T] to generate a template configuration file
echo Press [U] to update the most recently selected doxyfile to support installed version of Doxygen
echo.
echo Press [D] to switch to Doxygen command line help (leaves this program)
echo Press [E] to end the program
echo.
set yesno="n"
set /P yesno="Enter Selection: "
echo.
if "%yesno%"=="d" ( goto show_doxygen_options )
if "%yesno%"=="D" ( goto show_doxygen_options )
if "%yesno%"=="e" ( goto end )
if "%yesno%"=="E" ( goto end )
if "%yesno%"=="t" ( goto generate_config_template )
if "%yesno%"=="T" ( goto generate_config_template ) 
if "%yesno%"=="l" ( goto generate_layout_template )
if "%yesno%"=="L" ( goto generate_layout_template ) 
if "%yesno%"=="u" ( goto update_doxyfile )
if "%yesno%"=="U" ( goto update_doxyfile ) else ( set yesno="n" )
goto end

:update_doxyfile
cls
echo Doxyfile Update:
echo ================
echo.
echo -- updating Doxygen configuration file %doxyfile%
cd "%doxydir%"
doxygen -u doxyfile 
echo.
goto list_options

:generate_config_template
cls
echo Generate Doxyfile Template:
echo ===========================
echo.
echo -- generate Doxygen configuration template %doxytemplate%
cd "%startdir%"
doxygen -g %doxytemplate%
echo.
goto list_options

:generate_layout_template
cls
echo Generate Doxygen Layout Template:
echo =================================
echo.
echo -- generate Doxygen layout template %layouttemplate%
cd "%startdir%"
doxygen -l %layouttemplate%
echo.
goto list_options

:show_doxygen_options
cls
echo Show all Doxygen Command Line Switches:
echo =======================================
echo.
doxygen -?
echo.
set autoclose=0
echo %autoclose%
goto end

:end
echo.
echo ################################  E N D  ################################
echo.
if %autoclose%==0 ( cmd /k ) else ( @timeout 10 )

