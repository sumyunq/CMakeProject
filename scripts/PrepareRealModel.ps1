#requires -Version 7.0
# Updated: 2026-11-02 11:12:46 +08:00 wbliu
$ErrorActionPreference = 'Stop'
$root = Split-Path $PSScriptRoot -Parent
$destination = Join-Path $root 'build/models/squeezenet1.1-7.onnx'
$expected = '1eeff551a67ae8d565ca33b572fc4b66e3ef357b0eb2863bb9ff47a918cc4088'
$url = 'https://media.githubusercontent.com/media/onnx/models/b1eeaa1ac722dcc1cd1a8284bde34393dab61c3d/validated/vision/classification/squeezenet/model/squeezenet1.1-7.onnx'
New-Item -ItemType Directory -Force -Path (Split-Path $destination) | Out-Null
if (Test-Path -LiteralPath $destination) {
    if ((Get-FileHash -Algorithm SHA256 -LiteralPath $destination).Hash -ne $expected) { throw '模型 SHA-256 不匹配，请检查本地文件。' }
    Write-Host "模型校验通过: $destination"
    return
}
$temp = Join-Path (Split-Path $destination) ([guid]::NewGuid().ToString() + '.download')
try {
    Invoke-WebRequest -Uri $url -OutFile $temp
    if ((Get-FileHash -Algorithm SHA256 -LiteralPath $temp).Hash -ne $expected) { throw '下载的模型 SHA-256 不匹配。' }
    Move-Item -LiteralPath $temp -Destination $destination
} finally {
    if (Test-Path -LiteralPath $temp) { Remove-Item -LiteralPath $temp }
}
Write-Host "模型下载并校验通过: $destination"
