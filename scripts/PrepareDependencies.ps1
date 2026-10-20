#requires -Version 7.0
param([ValidateSet('Debug','Release','Both')][string]$Configuration = 'Both')
. "$PSScriptRoot/Common.ps1"
Import-CompilerEnvironment
Set-Location -LiteralPath $script:RepoRoot
$sourcePath = Join-Path $script:RepoRoot '3rdparty/opencv/5.0.0'
$expectedCommit = '40738fb16ceddb5fb3fea747585f7ce6abb0605b'
if (-not (Test-Path -LiteralPath "$sourcePath/.git")) {
    New-Item -ItemType Directory -Force -Path (Split-Path $sourcePath) | Out-Null
    Invoke-LoggedCommand 'git' @('-c','http.lowSpeedLimit=1024','-c','http.lowSpeedTime=60','clone','--filter=blob:none','--sparse','--depth','1','--branch','5.0.0','--single-branch','https://github.com/opencv/opencv.git',$sourcePath) "$script:RepoRoot/build/verification/opencv-download.log"
}
Invoke-LoggedCommand 'git' @('-C',$sourcePath,'sparse-checkout','set','--skip-checks','cmake','3rdparty','include','doc/CMakeLists.txt','docs_sphinx/CMakeLists.txt','modules/core','modules/flann','modules/geometry','modules/imgproc','modules/imgcodecs') "$script:RepoRoot/build/verification/opencv-checkout.log"
$sourceCommit = & git -C $sourcePath rev-parse HEAD
if ($LASTEXITCODE -ne 0 -or $sourceCommit -ne $expectedCommit) { throw "OpenCV 源码提交不匹配: $sourceCommit" }
$sourceChanges = & git -C $sourcePath status --porcelain
if ($LASTEXITCODE -ne 0 -or $sourceChanges) { throw '依赖源码不干净，停止构建。' }
$versionHeader = Get-Content -LiteralPath "$sourcePath/modules/core/include/opencv2/core/version.hpp" -Raw
foreach ($definition in @('CV_VERSION_MAJOR\s+5','CV_VERSION_MINOR\s+0','CV_VERSION_REVISION\s+0','CV_VERSION_STATUS\s+""')) {
    if ($versionHeader -notmatch $definition) { throw "源码版本头不匹配: $definition" }
}
$configurations = if ($Configuration -eq 'Both') { @('Debug','Release') } else { @($Configuration) }
foreach ($buildConfiguration in $configurations) {
    $buildPath = "$script:RepoRoot/build/deps/opencv-$buildConfiguration"
    $installPath = "$script:RepoRoot/build/deps/install/$buildConfiguration"
    $options = @('-S',$sourcePath,'-B',$buildPath,'-G','Ninja',"-DCMAKE_MAKE_PROGRAM=$($script:LocalTools.ninja)","-DCMAKE_BUILD_TYPE=$buildConfiguration",'-DCMAKE_CXX_STANDARD=20','-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded$<$<CONFIG:Debug>:Debug>DLL',"-DCMAKE_INSTALL_PREFIX=$installPath",'-DBUILD_LIST=core,imgproc,imgcodecs','-DBUILD_DOCS=OFF','-DBUILD_SHARED_LIBS=ON','-DBUILD_TESTS=OFF','-DBUILD_PERF_TESTS=OFF','-DBUILD_EXAMPLES=OFF','-DBUILD_opencv_apps=OFF','-DBUILD_opencv_python2=OFF','-DBUILD_opencv_python3=OFF','-DWITH_QT=OFF','-DWITH_CUDA=OFF','-DWITH_OPENCL=OFF','-DWITH_IPP=OFF','-DWITH_FFMPEG=OFF','-DWITH_VTK=OFF','-DWITH_OPENEXR=OFF','-DWITH_JASPER=OFF','-DWITH_WEBP=OFF','-DWITH_TIFF=OFF','-DWITH_JPEG=ON','-DWITH_PNG=ON','-DBUILD_JPEG=ON','-DBUILD_PNG=ON','-DBUILD_ZLIB=ON')
    # OpenCV 5.0.0 geometry uses M_PI; MSVC requires this opt-in before math headers.
    $options += '-DOPENCV_EXTRA_CXX_FLAGS=/D_USE_MATH_DEFINES'
    Invoke-LoggedCommand $script:LocalTools.cmake $options "$script:RepoRoot/build/verification/opencv-$buildConfiguration-configure.log"
    Invoke-LoggedCommand $script:LocalTools.cmake @('--build',$buildPath,'--parallel','4') "$script:RepoRoot/build/verification/opencv-$buildConfiguration-build.log"
    Invoke-LoggedCommand $script:LocalTools.cmake @('--install',$buildPath) "$script:RepoRoot/build/verification/opencv-$buildConfiguration-install.log"
    @{ version='5.0.0'; commit=$sourceCommit; configuration=$buildConfiguration; options=$options; compiler=$env:VCToolsVersion; modules=@('core','imgproc','imgcodecs') } | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath "$installPath/cvlab-build.json" -Encoding utf8
    Write-Output "依赖安装完成: $buildConfiguration"
}
