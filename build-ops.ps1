param([string]$Sdk='D:\Program Files\Mastercam 2026\sdk',[string]$BuildDirectory='build')
$ErrorActionPreference='Stop'
$out=Join-Path $PSScriptRoot $BuildDirectory
New-Item -ItemType Directory -Force -Path $out | Out-Null
$interfaces=Join-Path $Sdk 'interfaces'
$includes=@($Sdk,$interfaces)+(Get-ChildItem -LiteralPath $interfaces -Directory -Recurse).FullName
$includes+=(Get-ChildItem -LiteralPath (Join-Path $Sdk 'NGP') -Directory -Recurse).FullName
$args=@('/nologo','/LD','/MD','/std:c++20','/EHsc','/DUNICODE','/D_UNICODE','/DNDEBUG')
$args+=$includes | ForEach-Object {'/I"'+$_+'"'}
$args+=@('"'+(Join-Path $PSScriptRoot 'ops.cpp')+'"','/Fo"'+(Join-Path $out 'level4_native.obj')+'"','/link','/OUT:"'+(Join-Path $out 'OpsProbe.dll')+'"','/IMPLIB:"'+(Join-Path $out 'OpsProbe.lib')+'"','/LIBPATH:"'+(Join-Path $Sdk 'x64\release')+'"','MCMill.lib','MCCore.lib','MCVerify.lib','MCMachineDef.lib','Mastercam.lib')
$rsp=Join-Path $out 'compile.rsp'
[IO.File]::WriteAllLines($rsp,$args,[Text.Encoding]::Default)
$dev='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat'
$batch=Join-Path $out 'compile.cmd'
[IO.File]::WriteAllLines($batch,@('@echo off',('call "'+$dev+'" >nul'),('cl.exe @"'+$rsp+'"')),[Text.Encoding]::Default)
& cmd.exe /d /c $batch
if($LASTEXITCODE -ne 0){throw "Compiler failed: $LASTEXITCODE"}
Get-Item -LiteralPath (Join-Path $out 'OpsProbe.dll') | Select-Object FullName,Length



