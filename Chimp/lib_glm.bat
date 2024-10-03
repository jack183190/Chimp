@echo off

REM Check if a tag argument was provided
if "%~1"=="" (
    echo Usage: %0 [tag]
    exit /b 1
)

set TAG=%~1

if not exist "vendor" (
    mkdir vendor
)
cd vendor
git clone https://github.com/g-truc/glm.git glm
cd glm || exit /b 1

git checkout %TAG%
exit /b 0