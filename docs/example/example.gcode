;====Single Track G-Code====
G90 G54 G64 G50 G17 G40 G80 G94 G91.1 G49
G90 ; absolute coordinates
G21 ; set units to millimeters
T11 G43 H11 M6 ; set tool as T11, perform tool change
M64 P2 ; start fume extractor
M64 P3 ; start argon purge gas
G4 P0.001 ; added because G1 being skipped
M201 (SDC 0) ; set laser power to 0%M201 (EMON) ; turn on the laser

;===Starting Single Track 1===
G1 Z25 F3900

;===Adjusting deposition rate===
M205 (H_0_V_0.4) ; Feed rate for hopper 1
M205 (H_1_V_2.5) ; Argon carrier gas flow rate hopper 1
M205 (H_2_V_0.0) ; Feed rate for hopper 2
M205 (H_3_V_2.5) ; Argon carrier gas flow rate hopper 2
G4 P30 ; powder stabilization

G1 X2.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 37.8)
G1 Y12.0 F651.85
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 2===
G1 Z25 F3900

G1 X5.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 38.75)
G1 Y12.0 F242.9
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 3===
G1 Z25 F3900

G1 X8.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 40.7)
G1 Y12.0 F693.55
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 4===
G1 Z25 F3900

G1 X11.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 32.55)
G1 Y12.0 F358.75
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 5===
G1 Z25 F3900

G1 X14.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 52.45)
G1 Y12.0 F349.15
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 6===
G1 Z25 F3900

G1 X17.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 42.65)
G1 Y12.0 F280.2
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 7===
G1 Z25 F3900

G1 X20.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 50.1)
G1 Y12.0 F629.05
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 8===
G1 Z25 F3900

G1 X23.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 43.5)
G1 Y12.0 F546.3
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 9===
G1 Z25 F3900

G1 X26.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 40.0)
G1 Y12.0 F389.7
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 10===
G1 Z25 F3900

G1 X29.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 35.05)
G1 Y12.0 F509.4
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 11===
G1 Z25 F3900

G1 X32.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 41.4)
G1 Y12.0 F434.55
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 12===
G1 Z25 F3900

G1 X35.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 58.25)
G1 Y12.0 F557.85
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 13===
G1 Z25 F3900

G1 X38.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 54.0)
G1 Y12.0 F257.7
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 14===
G1 Z25 F3900

G1 X41.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 34.35)
G1 Y12.0 F318.6
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 15===
G1 Z25 F3900

G1 X44.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 47.0)
G1 Y12.0 F472.05
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 16===
G1 Z25 F3900

G1 X47.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 51.35)
G1 Y12.0 F377.4
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 17===
G1 Z25 F3900

G1 X50.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 31.7)
G1 Y12.0 F636.75
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 18===
G1 Z25 F3900

G1 X53.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 59.8)
G1 Y12.0 F451.7
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 19===
G1 Z25 F3900

G1 X56.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 55.25)
G1 Y12.0 F666.3
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 20===
G1 Z25 F3900

G1 X59.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 45.9)
G1 Y12.0 F331.15
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 21===
G1 Z25 F3900

G1 X62.0 Y2.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 31.0)
G1 Y12.0 F419.85
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 22===
G1 Z25 F3900

G1 X2.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 57.75)
G1 Y24.0 F485.55
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 23===
G1 Z25 F3900

G1 X5.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 54.3)
G1 Y24.0 F726.35
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 24===
G1 Z25 F3900

G1 X8.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 33.65)
G1 Y24.0 F608.75
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 25===
G1 Z25 F3900

G1 X11.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 48.4)
G1 Y24.0 F406.1
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 26===
G1 Z25 F3900

G1 X14.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 49.35)
G1 Y24.0 F594.0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 27===
G1 Z25 F3900

G1 X17.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 36.1)
G1 Y24.0 F565.0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 28===
G1 Z25 F3900

G1 X20.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 44.15)
G1 Y24.0 F705.65
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 29===
G1 Z25 F3900

G1 X23.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 56.3)
G1 Y24.0 F297.35
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 30===
G1 Z25 F3900

G1 X26.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 47.2)
G1 Y24.0 F521.0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 31===
G1 Z25 F3900

;===Adjusting deposition rate===
M205 (H_0_V_0.5) ; Feed rate for hopper 1
M205 (H_1_V_2.5) ; Argon carrier gas flow rate hopper 1
M205 (H_2_V_0.0) ; Feed rate for hopper 2
M205 (H_3_V_2.5) ; Argon carrier gas flow rate hopper 2
G4 P30 ; powder stabilization

G1 X29.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 37.8)
G1 Y24.0 F651.85
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 32===
G1 Z25 F3900

G1 X32.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 38.75)
G1 Y24.0 F242.9
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 33===
G1 Z25 F3900

G1 X35.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 40.7)
G1 Y24.0 F693.55
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 34===
G1 Z25 F3900

G1 X38.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 32.55)
G1 Y24.0 F358.75
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 35===
G1 Z25 F3900

G1 X41.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 52.45)
G1 Y24.0 F349.15
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 36===
G1 Z25 F3900

G1 X44.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 42.65)
G1 Y24.0 F280.2
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 37===
G1 Z25 F3900

G1 X47.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 50.1)
G1 Y24.0 F629.05
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 38===
G1 Z25 F3900

G1 X50.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 43.5)
G1 Y24.0 F546.3
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 39===
G1 Z25 F3900

G1 X53.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 40.0)
G1 Y24.0 F389.7
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 40===
G1 Z25 F3900

G1 X56.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 35.05)
G1 Y24.0 F509.4
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 41===
G1 Z25 F3900

G1 X59.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 41.4)
G1 Y24.0 F434.55
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 42===
G1 Z25 F3900

G1 X62.0 Y14.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 58.25)
G1 Y24.0 F557.85
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 43===
G1 Z25 F3900

G1 X2.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 54.0)
G1 Y36.0 F257.7
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 44===
G1 Z25 F3900

G1 X5.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 34.35)
G1 Y36.0 F318.6
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 45===
G1 Z25 F3900

G1 X8.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 47.0)
G1 Y36.0 F472.05
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 46===
G1 Z25 F3900

G1 X11.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 51.35)
G1 Y36.0 F377.4
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 47===
G1 Z25 F3900

G1 X14.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 31.7)
G1 Y36.0 F636.75
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 48===
G1 Z25 F3900

G1 X17.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 59.8)
G1 Y36.0 F451.7
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 49===
G1 Z25 F3900

G1 X20.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 55.25)
G1 Y36.0 F666.3
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 50===
G1 Z25 F3900

G1 X23.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 45.9)
G1 Y36.0 F331.15
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 51===
G1 Z25 F3900

G1 X26.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 31.0)
G1 Y36.0 F419.85
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 52===
G1 Z25 F3900

G1 X29.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 57.75)
G1 Y36.0 F485.55
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 53===
G1 Z25 F3900

G1 X32.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 54.3)
G1 Y36.0 F726.35
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 54===
G1 Z25 F3900

G1 X35.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 33.65)
G1 Y36.0 F608.75
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 55===
G1 Z25 F3900

G1 X38.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 48.4)
G1 Y36.0 F406.1
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 56===
G1 Z25 F3900

G1 X41.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 49.35)
G1 Y36.0 F594.0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 57===
G1 Z25 F3900

G1 X44.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 36.1)
G1 Y36.0 F565.0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 58===
G1 Z25 F3900

G1 X47.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 44.15)
G1 Y36.0 F705.65
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 59===
G1 Z25 F3900

G1 X50.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 56.3)
G1 Y36.0 F297.35
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)

G4 P0 ; add cooldown between objects

;===Starting Single Track 60===
G1 Z25 F3900

G1 X53.0 Y26.0
G1 Z0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 47.2)
G1 Y36.0 F521.0
G4 P0.001 ; added because G1 is being skipped
M201 (SDC 0)
G4 P0.001 ; added because G1 is being skipped

M201 (EMOFF) ; turn off the laser
M65 P3 ; stop Argon purge gas
M65 P2 ; stop fume extractor
M205 (H_0_V_0) ; turn off hopper 1
M205 (H_1_V_0) ; turn off hopper 1 carrier gas
M205 (H_2_V_0) ; turn off hopper 2
M205 (H_3_V_0) ; turn off hopper 2 carrier gas
