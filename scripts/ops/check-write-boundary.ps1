# check-write-boundary.ps1
# Version: 2.0.0 | 2026-04-07
# Fix: UTF-8 stdin for PowerShell 5.1 on non-English Windows (CP950/GBK/Shift-JIS)
# Source: Harness Engineering / packs / ops-pack / scripts
# Sync: All project copies must match this version hash.

param(
    [string]$PolicyPath = ""
)

Set-StrictMode -Version 2.0
$ErrorActionPreference = "Stop"

function New-DecisionPayload {
    param(
        [string]$Decision,
        [string]$Reason,
        [string]$SystemMessage = ""
    )

    $hookOutput = @{
        hookSpecificOutput = @{
            hookEventName = "PreToolUse"
            permissionDecision = $Decision
        }
    }

    if ($Reason) {
        $hookOutput.hookSpecificOutput.permissionDecisionReason = $Reason
    }

    if ($SystemMessage) {
        $hookOutput.systemMessage = $SystemMessage
    }

    return ($hookOutput | ConvertTo-Json -Depth 10 -Compress)
}

function Normalize-PolicyRoot {
    param([string]$Value)

    if (-not $Value) {
        return ""
    }

    $normalized = $Value.Trim().Replace("\", "/")
    $normalized = $normalized.TrimStart(".")
    $normalized = $normalized.TrimStart("/")

    if (-not $normalized) {
        return ""
    }

    if (-not $normalized.EndsWith("/")) {
        $normalized += "/"
    }

    return $normalized
}

function Test-UnderPolicyRoots {
    param(
        [string]$RelativePath,
        [object[]]$Roots
    )

    foreach ($root in $Roots) {
        $normalizedRoot = Normalize-PolicyRoot ([string]$root)
        if (-not $normalizedRoot) {
            continue
        }

        $normalizedFile = $RelativePath.Replace("\", "/")        $normalizedFile = $normalizedFile.TrimStart(".").TrimStart("/")        if ($normalizedFile.StartsWith($normalizedRoot, [System.StringComparison]::OrdinalIgnoreCase)) {
            return $true
        }

        if ($normalizedFile.Equals($normalizedRoot.TrimEnd("/"), [System.StringComparison]::OrdinalIgnoreCase)) {
            return $true
        }
    }

    return $false
}

function Resolve-FullPathSafe {
    param(
        [string]$PathValue,
        [string]$WorkspaceRoot
    )

    if (-not $PathValue) {
        return $null
    }

    $candidate = $PathValue.Trim()
    if (-not $candidate) {
        return $null
    }

    $candidate = $candidate.Replace("/", "\")

    try {
        if ([System.IO.Path]::IsPathRooted($candidate)) {
            return [System.IO.Path]::GetFullPath($candidate).TrimEnd("\\")
        }

        return [System.IO.Path]::GetFullPath((Join-Path $WorkspaceRoot $candidate)).TrimEnd("\\")
    }
    catch {
        return $null
    }
}

function Convert-ToRelativeWorkspacePath {
    param(
        [string]$FullPath,
        [string]$WorkspaceRoot
    )

    $normalizedRoot = [System.IO.Path]::GetFullPath($WorkspaceRoot).TrimEnd("\\")
    $normalizedFile = [System.IO.Path]::GetFullPath($FullPath).TrimEnd("\\")

    if (-not $normalizedFile.StartsWith($normalizedRoot, [System.StringComparison]::OrdinalIgnoreCase)) {
        return $null
    }

    $relative = $normalizedFile.Substring($normalizedRoot.Length).TrimStart("\\")
    return $relative.Replace("\", "/")
}

function Add-StringPath {
    param(
        [System.Collections.ArrayList]$Collector,
        [object]$Value
    )

    if ($Value -is [string] -and $Value.Trim()) {
        [void]$Collector.Add($Value.Trim())
    }
}

function Extract-PatchPaths {
    param([string]$PatchText)

    $matches = [regex]::Matches($PatchText, "(?m)^\*\*\* (?:Add|Update|Delete) File: (.+?)(?: -> .+)?$")
    $paths = New-Object System.Collections.ArrayList

    foreach ($match in $matches) {
        [void]$paths.Add($match.Groups[1].Value.Trim())
    }

    return $paths
}

function Collect-ToolPaths {
    param(
        [object]$Node,
        [System.Collections.ArrayList]$Collector
    )

    if ($null -eq $Node) {
        return
    }

    if ($Node -is [string]) {
        return
    }

    if ($Node -is [System.Collections.IEnumerable] -and -not ($Node -is [string])) {
        foreach ($item in $Node) {
            Collect-ToolPaths -Node $item -Collector $Collector
        }
        return
    }

    $pathPropertyNames = @(
        "filePath",
        "filePaths",
        "dirPath",
        "path",
        "paths",
        "targetPath",
        "sourcePath",
        "destinationPath"
    )

    foreach ($propertyName in $pathPropertyNames) {
        if ($Node.PSObject.Properties.Name -contains $propertyName) {
            $value = $Node.$propertyName
            if ($value -is [string]) {
                Add-StringPath -Collector $Collector -Value $value
            }
            elseif ($value -is [System.Collections.IEnumerable]) {
                foreach ($item in $value) {
                    Add-StringPath -Collector $Collector -Value $item
                }
            }
        }
    }

    if ($Node.PSObject.Properties.Name -contains "input" -and $Node.input -is [string]) {
        foreach ($path in (Extract-PatchPaths -PatchText $Node.input)) {
            Add-StringPath -Collector $Collector -Value $path
        }
    }

    foreach ($property in $Node.PSObject.Properties) {
        $value = $property.Value
        if ($value -is [pscustomobject]) {
            Collect-ToolPaths -Node $value -Collector $Collector
        }
        elseif ($value -is [System.Collections.IEnumerable] -and -not ($value -is [string])) {
            foreach ($item in $value) {
                if ($item -is [pscustomobject] -or ($item -is [System.Collections.IEnumerable] -and -not ($item -is [string]))) {
                    Collect-ToolPaths -Node $item -Collector $Collector
                }
            }
        }
    }
}

function Test-IsWriteTool {
    param(
        [string]$ToolName,
        [object]$ToolInput
    )

    if (-not $ToolName) {
        return $false
    }

    $lowerName = $ToolName.ToLowerInvariant()
    $writeSignals = @("apply_patch", "create", "edit", "write", "replace", "rename", "delete", "move", "mkdir")

    foreach ($signal in $writeSignals) {
        if ($lowerName.Contains($signal)) {
            return $true
        }
    }

    if ($ToolInput -and $ToolInput.PSObject.Properties.Name -contains "input") {
        if ($ToolInput.input -is [string] -and $ToolInput.input -match "\*\*\* (Add|Update|Delete) File:") {
            return $true
        }
    }

    return $false
}

try {
    # Read stdin with explicit UTF-8 encoding.
    # VS Code sends UTF-8 JSON but PowerShell 5.1 defaults to system locale (CP950),
    # which corrupts multi-byte characters (e.g. Chinese in explanation/goal fields)
    # and causes ConvertFrom-Json to fail.
    $stdinStream = [Console]::OpenStandardInput()
    $reader = New-Object System.IO.StreamReader($stdinStream, [System.Text.Encoding]::UTF8)
    $rawInput = $reader.ReadToEnd()
    $reader.Close()
    if (-not $rawInput) {
        Write-Output (New-DecisionPayload -Decision "allow" -Reason "")
        exit 0
    }

    $hookInput = $rawInput | ConvertFrom-Json
    $workspaceRoot = [string]$hookInput.cwd

    if (-not $workspaceRoot) {
        Write-Output (New-DecisionPayload -Decision "ask" -Reason "Workspace root is missing from hook input.")
        exit 0
    }

    if (-not (Test-IsWriteTool -ToolName ([string]$hookInput.tool_name) -ToolInput $hookInput.tool_input)) {
        Write-Output (New-DecisionPayload -Decision "allow" -Reason "")
        exit 0
    }

    if (-not $PolicyPath) {
        $PolicyPath = $env:OPS_WRITE_POLICY
    }

    if (-not $PolicyPath) {
        $PolicyPath = ".github/ops/write-boundary-policy.json"
    }

    if (-not [System.IO.Path]::IsPathRooted($PolicyPath)) {
        $PolicyPath = Join-Path $workspaceRoot $PolicyPath
    }

    if (-not (Test-Path $PolicyPath)) {
        Write-Output (New-DecisionPayload -Decision "ask" -Reason "Write boundary policy file is missing. Review the target path before allowing edits." -SystemMessage "Missing policy: $PolicyPath")
        exit 0
    }

    $policy = (Get-Content -Raw -Path $PolicyPath) | ConvertFrom-Json
    $allowedWriteRoots = @($policy.allowedWriteRoots)
    $readonlyRoots = @($policy.readonlyRoots)
    $backupRequiredRoots = @($policy.backupRequiredRoots)
    $backupRequiredExtensions = @($policy.backupRequiredExtensions)
    $confirmOutsideAllowed = $true

    if ($null -ne $policy.confirmOutsideAllowed) {
        $confirmOutsideAllowed = [bool]$policy.confirmOutsideAllowed
    }

    $rawPaths = New-Object System.Collections.ArrayList
    Collect-ToolPaths -Node $hookInput.tool_input -Collector $rawPaths

    $candidatePaths = @($rawPaths | Select-Object -Unique)
    if ($candidatePaths.Count -eq 0) {
        Write-Output (New-DecisionPayload -Decision "allow" -Reason "")
        exit 0
    }

    $decision = "allow"
    $reasons = New-Object System.Collections.ArrayList

    foreach ($candidatePath in $candidatePaths) {
        $fullPath = Resolve-FullPathSafe -PathValue $candidatePath -WorkspaceRoot $workspaceRoot
        if (-not $fullPath) {
            continue
        }

        $relativePath = Convert-ToRelativeWorkspacePath -FullPath $fullPath -WorkspaceRoot $workspaceRoot
        if (-not $relativePath) {
            $decision = "ask"
            [void]$reasons.Add("Path outside workspace: $candidatePath")
            continue
        }

        if (Test-UnderPolicyRoots -RelativePath $relativePath -Roots $readonlyRoots) {
            $decision = "deny"
            [void]$reasons.Add("Readonly path blocked: $relativePath")
            continue
        }

        if ($allowedWriteRoots.Count -gt 0 -and -not (Test-UnderPolicyRoots -RelativePath $relativePath -Roots $allowedWriteRoots)) {
            if ($confirmOutsideAllowed -and $decision -ne "deny") {
                $decision = "ask"
                [void]$reasons.Add("Path outside allowed write roots: $relativePath")
            }
        }

        # Backup check only applies when file is NOT in allowedWriteRoots
        $isInAllowedRoot = ($allowedWriteRoots.Count -gt 0 -and (Test-UnderPolicyRoots -RelativePath $relativePath -Roots $allowedWriteRoots))
        if (-not $isInAllowedRoot) {
            $extension = [System.IO.Path]::GetExtension($relativePath)
            $needsBackup = (Test-UnderPolicyRoots -RelativePath $relativePath -Roots $backupRequiredRoots)

            if (-not $needsBackup -and $extension) {
                foreach ($backupExtension in $backupRequiredExtensions) {
                    if ($extension.Equals([string]$backupExtension, [System.StringComparison]::OrdinalIgnoreCase)) {
                        $needsBackup = $true
                        break
                    }
                }
            }

            if ($needsBackup) {
                [void]$reasons.Add("Backup required before editing: $relativePath")
                if ($decision -eq "allow") {
                    $decision = "ask"
                }
            }
        }
    }

    $uniqueReasons = @($reasons | Select-Object -Unique)
    $reasonText = [string]::Join("; ", $uniqueReasons)
    Write-Output (New-DecisionPayload -Decision $decision -Reason $reasonText)
    exit 0
}
catch {
    Write-Output (New-DecisionPayload -Decision "allow" -Reason "Write-boundary hook error - auto-allowing." -SystemMessage $_.Exception.Message)
    exit 0
}
