@echo off

:start
cls
echo.
echo ######################  DOXYGEN OUTPUT GENERATION  ######################
echo.
doxygen -v
echo.

set startdir="%~dp0"
cd /d %startdir%
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

echo Working Directories: 
echo ====================
echo.
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

echo Output Generation:
echo ==================
echo.
echo -- preparing output generation...
rem Checking if the output directory already exists or needs to be created
echo -- searching output directory 
if exist %outputdir% echo -- output directory present
if exist %outputdir% goto docs_present
echo -- no output directory found

:create_docs
mkdir "%outputdir%"
echo -- created new output directory 'docs'
set /A new_docs=1
goto switch_to_docs

:docs_present
set /A new_docs=0
echo -- found output directory 'docs'

:switch_to_docs
cd "%outputdir%"
if "%new_docs%"=="1" goto skip_delete_images

:get_options
echo.
echo Option 1/4: Clear the output directory before generating new content (recommended)?
echo.
echo     Y = Yes        deletes the content of the output directory and the complete content will be generated
echo     N = No         keeps all content of the output directory and only changed files will be updated
echo     E = Exit       skips the code generation and ends the program
echo.
set /P yesno="Enter Selection: "
echo.
if "%yesno%"=="e" (goto end)
if "%yesno%"=="E" (goto end)
if "%yesno%"=="Y" (goto delete_output)
if "%yesno%"=="y" (goto delete_output) else (set yesno="n")
echo -- selection: (%yesno%)
echo -- output director contents will not be cleared
goto skip_delete_output

:delete_output
echo -- clearing output directory - please wait...
del /F /Q *.*
if exist "%searchdir%" del /F /Q "%searchdir%\*.*"
echo -- output directory cleared

:skip_delete_output
echo.
echo Option 2/4: Clear the 'images' directory of the doxygen output directory before generating new content (recommended)?
echo.
echo     Y = Yes        deletes the content of the 'images' sub-directory in the doxygen output directory
echo     N = No         keeps all content of the 'images' sub-directory in the doxygen output directory 
echo                    and only changed files will be updated
echo     E = Exit       skips the code generation and ends the program
echo.
set yesno="n"
set /P yesno="Enter Selection: "
echo.
if "%yesno%"=="e" (goto end)
if "%yesno%"=="E" (goto end)
if "%yesno%"=="Y" (goto delete_images)
if "%yesno%"=="y" (goto delete_images) else (set yesno="n")
echo -- selection: (%yesno%)
echo -- output images directory will not be cleared
goto skip_delete_images

:delete_images
echo -- clearing images directory - please wait...
if exist "%outputimg%" del /F /Q "%outputimg%\*.*"
echo -- images directory cleared

:skip_delete_images
echo.
echo Option 3/4: Open output in browser after generation?
echo.
echo     Y = Yes        the generated output will be opened in the default browser
echo     N = No         the program ends after content has been generated
echo     E = Exit       skips the code generation and ends the program
echo.
set yesno="n"
set /P yesno="Enter Selection: "
echo.
if "%yesno%"=="e" ( goto end )
if "%yesno%"=="E" ( goto end )
if "%yesno%"=="Y" ( goto generate_output )
if "%yesno%"=="y" ( goto generate_output ) else ( set yesno="n" )
echo -- selection: (%yesno%)
echo -- generated output will not be opened automatically

:generate_output
if "%yesno%"=="Y" ( set openbrowser="y" )
if "%yesno%"=="y" ( set openbrowser="y" ) else ( set openbrowser="n" )
if %openbrowser%=="y" ( echo -- generated output will be opened automatically after output generation )
echo. 
echo Option 4/4: Redirect Doxygen output messages to output file?
echo.
echo     Y = Yes        the generated output messages will be redirected to output log file
echo     N = No         the generated output messages will not be redirected to output log file
echo     E = Exit       skips the code generation and ends the program
echo.
set yesno="n"
set /P yesno="Enter Selection: "
echo.
set logoutput="n"
if "%yesno%"=="e" ( goto end )
if "%yesno%"=="E" ( goto end )
if "%yesno%"=="Y" ( set logoutput="y" )
if "%yesno%"=="y" ( set logoutput="y" ) else ( set yesno="n" )
echo -- selection: (%yesno%)
echo -- generate output...
if exist "%logfile%" ( del /F /Q "%logfile%" ) 
type nul > "%logfile%"
cls
echo ########################  R U N   D O X Y G E N  ########################
echo. 
echo -- searching for doxygen path...
echo -- %doxydir%
echo -- setting doxygen path...
cd "%doxydir%"
set testdir=%cd%
echo -- calling doxygen path...
echo -- %testdir%
echo -- calling doxygen generator...
if %logoutput%=="y" ( goto generate_output_log )
echo -- generating output without message logging...
doxygen doxyfile
goto exit_generation

:generate_output_log
echo -- generating output with active message logging
echo -- please be patient, this can take a while...
doxygen doxyfile -d >"%logfile%"

:exit_generation
echo ##############  D O X Y G E N   E X E C U T I O N   E N D  ##############
echo. 
echo -- merging template image folder content into output image directory
Xcopy /e /i /c /r /y "%rootimg%" "%outputimg%"
if exist "%mdimg%"      ( Xcopy /e /i /c /r /y "%mdimg%" "%outputimg%" )
if exist "%userimg%"    ( Xcopy /e /i /c /r /y "%userimg%" "%outputimg%" )
if exist "%doxyimg%"    ( Xcopy /e /i /c /r /y "%doxyimg%" "%outputimg%" )
if exist "%doxysearch%" ( Xcopy /e /i /c /r /y "%doxysearch%" "%searchdir%" )
echo. 
echo -- content generation complete
echo. 
echo ###############################  D O N E  ###############################
echo. 
if %openbrowser%=="y" ( goto open_output )
echo. 
echo -- 'Open Output after generation' option skipped by user (%openbrowser%)
goto end

:open_output
echo -- open output (%openbrowser%)
%outputdir%\index.html
rem if %logoutput%=="y" %logfile%

:end
echo ################################  E N D  ################################
@timeout 10

