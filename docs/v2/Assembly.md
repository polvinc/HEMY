
# ⚠️ WORK IN PROGRESS – Assembly Guide in Progress!

> **This assembly guide is still being written and improved.**
>
> Some steps may be missing, incomplete, or updated soon.
>
> If you're building HEMY, please read carefully, share feedback, and
> check the [Discussions](https://github.com/polvinc/HEMY/discussions) tab for help and updates.

___

# 🛠️ HEMY v2 Assembly Guide

This document walks you through the mechanical assembly of the HEMY v2 equatorial mount. Each section includes required parts, instructions, and suggested tips.

___
## Overall parts view 

The different parts the will be used in the current manual are presented here grouped by assembly groups. (E.g.: for the screw)
![Overall presentation](./pictures/2025-HEMY_disassembled-01_small.jpg)
[Full size image](./pictures/2025-HEMY_disassembled-01.jpg)


Above all, don't hesitate to open the [onshape 3D project](https://cad.onshape.com/documents/5bb7abe94ccc1dfccd880b05/w/c4dcfbc560d188dfaca97350/e/5198383ed1b9d7f502f35102?renderMode=0&uiState=6808b73883ed3655f234ab55) in parallel, so that you can observe the subtleties of assembly, the details of assembly, and the sub-assemblies.



___

## Axis Assembly (common part)

### 🔧 Step 1: Harmonic Reducer Pulley Installation

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP01-01.jpg" width="20%" style="margin-bottom: 5px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP01-02.jpg" width="20%">
</div>

**Parts:**
- C06 – Harmonic Reducer ZXK17-100 (x2)  
- A12 – Main Pulley (70T) (x2)
- S07 - Cone point hex socket set M3x3 (x12)
  
**Instructions:**  
- Foreach Pulley (70T) and Harmonic Reducer
   - Ensure correct orientation of the screws — refer to the 3D model if needed.
      - Replace cone point hex socket (original pressure screws shipped with the reducer) by shorter ones and change their orientation to push to the outside.
   - Attach the 70-tooth main pulley (A12) to one of the harmonic reducers (C06).  

Note: the original pressure screws shipped with the reducer have been replaced with shorter ones._

Note: You need the T01 tool (Allen Key).

___

## RA Axis Assembly


### 🔧 Step 2: Mount Reducer to Main Frame

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP02-01.jpg" width="20%" style="margin-bottom: 5px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP02-02.jpg" width="20%">
</div>

**Parts:**  
- A01 – Main Frame A (x1)  
- C10 – Square Corner Screw M4 (x2)  
- S19 – M4x8 Screws (x2)  
- S06 – M3x30 Screws (x7)  
- S02 – M3 Washers (x7)  
- S10 – M3 Nuts (x7)

**Instructions:**  
Fix two M4 square corner cubes (C10) onto Main Frame A (A01) using one M4x8 screw each. Then install the reducer (C06) using M3x30 screws with washers and nuts. 

- Double-check the reducer's orientation by identifying its screws and orienting them as in the picture.

- Double-check the reducer's alignment.

Due to the necessity to ensure that the reducer fits into the space, a little margin has been taken. Therefore, it is important to verify that there is as much space between the reducer and the main plate on each side.
  
> Tip: a 3D-printed centering tool can be helpful for perfect alignment. (To be build.)
> 
> Tip: put the same folded paper sheet on four sides of the reducer.

- Once the reducer is in place, remove the two screws indicated in blue on the photo (for later brake mount).

___

### 🔧 Step 3: Brake Sub-Assembly

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP03-01.jpg" width="20%" style="margin-bottom: 5px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP03-02.jpg" width="20%">
</div>

**Parts:**
- C08 - Shaft 5x45mm-Grind Flat
- A07 – Brake Support (x2)
- C15 – Brake (SWB-003) (x1)  
- C05 - Bearing 625ZZ (x2)
- S03 – Washer M5 (x2)  
- C01 – Pulley GT2 16T (x1)

- C12 – Spacer M3x19mm FF (x3)  
- S05 – M3x10 Screws (x3)
  
- S04 – M2x10 Screws (x2)  
- S01 – Washer M2 (x2)
- C04 - GT2 Belt – 200 mm

**Instructions:**


#### 🔧 Step 3.1: shaft sub assembly :
1. The grinding of the shaft is insufficient. It is necessary to complete this grinding to fit into the brake connection piece. Perform grinding over the full length of the shaft.
2. On One Break support assemble the spacer with M3 screws.
3. Then from this support, assemble from left to right on the shaft (note: The belt shall be inserted to go around the pulley) :
   1. Break Support
   2. Bearing
   3. Washer M5
   4. Pulley GT2 (Tighten it with the pulley screw).
   5. Washer
   6. Bearing
   7. Break support
4. Screw the last Break support to the Spacer.
5. Unscrew the pulley screw so that the end of the bar does not protrude beyond the edge of the brake support when the pulley is resting on the brake support.
6. Insert the brake connection piece on the other side of the shaft so that it is as close as possible to the support without touching it (1mm margin).

The result should be consistent to the following (save for the belt that shall have been inserted and cannot be seen here) :

<div style="flex: 1; flex-direction: column; text-align: inherit; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP03-03.jpg" width="20%" style="margin-bottom: 5px;">
</div>



#### 🔧 Step 3.2: Break preparation :
The connecting screws of the break shall be replaced with longer M2 screws.

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP03-04.jpg" width="20%" style="margin-bottom: 5px;">
</div>

For this you need to unscrew the following screws.

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP03-05.jpg" width="20%" style="margin-bottom: 5px;">
</div>

As those screws are tighted strongly, you will need a strong screw driver. You shall be carefull whhile unscrewing, to avoid the internal springs to get out from coming out of their housing.

Replace the screws with the M2 screws.

#### 🔧 Step 3.3: Final Results :

Results should look like :

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP03-06.jpg" width="20%" style="margin-bottom: 5px;">
</div>

Mount the brake (C15) to its upper support plate (A07) using three M3x19mm spacers and M3x10 screws. Use M2 washers and screws where needed for the electrical interface. Install the GT2 16T pulley (C01) onto the brake shaft.

___

### 🔧 Step 4: RA Assembly & Belt Tensioning

<div style="display: flex; flex-wrap: wrap; justify-content: space-between; max-width: 100%;">
  <div style="flex: 0 0 48%; max-width: 200px; margin-bottom: 10px;">
    <img src="pictures/2025-HEMY2-ASM-STEP04-01.jpg" style="-margin-bottom: 5px;">
    <img src="pictures/2025-HEMY2-ASM-STEP04-03.jpg" style="width: 30%; margin-bottom: 5px;">
    <img src="pictures/2025-HEMY2-ASM-STEP04-05.jpg" style="width: 30%; margin-bottom: 5px;">
  </div>
  <div style="flex: 0 0 48%; max-width: 200px; margin-bottom: 10px;">
    <img src="pictures/2025-HEMY2-ASM-STEP04-02.jpg" style="width: 30%; margin-bottom: 5px;">
    <img src="pictures/2025-HEMY2-ASM-STEP04-04.jpg" style="width: 30%; margin-bottom: 5px;">
    <img src="pictures/2025-HEMY2-ASM-STEP04-06.jpg" style="width: 30%; margin-bottom: 5px;">
  </div>
</div>

<div style="display: flex; justify-content: center; max-width: 200px;">
  <img src="pictures/2025-HEMY2-ASM-STEP04-07.jpg" style="width: 18%;">
</div>

**Parts:**
- A07 – Brake Support (x1)  

- A08 – Motor Support (x1)  
- C04 – GT2 Belt 200mm (x1)  
- C02 – Pulley GT2 20T (x1)  
- C07 – NEMA17 Motor (x1)  
- C11 – Spacer M3x21mm FM (x3)  
- S15 – M3x35 Screws (x2)  
- S06 – M3x30 Screws (x2)  
- S10 – M3 Nuts (x4)

**Instructions:**  
This is the most delicate step — take your time!

1. Prepare the NEMA17 motor (C07) with the GT2 20T pulley (C02) and three M3x21mm spacers (C11).
2. Position the belt (C04) around the brake pulley (C01).
3. Close the assembly with the lower brake support (A07).
4. Place the motor support (A08) onto the motor.
5. Lightly secure the motor to the reducer using one screw (indicated blue in the photo) — don't tighten fully yet.
6. Mount the brake onto the reducer using two M3x35 screws and nuts.
7. Insert the final two screws (red in photo) and adjust belt tension carefully.

🎉 Congratulations! The RA axis sub-assembly is now complete — the most challenging part is done!

___

## DEC Axis Assembly

### 🔧 Step 5: Main Pulley Installation

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP01-01.jpg" width="20%" style="margin-bottom: 5px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP01-02.jpg" width="20%">
</div>

**Parts:**
- C06 – Harmonic Reducer ZXK17-100 (x1)  
- A12 – Main Pulley (70T) (x1)

**Instructions:**  
As in Step 1, attach the 70T main pulley to the second harmonic reducer. Ensure the correct screw orientation — consult the 3D model. Use the shorter replacement pressure screws provided.

___

### 🔧 Step 6: Attach Cubes to Main Frame B

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP06-01.jpg" width="20%" style="margin-bottom: 5px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP06-02.jpg" width="20%">
</div>

**Parts:**
- A02 – Main Frame B (x1)  
- C10 – Square Corner Screw M4 (x6)  
- S19 – M4x8 Screws (x6)

**Instructions:**  
Attach all six M4 corner cubes (C10) to the Main Frame B (A02) using M4x8 screws.

___

### 🔧 Step 7: Mount DEC Reducer

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP07-01.jpg" width="20%" style="margin-bottom: 5px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP07-02-blue.jpg" width="20%">
</div>

**Parts:**
- S06 – M3x30 Screws (x9)  
- S02 – M3 Washers (x9)  
- S10 – M3 Nuts (x9)

**Instructions:**  
Center the harmonic reducer with its pulley onto Main Frame B (A02), and secure it using M3x30 screws with washers and nuts.

Make sure to properly orient the harmonic reducer in the main part, using the reducer screws as reference points. 

Note that two of these screws will be removed and replaced in a step described below with screws that will hold the motor. (See photo at the bottom left). 

Before proceeding with the extraction of these screws, ensure that the 9 connecting screws of the harmonic reducer and the main part are tightly secured.

> Tip: A 3D-printed alignment jig is helpful here as well.

Once aligned and fastened, remove the two blue-marked screws from the reducer (as shown in the photo).

___

### 🔧 Step 8: Prepare Motor Assembly

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP08-01.jpg" width="20%" style="margin-bottom: 5px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP08-02.jpg" width="20%">
</div>

**Parts:**
- C07 – NEMA17 Motor (x1)  
- C02 – Pulley GT2 20T (x1)  
- C03 – GT2 Belt 166mm (x1)  
- C11 – Spacer M3x21mm FM (x3)
- S16 - Socket button head M3x5 (x3)

**Instructions:**  
Mount the GT2 20T pulley onto the NEMA17 motor shaft. Add the three 21mm spacers and fit the 166mm GT2 belt.

___

### 🔧 Step 9: Mount Motor to Reducer

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP09-01.jpg" width="20%" style="margin-bottom: 5px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP09-02.jpg" width="20%">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP09-03.jpg" width="20%">
</div>

**Parts:**
- S15 – M3x35 Screws (x1)  
- S02 – M3 Washers (x1)  
- S10 – M3 Nuts (x1)
- S13 – M3x16 Screws (x2)

**Instructions:**  
Position the motor assembly on the reducer. Use the M3x35 screws and M3x16 screws to fasten everything and tension the belt accordingly.

🎉 The DEC axis is now fully assembled!

___

## Axis Frame Assembly

### 🔧 Step 10: Connect RA and DEC Axes

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP10-01.jpg" width="14%" style="margin-bottom: 5px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP10-02.jpg" width="14%">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP10-03.jpg" width="14%">
</div>

**Parts:**
- A01 – Main Frame A (RA side)  
- A02 – Main Frame B (DEC side)  
- S18 – M4x10 Screws (x4 or more as needed)

**Instructions:**  
Position the RA and DEC sub-assemblies to form a right angle (90°) between the two main frame plates (A01 and A02). Align the corresponding mounting holes and loosely fasten them using M4x10 screws.  
> ⚠️ Do not fully tighten the screws yet — the final squaring and reinforcement will be done with the lateral plates in later steps.

___

## Dovetail Assembly

> ⚠️ Note: The USB and DC passthrough electronics for the dovetail are still experimental and not fully tested.

### 🔧 Step 11: Attach Electronics and Cover

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP11-01.jpg" width="20%" style="margin-bottom: 5px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP11-02.jpg" width="20%">
</div>

**Parts:**
- B01 – Dovetail PCB Cover (x1)  
- PCB07 – Dovetail passthrough board (experimental)  
- A09 – Vixen/Losmandy Dovetail (x1)

**Instructions:**  
Position the passthrough PCB onto the dovetail (A09) and secure the 3D-printed cover (B01) as shown.

___

### 🔧 Step 12: Mount Dovetail on DEC Reducer

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP12-01.jpg" width="20%" style="margin-bottom: 5px;">
</div>

**Parts:**
- S06 – M3x30 Screws (x2)

**Instructions:**  
Route the cables through the reducer’s central cutout, then secure the dovetail (A09) to the DEC reducer using M3x30 screws.

___

### 🔧 Step 13: Install Clamps

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP13-01.jpg" width="20%" style="margin-bottom: 5px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP13-02.jpg" width="20%">
</div>

**Parts:**
- A10 – RFix Dovetail Clamps (x2)  
- C20 – Button Screws M6x40 with Springs (x2)

**Instructions:**  
Insert the two clamps (A10) into the dovetail (A09), secure them using the M6x40 button screws and springs.

🎉 The dovetail interface is now installed and ready to hold your telescope!

___

## Lateral Plates and Electronics

### 🔧 Step 14: Mount Lateral Plate B and Secure Frame

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP14-01.jpg" width="20%" style="margin-bottom: 5px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP14-02.jpg" width="20%">
</div>

**Parts:**
- A04 – Lateral Plate B (x1)  
- S19 – M4x8 Screws (as needed)

**Instructions:**  
Attach the lateral plate (A04) to the RA/DEC frame to complete the squared bracket. Once aligned, tighten all previously loose screws to secure the overall structure.

___

### 🔧 Step 15: Install Top Electronics

<div style="display: flex; margin-bottom: 5px;">
  <img src="pictures/2025-HEMY2-ASM-STEP15-01.jpg" width="30%" style="margin-right: 5px;">
  <img src="pictures/2025-HEMY2-ASM-STEP15-02.jpg" width="30%">
</div>

<div style="display: flex;">
  <img src="pictures/2025-HEMY2-ASM-STEP15-03.jpg" width="30%" style="margin-right: 5px;">
  <img src="pictures/2025-HEMY2-ASM-STEP15-04.jpg" width="30%">
</div>


**Parts:**
- All PCBs (assembled, see `pcb_assembly.md`)  
- C13 – Spacer M3x8mm FF (x4)  
- Handle, GPS antenna, GPS cover

**Instructions:**  
Install two M3 corner cubes on the top plate. Mount the main assembled PCB (including brake driver, TMC5160s, and Teensy) on the top plate using the M3x8 spacers. Install the handle and antennas (Wi-Fi and GPS), and place the GPS antenna cover (B02).
Make sure all cables are connected and routed cleanly.

___

### 🔧 Step 16: Final Assembly of Top/Bottom/Lateral Plates

<div style="display: flex; margin-bottom: 5px;">
  <img src="pictures/2025-HEMY2-ASM-STEP16-01.jpg" width="30%" style="margin-right: 5px;">
  <img src="pictures/2025-HEMY2-ASM-STEP16-02.jpg" width="30%">
</div>

<div style="display: flex;">
  <img src="pictures/2025-HEMY2-ASM-STEP16-03.jpg" width="30%" style="margin-right: 5px;">
  <img src="pictures/2025-HEMY2-ASM-STEP16-04.jpg" width="30%">
</div>

**Parts:**
- A05 – Top Plate  
- A06 – Bottom Plate  
- A03 – Lateral Plate A

**Instructions:**  
Secure the top plate (A05) to the mount structure first, ensuring all cabling is in place. Then attach the bottom plate (A06) and the remaining lateral plate (A03) to complete the enclosure.

___

## Final Adjustments and Output Flange

### 🔧 Step 17: Install Output Flange and Vixen Plate

<div style="flex: 1; max-width: 200px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP17-01.jpg" width="20%" style="margin-bottom: 5px;">
  <img align="right" src="pictures/2025-HEMY2-ASM-STEP17-02.jpg" width="20%">
</div>

**Parts:**
- A11 – Output Flange  
- C18 – Vixen Plate

**Instructions:**  
Install the output flange (A11) onto the RA reducer. Then, mount the Vixen plate (C18) securely.

🎉 You have now fully assembled your HEMY mount!

___

## ✅ Final Checklist Before First Use

- [ ] All mechanical screws are tightened
- [ ] Belts are properly tensioned on both RA and DEC
- [ ] All pulleys are aligned and set screws are secure
- [ ] The output flange and dovetail clamps are solidly mounted
- [ ] Brake is mounted and functional
- [ ] GPS and antennas are installed and connected
- [ ] Power wiring is secured and routed cleanly
- [ ] All PCBs are installed and correctly connected
- [ ] No stray cables or obstructing elements
- [ ] RA and DEC axes can rotate freely with appropriate torque on motors

___

## 🧠 Flashing OnStepX Firmware

HEMY is designed to run with **OnStepX**, an open-source controller firmware for astronomical mounts. Flashing the firmware is simple, but depends on your hardware configuration (Teensy, STM32, ESP32, etc).

### Basic Steps:
1. **Download OnStepX** from the [official GitHub repository](https://github.com/hjd1964/OnStepX)
2. **Open the firmware** in [PlatformIO](https://platformio.org/) (VSCode plugin)
3. **Edit `Config.h` and `Board.h`** to match your HEMY configuration (stepper drivers, gear ratios, brake, etc)
4. Connect your board via USB
5. Click **Upload** in PlatformIO to flash the firmware
6. Open the **serial monitor** and confirm initialization (check RA/DEC movements, homing, etc)

> 💡 Tip: Refer to the `pcb_assembly.md` file for hardware pin mapping and settings.

Once the firmware is flashed and the mount is powered, you’re ready to configure OnStep via Wi-Fi, serial, or compatible astronomy tools (INDI, ASCOM, etc).

___
