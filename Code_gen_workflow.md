# PROFESSIONAL GUIDE: AUTOMATED SIMULINK CODE GENERATION
**Subject:** Complete Workflow for Production-Ready Embedded C Code (TI C2000 & General MCU) for any Simulink model

**Author:** Mahmoud Eldwakhly

---

## 1. Introduction
In professional embedded engineering, simply clicking the "Build" button in Simulink is rarely enough. To produce code that runs efficiently on a microcontroller (like a TI C2000) and integrates safely with other drivers, we need **absolute control** over memory, data types, and file structure.

This workflow separates the project into three distinct layers:
1. **The Architecture:** The visual Simulink model (Logic).
2. **The Data Dictionary:** A MATLAB script defining variables, types, and memory (The "Fuel").
3. **The Engine:** An automated script that compiles the code with strict professional rules (The "Generator").

This guide explains how to replicate this professional workflow for **any** Simulink project.

---

## 2. Phase 1: Model Architecture (Simulink)
Before generating code, the model must be structured to behave like C-code.

### 2.1 Signal Naming and Resolution
In C, every variable has a name. In Simulink, lines are often unnamed.
- **Rule:** Every line that you want to appear as a variable in C (Inputs, Outputs, significant internal states) **MUST** have a name.
- **How to do it:** Double-click a signal line and type a name (e.g., `Phase_A_Current_Reading`).
- **Description:** Right-click the signal → Properties → Documentation. Add a description here; it will appear as a comment in the generated C code.

### 2.2 Atomic Subsystems (Creating Functions)
By default, Simulink combines everything into one giant `step()` function. To create modular C functions (e.g., `void Calculate_Phase_A(...)`):
1. Group your logic into a **Subsystem**.
2. Right-click the Subsystem → **Block Parameters**.
3. Check **"Treat as atomic unit"**.
4. Go to the **Code Generation** tab inside the block parameters.
5. Set Function Packaging to **"Nonreusable function"**.
- **Result:** The generated code will create a specific `.c` function for this logic, making it cleaner and easier to debug.

---

## 3. Phase 2: The Data Dictionary (The "Fuel")
We do not hardcode numbers (like `5.0` or `4095`) inside the model blocks. Instead, we use a MATLAB script to define every parameter and signal. This gives us a single point of truth.

### 3.1 Defining Parameters (Constants)
Used for thresholds, calibration values, and physical constants.

```matlab
% Define the value
ADC_max = Simulink.Parameter(4095);

% FORCE the Data Type (Crucial for C-Code)
ADC_max.DataType = 'int32'; 

% FORCE the Visibility (Storage Class)
ADC_max.CoderInfo.StorageClass = 'ExportedGlobal';
```

- **Why Int32?** If we leave it as default, Simulink treats it as a double (Float). Microcontrollers are slow with Floats. Forcing int32 makes the code fast and small.
- **Why ExportedGlobal?** This allows the variable to be "tuned" (changed) in real-time using calibration tools (like CANape) without recompiling.

### 3.2 Defining Signals (Inputs/Outputs)
We use `Simulink.Signal` objects to control how variables are stored in memory.

#### A. Inputs (Reading from Hardware)
When reading from a hardware register (like an ADC result), we don't "own" the variable; the hardware driver does.

```matlab
Adc_value_a = Simulink.Signal; 
Adc_value_a.DataType = 'int32'; 
Adc_value_a.CoderInfo.StorageClass = 'ImportedExternPointer';
```

- **ImportedExternPointer:** Tells the C-code generator: "Do not create this variable. It exists elsewhere. Just create a pointer to read it."

#### B. Outputs (Writing Results)
When calculating a result that `main.c` needs to read, we use `ExportedGlobal`.

```matlab
I_out_a = Simulink.Signal; 
I_out_a.DataType = 'uint32'; 
I_out_a.CoderInfo.StorageClass = 'ExportedGlobal';
```

- **Result in C:** `uint32_T I_out_a;` (A global variable accessible by everyone).

#### C. Internal Calculations (Hidden)
For intermediate math (like `Gain * Value`), we don't want to clutter the global header file.

```matlab
Internal_Calc = Simulink.Signal; 
Internal_Calc.DataType = 'auto';     % Let Simulink decide (handles 64-bit math)
Internal_Calc.CoderInfo.StorageClass = 'Auto'; % Hidden from .h file
```

- **Result in C:** The variable becomes a temporary local variable inside the function (static or local stack). It uses zero global RAM.

---

## 4. Phase 3: The Automation Engine (The Script)
We use a script (`codeGen_automated.m`) instead of the Simulink "Build" button.

### Why use a script?
- **Repeatability:** It guarantees the settings are identical every time.
- **Safety:** It performs checks (like scanning for Data Type mismatches) that the App ignores.
- **Hygiene:** It forces all generated files into a clean Build folder, keeping your project organized.

### How it works (Line-by-Line Breakdown)

#### Environment Cleanup:
```matlab
Simulink.fileGenControl('reset');
```
Clears old paths to prevent "Shadowed File" errors.

#### The "Smart Scan" (Crucial Feature):
- **Problem:** If you define a Parameter as `int32` in the script, but the Block in Simulink defaults to `double`, the build fails.
- **Solution:** The script scans every Constant block in your model. If it sees you are using an `int32` parameter, it automatically updates the block to output `int32`.

#### Forcing 64-bit Math:
```matlab
set_param(cs, 'ProdLongLongMode', 'on');
```
This forces the compiler to use `long long` (native hardware math) instead of generating slow, ugly software libraries (`sMultiWordMul`).

#### Building in a Sandbox:
```matlab
cd(buildFolder);
slbuild(modelName);
```
The script physically moves MATLAB into the Build folder before generating code. This ensures all `.c` and `.h` files end up in one place, not scattered in your project root.

---

## 5. Summary of Differences

- **Data Types:**
  - Simulink "Build" Button: Defaults to double (Float) everywhere.
  - Professional Workflow: Forced to int32/uint32 (Efficient).
- **Variable Names:**
  - Simulink "Build" Button: Random (rtu_In1, rty_Out2).
  - Professional Workflow: Precise (Phase_A_Current, Error_Flag).
- **File Structure:**
  - Simulink "Build" Button: Files dumped in root folder.
  - Professional Workflow: Clean Build folder.
- **Math Efficiency:**
  - Simulink "Build" Button: Uses slow helper functions.
  - Professional Workflow: Uses Hardware Native `long long`.
- **Integration:**
  - Simulink "Build" Button: Hard to connect to drivers.
  - Professional Workflow: `ImportedExtern` connects instantly.

---

## 6. Practical Reference: TLE4971 Current Sensor Example
Below are the two core scripts developed for the TLE4971 project. These serve as the templates for any future project.

### A. The Engine Script: `codeGen_automated.m`
Save this file in your project folder.

```matlab
function codeGen_automated(modelName, dataScriptName)
% CODEGEN_AUTOMATED Final Professional Engine
% 
% GENERALIZED: Works for ANY model.
% FEATURES:
%   1. Automatic "Smart Scan" to fix Data Type Mismatches (Double vs Int32).
%   2. Forces 64-bit Math (LongLong) for clean C-code.
%   3. Handles Folder management to prevent conflicts.

    %% 1. INITIAL SETUP
    % Use defaults if no arguments provided
    if nargin < 1, modelName = 'current_sensor_model'; end
    if nargin < 2, dataScriptName = 'WS_script_current_sensor_model'; end

    clc;
    fprintf('======================================================\n');
    fprintf('        PROFESSIONAL CODE GENERATION ENGINE           \n');
    fprintf('======================================================\n');

    % Reset paths to ensure a clean start
    Simulink.fileGenControl('reset');
    
    % Define Project Paths
    projectRoot = pwd;
    buildFolder = fullfile(projectRoot, 'Build');
    if ~exist(buildFolder, 'dir'); mkdir(buildFolder); end

    try
        %% 2. LOAD DATA (The Fuel)
        fprintf('>>> [Step 1] Loading Data Script: %s... \n', dataScriptName);
        scriptPath = which(dataScriptName);
        if isempty(scriptPath)
             error('Data script "%s" not found! Ensure it is in the project folder.', dataScriptName);
        end
        evalin('base', sprintf('run(''%s'')', scriptPath));


        %% 3. LOAD MODEL & APPLY UNIVERSAL FIXES
        fprintf('>>> [Step 2] Loading Model...\n');
        load_system(modelName);
        cs = getActiveConfigSet(modelName);
        
        % --- FIX 1: SOLVER (Force Fixed-Step) ---
        set_param(modelName, 'SolverType', 'Fixed-step');
        set_param(modelName, 'Solver', 'FixedStepAuto');
        set_param(modelName, 'DataDictionary', ''); % Unlink old dictionaries

        % --- FIX 2: HARDWARE (TI C2000) ---
        set_param(cs, 'ProdHWDeviceType', 'Texas Instruments->C2000');
        
        % --- FIX 3: 64-BIT MATH (The "sMultiWordMul" Killer) ---
        try
            set_param(cs, 'ProdLongLongMode', 'on');
            fprintf('    -> OPTIMIZATION: 64-bit Math (LongLong) ENABLED.\n');
        catch
            fprintf('    -> NOTE: LongLongMode not supported on this target (Skipping).\n');
        end
        
        % --- FIX 4: CLEAN CODE FORMATTING ---
        set_param(cs, 'SystemTargetFile', 'ert.tlc');
        set_param(cs, 'GenCodeOnly', 'on');
        set_param(cs, 'GenerateReport', 'on');
        set_param(cs, 'GenerateComments', 'on');


        %% [NEW] STEP 2.5: SMART TYPE FIXER (The General Solution)
        % Instead of hardcoding paths, we scan EVERY Constant block.
        % If a block uses a parameter that we know is Int32, we force the block to Int32.
        fprintf('>>> [Step 2.5] Smart-Scanning for Data Type Mismatches...\n');
        
        % Find ALL Constant blocks in the model (Recursive search)
        constBlocks = find_system(modelName, 'MatchFilter', @Simulink.match.allVariants, 'BlockType', 'Constant');
        
        fixedCount = 0;
        for i = 1:length(constBlocks)
            blk = constBlocks{i};
            valStr = get_param(blk, 'Value');
            
            % Check if the Value is a variable name in the workspace
            if evalin('base', sprintf('exist(''%s'', ''var'')', valStr))
                % It is a variable. Check if it is a Simulink.Parameter with Int32 type.
                isInt32 = false;
                try
                    isInt32 = evalin('base', sprintf('isa(%s, ''Simulink.Parameter'') && strcmp(%s.DataType, ''int32'')', valStr, valStr));
                catch
                end
                
                if isInt32
                    % FOUND ONE! This block uses an Int32 parameter but defaults to Double.
                    % We MUST force it to Int32 to prevent the error.
                    set_param(blk, 'OutDataTypeStr', 'int32');
                    fprintf('    -> Auto-Fixed Block: %s (Matched Int32 Parameter)\n', blk);
                    fixedCount = fixedCount + 1;
                end
            end
        end
        
        if fixedCount == 0
            fprintf('    -> No mismatched blocks found. (Model is clean or uses explicit types).\n');
        end


        %% 4. CONFIGURE ATOMIC SUBSYSTEMS
        % Generalizes to any subsystem marked as "Atomic"
        fprintf('>>> [Step 3] Configuring Atomic Subsystems...\n');
        blocks = find_system(modelName, 'BlockType', 'SubSystem', 'TreatAsAtomicUnit', 'on');
        for k = 1:length(blocks)
            set_param(blocks{k}, 'RTWSystemCode', 'Nonreusable function');
        end


        %% 5. BUILD
        fprintf('>>> [Step 4] Validating & Building...\n');
        
        % Diagnostic update to catch errors before building
        set_param(modelName, 'SimulationCommand', 'update'); 
        
        % Move to Build folder to prevent path conflicts
        Simulink.fileGenControl('set', 'CacheFolder', buildFolder, 'CodeGenFolder', buildFolder, 'KeepPreviousPath', false);
        cd(buildFolder);
        
        % Build
        slbuild(modelName);
        
        fprintf('\n======================================================\n');
        fprintf(' SUCCESS! Production Code generated in: /Build \n');
        fprintf('======================================================\n');

    catch ME
        fprintf('\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n');
        fprintf(' BUILD FAILED: \n');
        fprintf(' %s\n', ME.message);
        
        if ~isempty(ME.cause)
             for k=1:length(ME.cause)
                 fprintf(' -> DETAILED CAUSE: %s\n', ME.cause{k}.message); 
             end
        end
        fprintf('!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n');
    end

    % Cleanup: Return to root
    cd(projectRoot);
    Simulink.fileGenControl('reset');
end
```

### B. The Fuel Script: `WS_script_current_sensor_model.m`
Save this file in your project folder.

```matlab
%% ========================================================================
%  FILE: WS_script_current_sensor_model.m
%  PURPOSE: FINAL OPTIMIZED MEMORY (Inputs=Extern, Outputs=Global Int32)
% ========================================================================
clear; clc;

%% 1. PARAMETERS (PHYSICS & CONSTANTS)
VDD          = Simulink.Parameter(5.0);
Vref_ADC     = Simulink.Parameter(4.5);
Vmid         = Simulink.Parameter(2.5);
Vmin         = Simulink.Parameter(0.5);
Vmax         = Simulink.Parameter(4.5);
IFSR_A       = Simulink.Parameter(120);
Sensitivity  = Simulink.Parameter((Vmax.Value - Vmin.Value) / (2 * IFSR_A.Value));

% --- Thresholds (Int32) ---
Error_threshold_phases = Simulink.Parameter(200); 
OCD1_threshold_A = Simulink.Parameter(192);
OCD2_threshold_A = Simulink.Parameter(96);

% --- Filter & ADC ---
ADC_bits = Simulink.Parameter(12);
ADC_max  = Simulink.Parameter(double(bitshift(uint32(1), uint32(ADC_bits.Value)) - uint32(1)));

% --- Calculated Offsets ---
val_Conversion = Vref_ADC.Value / (ADC_max.Value * Sensitivity.Value);
val_Offset     = -(ADC_max.Value * Vmid.Value / Vref_ADC.Value);

% --- FORCE PARAMETER TYPES (INT32) ---
% ExportedGlobal = Stored in Flash/RAM (Tunable)
ADC_max.DataType = 'int32'; 
ADC_max.CoderInfo.StorageClass = 'ExportedGlobal';

Error_threshold_phases.DataType = 'int32';
Error_threshold_phases.CoderInfo.StorageClass = 'ExportedGlobal';

ADC_Zero_Current_Offset = Simulink.Parameter(val_Offset);
ADC_Zero_Current_Offset.DataType = 'int32';
ADC_Zero_Current_Offset.CoderInfo.StorageClass = 'ExportedGlobal';

ADC_Conversion_Current = Simulink.Parameter(val_Conversion);
ADC_Conversion_Current.DataType = 'single'; 
ADC_Conversion_Current.CoderInfo.StorageClass = 'Auto'; % Hidden (Compiler Optimization)

%% 2. AUTOMATION: CONVERT REMAINING PARAMETERS
allVars = who;
for k = 1:numel(allVars)
    objName = allVars{k};
    objValue = eval(objName);
    if isa(objValue, 'Simulink.Parameter') && strcmp(objValue.CoderInfo.StorageClass, 'Auto')
        % Leave as Auto
    end
end

%% 3. SIGNALS (MEMORY OPTIMIZED)
% -------------------------------------------------------------------------

% [HIDDEN] INTERNAL SIGNALS -> STORAGE: AUTO
% These will be 'local variables' in C functions.
% They consume STACK memory (Temporary), NOT Global RAM.
ADC_MIN_a = Simulink.Signal; ADC_MIN_a.DataType = 'int32'; ADC_MIN_a.CoderInfo.StorageClass = 'Auto';
ADC_MAX_a = Simulink.Signal; ADC_MAX_a.DataType = 'int32'; ADC_MAX_a.CoderInfo.StorageClass = 'Auto';
ADC_MIN_b = Simulink.Signal; ADC_MIN_b.DataType = 'int32'; ADC_MIN_b.CoderInfo.StorageClass = 'Auto';
ADC_MAX_b = Simulink.Signal; ADC_MAX_b.DataType = 'int32'; ADC_MAX_b.CoderInfo.StorageClass = 'Auto';
ADC_MIN_c = Simulink.Signal; ADC_MIN_c.DataType = 'int32'; ADC_MIN_c.CoderInfo.StorageClass = 'Auto';
ADC_MAX_c = Simulink.Signal; ADC_MAX_c.DataType = 'int32'; ADC_MAX_c.CoderInfo.StorageClass = 'Auto';

ADC_Zero_Current_Offset_a = Simulink.Signal; ADC_Zero_Current_Offset_a.DataType = 'int32'; ADC_Zero_Current_Offset_a.CoderInfo.StorageClass = 'Auto';
ADC_Zero_Current_Offset_b = Simulink.Signal; ADC_Zero_Current_Offset_b.DataType = 'int32'; ADC_Zero_Current_Offset_b.CoderInfo.StorageClass = 'Auto';
ADC_Zero_Current_Offset_c = Simulink.Signal; ADC_Zero_Current_Offset_c.DataType = 'int32'; ADC_Zero_Current_Offset_c.CoderInfo.StorageClass = 'Auto';

% [HIDDEN] GAIN CALCULATIONS (Must be Auto for 64-bit Math)
% This creates a temporary 'int64' on the stack, does the multiply, 
% and discards it immediately. Zero Global RAM usage.
ADC_Conversion_Gain_a = Simulink.Signal; ADC_Conversion_Gain_a.DataType = 'auto'; ADC_Conversion_Gain_a.CoderInfo.StorageClass = 'Auto';
ADC_Conversion_Gain_b = Simulink.Signal; ADC_Conversion_Gain_b.DataType = 'auto'; ADC_Conversion_Gain_b.CoderInfo.StorageClass = 'Auto';
ADC_Conversion_Gain_c = Simulink.Signal; ADC_Conversion_Gain_c.DataType = 'auto'; ADC_Conversion_Gain_c.CoderInfo.StorageClass = 'Auto';

% [GLOBAL] MAIN RESULTS (UInt32)
Phase_A_Current_Reading = Simulink.Signal; Phase_A_Current_Reading.DataType = 'uint32'; Phase_A_Current_Reading.CoderInfo.StorageClass = 'ExportedGlobal';
Phase_B_Current_Reading = Simulink.Signal; Phase_B_Current_Reading.DataType = 'uint32'; Phase_B_Current_Reading.CoderInfo.StorageClass = 'ExportedGlobal';
Phase_C_Current_Reading = Simulink.Signal; Phase_C_Current_Reading.DataType = 'uint32'; Phase_C_Current_Reading.CoderInfo.StorageClass = 'ExportedGlobal';

I_out_a = Simulink.Signal; I_out_a.DataType = 'uint32'; I_out_a.CoderInfo.StorageClass = 'ExportedGlobal'; I_out_a.CoderInfo.Identifier = 'I_out_a_variable';
I_out_b = Simulink.Signal; I_out_b.DataType = 'uint32'; I_out_b.CoderInfo.StorageClass = 'ExportedGlobal'; I_out_b.CoderInfo.Identifier = 'I_out_b_variable';
I_out_c = Simulink.Signal; I_out_c.DataType = 'uint32'; I_out_c.CoderInfo.StorageClass = 'ExportedGlobal'; I_out_c.CoderInfo.Identifier = 'I_out_c_variable';

% [GLOBAL] FLAGS (Boolean - Required for Logic)
ERROR_PHASES_CURRENTS = Simulink.Signal; ERROR_PHASES_CURRENTS.DataType = 'boolean'; ERROR_PHASES_CURRENTS.CoderInfo.StorageClass = 'ExportedGlobal';
ERROR_ADC_RANGE_A = Simulink.Signal; ERROR_ADC_RANGE_A.DataType = 'boolean'; ERROR_ADC_RANGE_A.CoderInfo.StorageClass = 'ExportedGlobal';
ERROR_ADC_RANGE_B = Simulink.Signal; ERROR_ADC_RANGE_B.DataType = 'boolean'; ERROR_ADC_RANGE_B.CoderInfo.StorageClass = 'ExportedGlobal';
ERROR_ADC_RANGE_C = Simulink.Signal; ERROR_ADC_RANGE_C.DataType = 'boolean'; ERROR_ADC_RANGE_C.CoderInfo.StorageClass = 'ExportedGlobal';

OC1_A_FLAG_BIT = Simulink.Signal; OC1_A_FLAG_BIT.DataType = 'boolean'; OC1_A_FLAG_BIT.CoderInfo.StorageClass = 'ExportedGlobal';
OC2_A_FLAG_BIT = Simulink.Signal; OC2_A_FLAG_BIT.DataType = 'boolean'; OC2_A_FLAG_BIT.CoderInfo.StorageClass = 'ExportedGlobal';
OC1_B_FLAG_BIT = Simulink.Signal; OC1_B_FLAG_BIT.DataType = 'boolean'; OC1_B_FLAG_BIT.CoderInfo.StorageClass = 'ExportedGlobal';
OC2_B_FLAG_BIT = Simulink.Signal; OC2_B_FLAG_BIT.DataType = 'boolean'; OC2_B_FLAG_BIT.CoderInfo.StorageClass = 'ExportedGlobal';
OC1_C_FLAG_BIT = Simulink.Signal; OC1_C_FLAG_BIT.DataType = 'boolean'; OC1_C_FLAG_BIT.CoderInfo.StorageClass = 'ExportedGlobal';
OC2_C_FLAG_BIT = Simulink.Signal; OC2_C_FLAG_BIT.DataType = 'boolean'; OC2_C_FLAG_BIT.CoderInfo.StorageClass = 'ExportedGlobal';

OCD1_a_flag  = Simulink.Signal; OCD1_a_flag.DataType  = 'boolean'; OCD1_a_flag.CoderInfo.StorageClass  = 'ExportedGlobal'; OCD1_a_flag.CoderInfo.Identifier  = 'OCD1_a_flag_variable';
OCD2_a_flag  = Simulink.Signal; OCD2_a_flag.DataType  = 'boolean'; OCD2_a_flag.CoderInfo.StorageClass  = 'ExportedGlobal'; OCD2_a_flag.CoderInfo.Identifier  = 'OCD2_a_flag_variable';
Error_flag_a = Simulink.Signal; Error_flag_a.DataType = 'boolean'; Error_flag_a.CoderInfo.StorageClass = 'ExportedGlobal'; Error_flag_a.CoderInfo.Identifier = 'Error_flag_a_variable';

OCD1_b_flag  = Simulink.Signal; OCD1_b_flag.DataType  = 'boolean'; OCD1_b_flag.CoderInfo.StorageClass  = 'ExportedGlobal'; OCD1_b_flag.CoderInfo.Identifier  = 'OCD1_b_flag_variable';
OCD2_b_flag  = Simulink.Signal; OCD2_b_flag.DataType  = 'boolean'; OCD2_b_flag.CoderInfo.StorageClass  = 'ExportedGlobal'; OCD2_b_flag.CoderInfo.Identifier  = 'OCD2_b_flag_variable';
Error_flag_b = Simulink.Signal; Error_flag_b.DataType = 'boolean'; Error_flag_b.CoderInfo.StorageClass = 'ExportedGlobal'; Error_flag_b.CoderInfo.Identifier = 'Error_flag_b_variable';

OCD1_c_flag  = Simulink.Signal; OCD1_c_flag.DataType  = 'boolean'; OCD1_c_flag.CoderInfo.StorageClass  = 'ExportedGlobal'; OCD1_c_flag.CoderInfo.Identifier  = 'OCD1_c_flag_variable';
OCD2_c_flag  = Simulink.Signal; OCD2_c_flag.DataType  = 'boolean'; OCD2_c_flag.CoderInfo.StorageClass  = 'ExportedGlobal'; OCD2_c_flag.CoderInfo.Identifier  = 'OCD2_c_flag_variable';
Error_flag_c = Simulink.Signal; Error_flag_c.DataType = 'boolean'; Error_flag_c.CoderInfo.StorageClass = 'ExportedGlobal'; Error_flag_c.CoderInfo.Identifier = 'Error_flag_c_variable';

ERROR_Variable = Simulink.Signal; 
ERROR_Variable.DataType = 'uint16'; 
ERROR_Variable.CoderInfo.StorageClass = 'ExportedGlobal'; 
ERROR_Variable.CoderInfo.Identifier = 'Error_Flags_Variable';

% [INPUTS] POINTERS (Int32 / Boolean)
Adc_value_a = Simulink.Signal; Adc_value_a.DataType = 'int32'; Adc_value_a.CoderInfo.StorageClass = 'ImportedExternPointer'; Adc_value_a.CoderInfo.Identifier = 'Adc_value_a_variable';
OCD1_a      = Simulink.Signal; OCD1_a.DataType      = 'boolean'; OCD1_a.CoderInfo.StorageClass      = 'ImportedExternPointer'; OCD1_a.CoderInfo.Identifier      = 'OCD1_a_variable';
OCD2_a      = Simulink.Signal; OCD2_a.DataType      = 'boolean'; OCD2_a.CoderInfo.StorageClass      = 'ImportedExternPointer'; OCD2_a.CoderInfo.Identifier      = 'OCD2_a_variable';

Adc_value_b = Simulink.Signal; Adc_value_b.DataType = 'int32'; Adc_value_b.CoderInfo.StorageClass = 'ImportedExternPointer'; Adc_value_b.CoderInfo.Identifier = 'Adc_value_b_variable';
OCD1_b      = Simulink.Signal; OCD1_b.DataType      = 'boolean'; OCD1_b.CoderInfo.StorageClass      = 'ImportedExternPointer'; OCD1_b.CoderInfo.Identifier      = 'OCD1_b_variable';
OCD2_b      = Simulink.Signal; OCD2_b.DataType      = 'boolean'; OCD2_b.CoderInfo.StorageClass      = 'ImportedExternPointer'; OCD2_b.CoderInfo.Identifier      = 'OCD2_b_variable';

Adc_value_c = Simulink.Signal; Adc_value_c.DataType = 'int32'; Adc_value_c.CoderInfo.StorageClass = 'ImportedExternPointer'; Adc_value_c.CoderInfo.Identifier = 'Adc_value_c_variable';
OCD1_c      = Simulink.Signal; OCD1_c.DataType      = 'boolean'; OCD1_c.CoderInfo.StorageClass      = 'ImportedExternPointer'; OCD1_c.CoderInfo.Identifier      = 'OCD1_c_variable';
OCD2_c      = Simulink.Signal; OCD2_c.DataType      = 'boolean'; OCD2_c.CoderInfo.StorageClass      = 'ImportedExternPointer'; OCD2_c.CoderInfo.Identifier      = 'OCD2_c_variable';

disp('>> DATA SCRIPT LOADED: Optimizations Applied.');
```
