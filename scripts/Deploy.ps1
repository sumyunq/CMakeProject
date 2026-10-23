#requires -Version 7.0
$originalPath = $env:PATH
$qtEnvironment = @{}
foreach ($variable in @('QT_PLUGIN_PATH','QT_QPA_PLATFORM_PLUGIN_PATH','QT_QPA_PLATFORM')) {
    $qtEnvironment[$variable] = [Environment]::GetEnvironmentVariable($variable, 'Process')
}
try {
    . "$PSScriptRoot/Common.ps1"
    Import-CompilerEnvironment
    Set-Location -LiteralPath $script:RepoRoot
    $destination = "$script:RepoRoot/dist/Release-" + (Get-Date -Format 'yyyyMMdd-HHmmss')
    Invoke-LoggedCommand $script:LocalTools.cmake @('--install',"$script:RepoRoot/build/local-release",'--prefix',$destination) "$script:RepoRoot/build/verification/deploy-install.log"
    $dlls = @(Get-ChildItem -LiteralPath "$script:RepoRoot/build/deps/install/Release" -Recurse -Filter 'opencv_*.dll' -File)
    if ($dlls.Count -lt 3) { throw '缺少 OpenCV 运行库，停止部署。' }
    foreach ($dll in $dlls) { Copy-Item -LiteralPath $dll.FullName -Destination $destination -ErrorAction Stop }
    $deploymentTool = Join-Path $script:LocalTools.qtRoot 'bin/windeployqt.exe'
    Invoke-LoggedCommand $deploymentTool @('--release','--no-translations',"$destination/vision_viewer.exe") "$script:RepoRoot/build/verification/deploy-qt.log"
    New-Item -ItemType Directory -Force -Path "$destination/licenses" | Out-Null
    Copy-Item -LiteralPath "$script:RepoRoot/3rdparty/opencv/5.0.0/LICENSE" -Destination "$destination/licenses/OpenCV.txt"
    Copy-Item -LiteralPath "$script:RepoRoot/LICENSE" -Destination "$destination/licenses/LearningProject.txt"
    Copy-Item -LiteralPath "$script:RepoRoot/build/deps/vcpkg_installed/cvlab-x64-windows-static-md/share/spdlog/copyright" -Destination "$destination/licenses/spdlog.txt" -ErrorAction Stop
    $opencvLicenses = "$script:RepoRoot/build/deps/install/Release/etc/licenses"
    Copy-Item -LiteralPath $opencvLicenses -Destination "$destination/licenses/OpenCV-thirdparty" -Recurse -ErrorAction Stop
    $qtLicenses = Join-Path (Split-Path (Split-Path $script:LocalTools.qtRoot)) 'Licenses'
    if (-not (Test-Path -LiteralPath $qtLicenses)) { throw '找不到已安装 Qt 的许可证目录。' }
    New-Item -ItemType Directory -Force -Path "$destination/licenses/Qt" | Out-Null
    # Copy public notices only; never copy account/license-key files such as sdk_license.
    foreach ($notice in @('COPYING.txt','Copyright.txt','LICENSE','LICENSE.FDL','LICENSE.GPL3-EXCEPT')) {
        Copy-Item -LiteralPath (Join-Path $qtLicenses $notice) -Destination "$destination/licenses/Qt" -ErrorAction Stop
    }
    $env:PATH = "$env:SystemRoot/System32;$env:SystemRoot"
    foreach ($variable in @('QT_PLUGIN_PATH','QT_QPA_PLATFORM_PLUGIN_PATH','QT_QPA_PLATFORM')) {
        [Environment]::SetEnvironmentVariable($variable, $null, 'Process')
    }
    Invoke-LoggedCommand "$destination/environment_probe.exe" @() "$script:RepoRoot/build/verification/deployed-probe.log"
    Invoke-LoggedCommand "$destination/mat_demo.exe" @() "$script:RepoRoot/build/verification/deployed-mat.log"
    foreach ($lesson in 3..30) {
        $program = if ($lesson -le 13) { 'foundation_labs' } elseif ($lesson -le 23) { 'advanced_labs' } else { 'engineering_labs' }
        Invoke-LoggedCommand "$destination/$program.exe" @($lesson.ToString('00'),"$destination/course-output/$lesson") "$script:RepoRoot/build/verification/deployed-course-$lesson.log"
    }
    # This is the delivered interactive GUI, not a background helper; verify a real window.
    $viewer = Start-Process -FilePath "$destination/vision_viewer.exe" -WorkingDirectory $destination -WindowStyle Normal -PassThru
    try {
        if (-not $viewer.WaitForInputIdle(10000)) { throw '部署窗口初始化超时。' }
        $deadline = [DateTime]::UtcNow.AddSeconds(10)
        do { $viewer.Refresh(); if ($viewer.HasExited) { throw '部署窗口提前退出。' }; if ($viewer.MainWindowHandle -ne 0) { break }; Start-Sleep -Milliseconds 100 } while ([DateTime]::UtcNow -lt $deadline)
        if ($viewer.MainWindowHandle -eq 0) { throw '部署程序没有创建窗口。' }
        if (-not $viewer.CloseMainWindow()) { throw '无法正常关闭部署窗口。' }
        if (-not $viewer.WaitForExit(10000) -or $viewer.ExitCode -ne 0) { throw '部署窗口未正常退出。' }
    } finally {
        if (-not $viewer.HasExited) { $viewer.Kill(); $viewer.WaitForExit() }
    }
    Write-Output "Release 干净 PATH 验证通过: $destination"
    Set-Content -LiteralPath "$script:RepoRoot/build/verification/latest-deployment.txt" -Value $destination -Encoding utf8
} finally {
    $env:PATH = $originalPath
    foreach ($variable in $qtEnvironment.Keys) {
        [Environment]::SetEnvironmentVariable($variable, $qtEnvironment[$variable], 'Process')
    }
}
