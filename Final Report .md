# GUIDE: AUTOMATED SIMULINK CODE GENERATION
**Subject:** Complete Workflow for Production-Ready Embedded C Code from a Simulink Model

**Author:** Mahmoud Eldwakhly

---

## 1. Phase 1: Model Architecture (Simulink)

### 1.1 Signal Naming and Resolution
In C, every variable has a name. In Simulink, lines are often unnamed.

- **Rule:** Every line that you want to appear as a variable in C (Inputs, Outputs, significant internal states) **MUST** have a name.
- **How to do it:** Double-click a signal line and type a name (e.g., `Phase_A_Current_Reading`).
- **Description:** Right-click the signal → Properties → Documentation. Add a description here; beside that choose to make the signal as a Simulink signal.

### 1.2 Atomic Subsystems (Creating Functions)
By default, Simulink mashes everything into one giant `step()` function. To create modular C functions (e.g., `void Calculate_Phase_A(...)`):

1. Group your logic into a Subsystem (the one you need to be as a function in the generated code).
2. Right-click the Subsystem → Block Parameters.
3. Check "Treat as atomic unit".
4. Go to the Code Generation tab inside the block parameters.
5. Set Function Packaging to "Nonreusable function".

- **Result:** The generated code will create a specific `.c` function for this logic, making it cleaner and easier to debug.

---

## 2. Phase 2: The Data Dictionary
We do not hardcode numbers (like `5.0` or `4095`) inside the model blocks. Instead, we use a MATLAB script to define every parameter and signal. This gives us a single point of truth.

### 2.1 Defining Parameters (Constants)
Used for thresholds, calibration values, and physical constants.

![](Final_Report_images/page002_img01.png)

![](Final_Report_images/page003_img01.png)

- **Why Int32?** If we leave it as default, Simulink treats it as a double (Float).
- **Why ExportedGlobal?** This allows the variable to be "tuned" (changed) in real-time using calibration tools. Like global variable in C coding.

### 2.2 Defining Signals (Inputs/Outputs)
We use `Simulink.Signal` objects to control how variables are stored in memory.

![](Final_Report_images/page003_img02.png)

#### A. Inputs (Reading from Hardware)
When reading from a hardware register (like an ADC result), we don't "own" the variable; the hardware driver does.

- `ImportedExternPointer`: Tells the C-code generator: "Do not create this variable. It exists elsewhere. Just create a pointer to read it." Like `extern` in default C coding.

#### B. Outputs (Writing Results)
When calculating a result that `main.c` (for example) needs to read, we use `ExportedGlobal`.

- **Result in C:** `uint32_T I_out_a;` (A global variable accessible by everyone).

#### C. Internal Calculations (Hidden)
For intermediate math (like `Gain * Value`), we don't want to clutter the global header file.

![](Final_Report_images/page004_img01.png)

- **Result in C:** The variable becomes a temporary local variable inside the function.

---

## 3. The Automation Engine (Code generation Script)
Using a script (`codeGen_automated.m`) instead of the Simulink "Build" button.

### Why?
1. Repeatability: It guarantees the settings are identical every time.
2. Safety: It performs checks (like scanning for Data Type mismatches) that the App ignores.
3. Hygiene: It forces all generated files into a clean Build folder, keeping your project organized.

### How it works (Line-by-Line Breakdown)

#### 3.1 Environment Cleanup
```matlab
Simulink.fileGenControl('reset');
```
- Clears old paths to prevent "Shadowed File" errors.

#### 3.2 Loading the Fuel
```matlab
evalin('base', sprintf('run(''%s'')', scriptPath));
```
- Loads your Workspace Script to ensure all variables (int32, Extern, etc.) are in memory.

#### 3.3 The "Smart Scan" (Crucial Feature)
- **Problem:** If you define a Parameter as `int32` in the script, but the Block in Simulink defaults to `double`, the build fails.
- **Solution:** The script scans every Constant block in your model. If it sees you are using an `int32` parameter, it automatically updates the block to output `int32`.
- This saves you from manually checking hundreds of blocks.

---

## 4. Forcing 64-bit Math
```matlab
set_param(cs, 'ProdLongLongMode', 'on');
```
- This forces the compiler to use `long long` (native hardware math) instead of generating slow, ugly software libraries (`sMultiWordMul`).

## 5. Building in a Sandbox
```matlab
cd(buildFolder);
slbuild(modelName);
```
- The script physically moves MATLAB into the Build folder before generating code. This ensures all `.c` and `.h` files end up in one place, not scattered in your project root.

---

## 6. Summary of Differences
| Feature | Using Simulink "Build" Button | Using Our Professional Workflow |
|---|---|---|
| Data Types | Defaults to double (Float) everywhere. | Forced to int32/uint32 (Efficient). |
| Variable Names | Random (rtu_In1, rty_Out2). | Precise (Phase_A_Current, Error_Flag). |
| File Structure | Files dumped in root folder. | Clean Build folder. |
| Math Efficiency | Uses slow helper functions. | Uses Hardware Native long long. |
| Integration | Hard to connect to drivers. | ImportedExtern connects instantly. |

---

## 7. How to Use This (General Instructions)
To apply this to any new project:

1. Create folders:
	- `/Models` (Put your `.slx` here).
	- `/Scripts` (Put the Engine and Data scripts here).
	- `/Build` (Empty).
2. Create the Data Script:
	- Copy the template `WS_script_...m`.
	- Rename variables to match your new model (e.g., `Motor_Speed`, `Battery_Volts`).
	- Set Inputs to `ImportedExternPointer` and Outputs to `ExportedGlobal`.
3. Run the Engine:
	- Command:
```matlab
codeGen_automated('MyNewModel', 'MyDataScript');
```
4. Result:
	- Open `/Build/MyNewModel_ert_rtw/MyNewModel.c`.
	- You will see clean, readable, professional C code ready for your microcontroller.

---

## 8. Reference Implementation (TLE4971 Current Sensor Model)
To demonstrate this workflow, a complete reference implementation is provided for the TLE4971 Current Sensor Model. This example includes the model, the data script, and the automation engine.

### Included Files
1. `codeGen_automated.m` (The Engine)
	- The generalized script that performs the smart scan, fixes data types, and generates code into the Build folder.
2. `WS_script_current_sensor_model.m` (The Fuel)
	- A complete definition of the TLE4971 sensor physics, thresholds, and signal interfaces (Inputs as Extern, Outputs as Global).
3. `current_sensor_model.slx` (The Model)
	- The Simulink logic showing Atomic Subsystems (Phase A/B/C) and proper signal naming.
