# MODEL CODE GENERATION WORKFLOW — PROFESSIONAL GUIDE
### (General Workflow Applicable to Any Simulink Model)
### Example Used: TLE4971 Current Sensor Model

## 1. Introduction
This document presents a professional, repeatable workflow for preparing **any Simulink model** for high-quality, embedded-ready C code generation.
Although the example used here is a **TLE4971 current-sensor model**, the workflow is fully general and applies to all fields including automotive, robotics, aerospace, DSP, power electronics, and industrial automation.

This process provides:
- Clean modular generated C functions  
- Explicit argument‑based interfaces  
- Strict control over data types  
- Predictable naming and memory layout  
- Hardware‑ready Embedded Coder structures

The workflow uses:
- A **Workspace Initialization Script**
- A **Code Generation Script**
- Proper **Simulink data object setup**
- Correct **subsystem functional configuration**

---

# 2. Workflow Overview

Every embedded Simulink project should contain:

---

## 1) Workspace Script

Defines parameters, signals, and constants using **Simulink.Parameter** and **Simulink.Signal**.

Example name:
```
<ProjectName>_Workspace_for_CodeGen.m
```

---

## 2) Code Generation Script

Configures:
- Hardware
- Embedded Coder (ERT)
- Naming rules
- Function packaging
- Automatic build

Example name:
```
<ProjectName>_CodeGen.m
```

Run:
```matlab
<ProjectName>_CodeGen('MyModel')
```

---

## 3) Simulink Model Configuration

Subsystems must be configured with:
- Atomic boundaries  
- Functional packaging  
- Argument‑based interfaces  
- Clear data types  

---

# 3. Workspace Script (General Concept)

The workspace script defines the entire data interface.

---

## 3.1 Components of a Workspace Script

---

### A. Parameters (Simulink.Parameter)

Used for constants, thresholds, calibration values, etc.

```matlab
Kp = Simulink.Parameter(0.5);
Kp.DataType = 'single';
Kp.CoderInfo.StorageClass = 'ExportedGlobal';
Kp.CoderInfo.TypeQualifier = 'const';
```

---

### B. Signals (Simulink.Signal)

Used for typed I/O definitions.

```matlab
MotorSpeed = Simulink.Signal;
MotorSpeed.DataType = 'uint16';
MotorSpeed.CoderInfo.StorageClass = 'Auto';
```

---

### C. Automatic Storage Class Assignment

```matlab
allParams = who;
for k = 1:numel(allParams)
    p = eval(allParams{k});
    if isa(p,'Simulink.Parameter')
        p.CoderInfo.StorageClass = 'ExportedGlobal';
        assignin('base',allParams{k},p);
    end
end
```

---

### D. ImportedExternPointer (Optional Hardware I/O)

```matlab
ADC_Value = Simulink.Signal;
ADC_Value.DataType = 'int32';
ADC_Value.CoderInfo.StorageClass = 'ImportedExternPointer';
ADC_Value.CoderInfo.Identifier = 'ADC_Read_Reg';
```

---

### E. Derived Parameters

```matlab
ADC_Max = Simulink.Parameter(2^ADC_bits.Value - 1);
ADC_Max.CoderInfo.StorageClass = 'ExportedGlobal';
```

---

# 4. Subsystem Configuration (Simulink)

Subsystem configuration determines function-level code output.

---

## 4.1 Treat Subsystem as Atomic

Enable:

```
Subsystem Parameters → Main → Treat as atomic unit: ON
```

---

## 4.2 Code Generation Settings

Set:

```
Function packaging: Nonreusable function
Function name options: User specified
Function interface: Use subsystem input/output ports as function arguments
```

Example resulting C function:

```c
void PhaseA_Process(int32_T adc,
                    bool ocd1,
                    bool ocd2,
                    uint32_T* current_out,
                    bool* error_flag);
```

---

# 5. Code Generation Script (General Template)

---

## Step 1 — Load Workspace

```matlab
run('<Project>_Workspace_for_CodeGen.m');
```

---

## Step 2 — Load Model

```matlab
load_system(modelName);
cs = getActiveConfigSet(modelName);
```

---

## Step 3 — Configure Hardware

```matlab
set_param(cs,'ProdHWDeviceType','Texas Instruments->C2000');
set_param(cs,'ProdEqTarget','on');
set_param(cs,'ProdLongLongMode','on');
```

---

## Step 4 — Configure Embedded Coder

```matlab
set_param(cs,'SystemTargetFile','ert.tlc');
set_param(cs,'GenerateReport','on');
set_param(cs,'GenCodeOnly','on');
set_param(cs,'UtilityFuncGeneration','Auto');
set_param(cs,'ParameterPrecisionLossMsg','none');
```

---

## Step 5 — Configure Subsystems for Function Export

```matlab
set_param(block,'TreatAsAtomicUnit','on');
set_param(block,'RTWSystemCode','Nonreusable function');
set_param(block,'RTWFcnNameOptions','User specified');
set_param(block,'RTWFcnName', funcName);
```

---

## Step 6 — Build

```matlab
slbuild(modelName);
```

---

## Step 7 — Save

```matlab
save_system(modelName);
```

---

# 6. Apply Workflow to Any Project

---

## Step 1 — Create Workspace

Example:
```
MotorDrive_Workspace_for_CodeGen.m
```

---

## Step 2 — Create CodeGen Script

Example:
```
MotorDrive_CodeGen.m
```

---

## Step 3 — Configure Subsystems

✔ Atomic  
✔ Function packaging = Nonreusable  
✔ User-specified function names  
✔ Argument-based interface  

---

## Step 4 — Generate Code

Result:

- Clean modular C  
- MISRA-friendly  
- Explicit APIs  
- No hidden globals  

---

# 7. Generated C Code Example

```c
void PhaseA_CurrentCalc(int32_T adc_value,
                        bool ocd1,
                        bool ocd2,
                        uint32_T* current_output,
                        bool* error_flag)
{
    /* Implementation */
}
```

---

# 8. Conclusion

This workflow gives engineering teams:

- Complete control over code generation  
- Predictable and clean C interfaces  
- Professional embedded-ready C output  
- A repeatable methodology usable in any model  

Applicable to:

- Motor control  
- Power converters  
- Sensors  
- DSP algorithms  
- Aerospace & automotive ECUs  
- Robotics  
- Industrial automation  

If you need a PDF, template scripts, or a GitHub-ready README — just ask.

