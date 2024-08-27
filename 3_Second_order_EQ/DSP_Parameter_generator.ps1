


$outputFile = "./SigmaDSP_parameters.h"
$lf = [System.Environment]::NewLine

function Find-File ($pattern) {
    $file = Get-ChildItem -Recurse $pattern
    if ($null -eq $file) {
        Write-Host -ForegroundColor Red "$pattern not found"
        exit
    }
    if ($file -is [system.array]) {
        Write-Host -ForegroundColor Red "Multiple files found for $pattern"
        exit
    }
    return $file
}

function Find-OptionalFile ($pattern) {
    $file = Get-ChildItem -Recurse $pattern
    if ($null -eq $file) {
        Write-Host -ForegroundColor Yellow "WARNING! $pattern not found"
    }
    if ($file -is [system.array]) {
        Write-Host -ForegroundColor Red "Multiple files found for $pattern"
        exit
    }
    return $file
}

$dspParamFile = Find-File "*_IC_1_PARAM.h"
$dspParamContent = Get-Content $dspParamFile

$eepromHexFile = Find-OptionalFile "*rom.hex"
if ($null -ne $eepromHexFile) {
    $eepromHexContent = Get-Content $eepromHexFile
}
else {
    Write-Host -ForegroundColor Yellow "EEPROM loading not possible"
}

$dspProgramFile = Find-File "*_IC_1.h"
$dspProgramContent = Get-Content $dspProgramFile

$eepromProgramFile = Find-OptionalFile "*_IC_2.h"
if ($null -ne $eepromProgramFile) {
    $eepromProgramContent = Get-Content $eepromProgramFile
}

# Skip EEPROM array if file is not present
if ($null -eq $eepromHexFile) {
    $hexContent = ""
}
else {
    # Generate C style array out from passed hex file
    $arrayLen = $eepromHexContent.Split(",", [System.StringSplitOptions]::RemoveEmptyEntries).Count
    $hexContent = $eepromHexContent -Join $lf | Out-String
    $hexContent = "/* This array contains the entire DSP program,
    and should be loaded into the external i2c EEPROM */
    const uint8_t PROGMEM DSP_eeprom_firmware[$arrayLen] = {`n$hexContent};"
}

# Extract relevant comments and macros
$paramContent = 
$dspParamContent | 
    Where-Object {
    $temp = $_.Split(' ')[1]; $_ -match "/* Module" -or $temp -match "_COUNT$" -or $temp -match "_FIXPT$" -or $temp -match "_VALUES$" -or $temp -match "_ADDR$"
}  

$paramContent = $paramContent -join $lf  | Out-String

# Track down the DSP and EEPROM i2c addresses
$addressLine = $dspProgramContent | 
    Where-Object {$_ -match "#define DEVICE_ADDR_IC_1" }
$hexNumberIndex = $addressLine.LastIndexOf("0x")

$dspProgAddress = $addressLine.Substring($hexNumberIndex, 4)

if ($null -ne $eepromProgramFile) {
    $addressLine = $eepromProgramContent | 
        Where-Object {$_ -match "#define DEVICE_ADDR_IC_2" }

    $hexNumberIndex = $addressLine.LastIndexOf("0x")
    $eepromProgAddress = $addressLine.Substring($hexNumberIndex, 4)
    $eepromProgAddressLine = "#define EEPROM_I2C_ADDRESS ($eepromProgAddress >> 1) & 0xFE"
}
else {
    $eepromProgAddressLine = ""
}

# Generate program data
$dspProgramContentSlice = $dspProgramContent[26..($dspProgramContent.length - 2)]

$dspProgramData = ""
foreach ($line in $dspProgramContentSlice) {
    $parts = $line.Split(" ", [System.StringSplitOptions]::RemoveEmptyEntries)
    $last = $parts[-1]

    if ($line -match "/* DSP" -or $line -match "/* Register") {
        $dspProgramData += "$line$lf"
    }

    if ($line -match "#define PROGRAM_SIZE_IC_1") {
        $dspProgramData += "#define PROGRAM_SIZE $last $lf"
    }

    if ($line -match "#define PROGRAM_ADDR_IC_1") {
        $dspProgramData += "
#define PROGRAM_ADDR $last $lf#define PROGRAM_REGSIZE 5 $lf"
    }

    if ($line -match "ADI_REG_TYPE Program_Data") {
        $dspProgramData += "const uint8_t PROGMEM DSP_program_data[PROGRAM_SIZE] = $lf{" 
    }

    if ($line -match "#define PARAM_SIZE_IC_1") {
        $dspProgramData += "#define PARAMETER_SIZE $last $lf"
    }

    if ($line -match "#define PARAM_ADDR_IC_1") {
        $dspProgramData += "#define PARAMETER_ADDR $last $lf#define PARAMETER_REGSIZE 4$lf"
    }

    if ($line -match "ADI_REG_TYPE Param_Data") {
        $dspProgramData += "const uint8_t PROGMEM DSP_parameter_data[PARAMETER_SIZE] = $lf{" 
    }

    if ($line -match "ADI_REG_TYPE R0_COREREGISTER") {
        $dspProgramData += "#define CORE_REGISTER_R0_SIZE 2$lf"
        $dspProgramData += "#define CORE_REGISTER_R0_ADDR 0x081C$lf"
        $dspProgramData += "#define CORE_REGISTER_R0_REGSIZE 2$lf"
        $dspProgramData += "const uint8_t PROGMEM DSP_core_register_R0_data[CORE_REGISTER_R0_SIZE] = $lf{$lf"        
    }

    if ($line -match "#define R3_HWCONFIGURATION_IC_1_SIZE") {
        $dspProgramData += "#define HARDWARE_CONF_SIZE $last$lf"
        $dspProgramData += "#define HARDWARE_CONF_ADDR 0x081C$lf"
        $dspProgramData += "#define HARDWARE_CONF_REGSIZE 1$lf$lf"
    }

    if ($line -match "ADI_REG_TYPE R3_HWCONFIGURATION") {
        $dspProgramData += "const uint8_t PROGMEM DSP_hardware_conf_data[HARDWARE_CONF_SIZE] = $lf{"
    }

    if ($line -match "ADI_REG_TYPE R4_COREREGISTER") {
        $dspProgramData += "#define CORE_REGISTER_R4_SIZE 2$lf"
        $dspProgramData += "#define CORE_REGISTER_R4_ADDR 0x081C$lf"
        $dspProgramData += "#define CORE_REGISTER_R4_REGSIZE 2$lf"
        $dspProgramData += "const uint8_t PROGMEM DSP_core_register_R4_data[CORE_REGISTER_R4_SIZE] = $lf{$lf"        
    }


    if ($line.StartsWith("0x")) {
        $dspProgramData += $line
    }

    if ($line.StartsWith("};")) {
        $dspProgramData += "};$lf$lf"
    }


}

$date = get-date -Format "dd.MM.yyyy HH.mm.ss"
$output = "#ifndef SIGMADSP_PARAMETERS_H
#define SIGMADSP_PARAMETERS_H

#include <SigmaDSP.h>

/****************************************************************************
| File name: SigmaDSP_parameters.h                                          |
| Generation tool: Powershell                                               |
| Date and time: $date                                        |
|                                                                           |
| ADAU1701 parameter and program file header                                |
| SigmaDSP library and its content is developed and maintained by MCUdude.  |
| https://github.com/MCUdude/SigmaDSP                                       |
|                                                                           |
| Huge thanks to the Aida DSP team who have reverse engineered a lot of the |
| Sigma DSP algorithms and made them open source and available to everyone. |
| This library would never have existed if it weren't for the Aida DSP team |
| and their incredible work.                                                |
|                                                                           |
| This file have been generated with the Sigmastudio_project_formatter.sh   |
| script. This file contains all the DSP function block parameters and      |
| addresses. It also contains the program that will be loaded to the        |
| external EEPROM.                                                          |
|                                                                           |
| The *_COUNT macro holds the number of addresses in memory each complete   |
| module takes.                                                             |
|                                                                           |
| The *_ADDR macro holds the current address for the module. Use this macro |
| when changing the behaviour of the modules (EQs, volume etc.).            |
|                                                                           |
| The *_FIXFT macros holds the default value of the module. Use this when   |
| restoring the default parameters.                                         |
|                                                                           |
| The DSP_eeprom_firmware[] array contains the DSP firmware, and can be     |
| loaded using the writeFirmware method in the DSPEEPROM class.             |
| When stored in the external i2c EEPROM, the firmware is automatically     |
| loaded into the DSP on boot if the SELFBOOT pin is tied to Vcc.           |
|                                                                           |
| If you want to load the DSP firmware directly without using an external   |
| EEPROM, you can simply run loadProgram() (located at the bottom of this   |
| file) where you pass the SigmaDSP object as the only parameter.           |
|                                                                           |
****************************************************************************/

/* 7-bit i2c addresses */
#define DSP_I2C_ADDRESS ($dspProgAddress >> 1) & 0xFE
$eepromProgAddressLine

// Define readout macro as empty
#define SIGMASTUDIOTYPE_SPECIAL(x) (x)

$paramContent

$hexContent

$dspProgramData

/**
 * @brief Function to load DSP firmware from the microcontroller flash memory
 * 
 * @param myDSP SigmaDSP object
 */
inline void loadProgram(SigmaDSP &myDSP)
{
  myDSP.writeRegister(CORE_REGISTER_R0_ADDR, CORE_REGISTER_R0_SIZE, DSP_core_register_R0_data);
  myDSP.writeRegisterBlock(PROGRAM_ADDR, PROGRAM_SIZE, DSP_program_data, PROGRAM_REGSIZE);
  myDSP.writeRegisterBlock(PARAMETER_ADDR, PARAMETER_SIZE, DSP_parameter_data, PARAMETER_REGSIZE);
  myDSP.writeRegister(HARDWARE_CONF_ADDR, HARDWARE_CONF_SIZE, DSP_hardware_conf_data);
  myDSP.writeRegister(CORE_REGISTER_R4_ADDR, CORE_REGISTER_R4_SIZE, DSP_core_register_R4_data);
}
#endif
"

Set-Content -Path $outputFile -Value $output.Replace(",}", "}")
Write-Host -ForegroundColor Green "Output written to $outputFile"

