/**
 * MK4duo 3D Printer Firmware
 *
 * Based on Marlin, Sprinter and grbl
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (C) 2013 - 2017 Alberto Cotronei @MagoKimbra
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * conditionals_post.h
 * Defines that depend on configuration but are not editable.
 */

#ifndef _CONDITIONALS_POST_H
#define _CONDITIONALS_POST_H

  /**
   * BLUETOOTH
   */
  #if ENABLED(BLUETOOTH)
    #undef SERIAL_PORT
    #define SERIAL_PORT BLUETOOTH_PORT
    #undef BAUDRATE
    #define BAUDRATE BLUETOOTH_BAUD
  #endif

  /**
   * SAM3X8E
   */
  #if ENABLED(ARDUINO_ARCH_SAM)
    #ifdef M100_FREE_MEMORY_WATCHER
      #undef M100_FREE_MEMORY_WATCHER
    #endif
    #if ENABLED(EMERGENCY_PARSER)
      #undef EMERGENCY_PARSER
    #endif
    #if MINIMUM_STEPPER_PULSE == 0
      #undef MINIMUM_STEPPER_PULSE
      #define MINIMUM_STEPPER_PULSE 1
    #endif
  #endif

  /**
   * Axis lengths and center
   */
  #define X_MAX_LENGTH (X_MAX_POS - (X_MIN_POS))
  #define Y_MAX_LENGTH (Y_MAX_POS - (Y_MIN_POS))
  #define Z_MAX_LENGTH (Z_MAX_POS - (Z_MIN_POS))
  #define X_CENTER float((X_MIN_POS + X_MAX_POS) * 0.5)
  #define Y_CENTER float((Y_MIN_POS + Y_MAX_POS) * 0.5)
  #define Z_CENTER float((Z_MIN_POS + Z_MAX_POS) * 0.5)

  /**
   * CoreXY, CoreXZ, and CoreYZ - and their reverse
   */
  #if IS_CORE
    #if CORE_IS_XY
      #define CORE_AXIS_1 A_AXIS
      #define CORE_AXIS_2 B_AXIS
      #define NORMAL_AXIS Z_AXIS
    #elif CORE_IS_XZ
      #define CORE_AXIS_1 A_AXIS
      #define NORMAL_AXIS Y_AXIS
      #define CORE_AXIS_2 C_AXIS
    #elif CORE_IS_YZ
      #define NORMAL_AXIS X_AXIS
      #define CORE_AXIS_1 B_AXIS
      #define CORE_AXIS_2 C_AXIS
    #endif
    #if (MECH(COREXY) || MECH(COREXZ) || MECH(COREYZ))
      #define CORESIGN(n) (n)
    #else
      #define CORESIGN(n) (-(n))
    #endif
  #endif

  /**
   * SCARA cannot use SLOWDOWN and requires QUICKHOME
   */
  #if IS_SCARA
    #undef SLOWDOWN
    #define QUICK_HOME
  #endif

   /**
   * Set the home position based on settings or manual overrides
   */
  #ifdef MANUAL_X_HOME_POS
    #define X_HOME_POS MANUAL_X_HOME_POS
  #elif ENABLED(BED_CENTER_AT_0_0)
    #if MECH(DELTA)
      #define X_HOME_POS 0
    #else
      #define X_HOME_POS ((X_MAX_LENGTH) * (X_HOME_DIR) * 0.5)
    #endif
  #else
    #if MECH(DELTA)
      #define X_HOME_POS (X_MIN_POS + (X_MAX_LENGTH) * 0.5)
    #else
      #define X_HOME_POS (X_HOME_DIR < 0 ? X_MIN_POS : X_MAX_POS)
    #endif
  #endif

  #ifdef MANUAL_Y_HOME_POS
    #define Y_HOME_POS MANUAL_Y_HOME_POS
  #elif ENABLED(BED_CENTER_AT_0_0)
    #if MECH(DELTA)
      #define Y_HOME_POS 0
    #else
      #define Y_HOME_POS ((Y_MAX_LENGTH) * (Y_HOME_DIR) * 0.5)
    #endif
  #else
    #if MECH(DELTA)
      #define Y_HOME_POS (Y_MIN_POS + (Y_MAX_LENGTH) * 0.5)
    #else
      #define Y_HOME_POS (Y_HOME_DIR < 0 ? Y_MIN_POS : Y_MAX_POS)
    #endif
  #endif

  #ifdef MANUAL_Z_HOME_POS
    #define Z_HOME_POS MANUAL_Z_HOME_POS
  #else
    #define Z_HOME_POS (Z_HOME_DIR < 0 ? Z_MIN_POS : Z_MAX_POS)
  #endif

  /**
   * Auto Bed Leveling and Z Probe Repeatability Test
   */
  #define HOMING_Z_WITH_PROBE (HAS_BED_PROBE && Z_HOME_DIR < 0)

  /**
   * Shorthand for pin tests, used wherever needed
   */

  // Steppers
  #define HAS_X_ENABLE      (PIN_EXISTS(X_ENABLE))
  #define HAS_X_DIR         (PIN_EXISTS(X_DIR))
  #define HAS_X_STEP        (PIN_EXISTS(X_STEP))
  #define HAS_X_MICROSTEPS  (ENABLED(USE_MICROSTEPS) && PIN_EXISTS(X_MS1))

  #define HAS_X2_ENABLE     (PIN_EXISTS(X2_ENABLE))
  #define HAS_X2_DIR        (PIN_EXISTS(X2_DIR))
  #define HAS_X2_STEP       (PIN_EXISTS(X2_STEP))

  #define HAS_Y_ENABLE      (PIN_EXISTS(Y_ENABLE))
  #define HAS_Y_DIR         (PIN_EXISTS(Y_DIR))
  #define HAS_Y_STEP        (PIN_EXISTS(Y_STEP))
  #define HAS_Y_MICROSTEPS  (ENABLED(USE_MICROSTEPS) && PIN_EXISTS(Y_MS1))

  #define HAS_Y2_ENABLE     (PIN_EXISTS(Y2_ENABLE))
  #define HAS_Y2_DIR        (PIN_EXISTS(Y2_DIR))
  #define HAS_Y2_STEP       (PIN_EXISTS(Y2_STEP))

  #define HAS_Z_ENABLE      (PIN_EXISTS(Z_ENABLE))
  #define HAS_Z_DIR         (PIN_EXISTS(Z_DIR))
  #define HAS_Z_STEP        (PIN_EXISTS(Z_STEP))
  #define HAS_Z_MICROSTEPS  (ENABLED(USE_MICROSTEPS) && PIN_EXISTS(Z_MS1))

  #define HAS_Z2_ENABLE     (PIN_EXISTS(Z2_ENABLE))
  #define HAS_Z2_DIR        (PIN_EXISTS(Z2_DIR))
  #define HAS_Z2_STEP       (PIN_EXISTS(Z2_STEP))

  #define HAS_Z3_ENABLE     (PIN_EXISTS(Z3_ENABLE))
  #define HAS_Z3_DIR        (PIN_EXISTS(Z3_DIR))
  #define HAS_Z3_STEP       (PIN_EXISTS(Z3_STEP))

  #define HAS_Z4_ENABLE     (PIN_EXISTS(Z4_ENABLE))
  #define HAS_Z4_DIR        (PIN_EXISTS(Z4_DIR))
  #define HAS_Z4_STEP       (PIN_EXISTS(Z4_STEP))

  // Extruder steppers and solenoids
  #define HAS_E0_ENABLE     (PIN_EXISTS(E0_ENABLE))
  #define HAS_E0_DIR        (PIN_EXISTS(E0_DIR))
  #define HAS_E0_STEP       (PIN_EXISTS(E0_STEP))
  #define HAS_E0_MICROSTEPS (ENABLED(USE_MICROSTEPS) && PIN_EXISTS(E0_MS1))
  #define HAS_SOLENOID_0    (PIN_EXISTS(SOL0))

  #define HAS_E1_ENABLE     (PIN_EXISTS(E1_ENABLE))
  #define HAS_E1_DIR        (PIN_EXISTS(E1_DIR))
  #define HAS_E1_STEP       (PIN_EXISTS(E1_STEP))
  #define HAS_E1_MICROSTEPS (ENABLED(USE_MICROSTEPS) && PIN_EXISTS(E1_MS1))
  #define HAS_SOLENOID_1    (PIN_EXISTS(SOL1))

  #define HAS_E2_ENABLE     (PIN_EXISTS(E2_ENABLE))
  #define HAS_E2_DIR        (PIN_EXISTS(E2_DIR))
  #define HAS_E2_STEP       (PIN_EXISTS(E2_STEP))
  #define HAS_E2_MICROSTEPS (ENABLED(USE_MICROSTEPS) && PIN_EXISTS(E2_MS1))
  #define HAS_SOLENOID_2    (PIN_EXISTS(SOL2))

  #define HAS_E3_ENABLE     (PIN_EXISTS(E3_ENABLE))
  #define HAS_E3_DIR        (PIN_EXISTS(E3_DIR))
  #define HAS_E3_STEP       (PIN_EXISTS(E3_STEP))
  #define HAS_E3_MICROSTEPS (PIN_EXISTS(E3_MS1))
  #define HAS_SOLENOID_3    (PIN_EXISTS(SOL3))

  #define HAS_E4_ENABLE     (PIN_EXISTS(E4_ENABLE))
  #define HAS_E4_DIR        (PIN_EXISTS(E4_DIR))
  #define HAS_E4_STEP       (PIN_EXISTS(E4_STEP))
  #define HAS_E4_MICROSTEPS (PIN_EXISTS(E4_MS1))
  #define HAS_SOLENOID_4    (PIN_EXISTS(SOL4))

  #define HAS_E5_ENABLE     (PIN_EXISTS(E5_ENABLE))
  #define HAS_E5_DIR        (PIN_EXISTS(E5_DIR))
  #define HAS_E5_STEP       (PIN_EXISTS(E5_STEP))
  #define HAS_E5_MICROSTEPS (PIN_EXISTS(E5_MS1))
  #define HAS_SOLENOID_5    (PIN_EXISTS(SOL5))

  // Endstops and bed probe
  #define HAS_X_MIN         (PIN_EXISTS(X_MIN))
  #define HAS_X_MAX         (PIN_EXISTS(X_MAX))
  #define HAS_Y_MIN         (PIN_EXISTS(Y_MIN))
  #define HAS_Y_MAX         (PIN_EXISTS(Y_MAX))
  #define HAS_Z_MIN         (PIN_EXISTS(Z_MIN))
  #define HAS_Z2_MIN        (PIN_EXISTS(Z2_MIN))
  #define HAS_Z3_MIN        (PIN_EXISTS(Z3_MIN))
  #define HAS_Z4_MIN        (PIN_EXISTS(Z4_MIN))
  #define HAS_Z_MAX         (PIN_EXISTS(Z_MAX))
  #define HAS_Z2_MAX        (PIN_EXISTS(Z2_MAX))
  #define HAS_Z3_MAX        (PIN_EXISTS(Z3_MAX))
  #define HAS_Z4_MAX        (PIN_EXISTS(Z4_MAX))
  #define HAS_Z_PROBE_PIN   (PIN_EXISTS(Z_PROBE))
  #define HAS_E_MIN         (PIN_EXISTS(E_MIN))

  // Utility
  #define HAS_DOOR          (ENABLED(DOOR_OPEN) && PIN_EXISTS(DOOR))
  #define HAS_POWER_CHECK   (ENABLED(POWER_CHECK) && PIN_EXISTS(POWER_CHECK))

  // Thermistors
  #define HAS_TEMP_0        (PIN_EXISTS(TEMP_0) && TEMP_SENSOR_0 != 0 && TEMP_SENSOR_0 > -2)
  #define HAS_TEMP_1        (PIN_EXISTS(TEMP_1) && TEMP_SENSOR_1 != 0 && TEMP_SENSOR_1 > -2)
  #define HAS_TEMP_2        (PIN_EXISTS(TEMP_2) && TEMP_SENSOR_2 != 0 && TEMP_SENSOR_2 > -2)
  #define HAS_TEMP_3        (PIN_EXISTS(TEMP_3) && TEMP_SENSOR_3 != 0 && TEMP_SENSOR_3 > -2)
  #define HAS_TEMP_HOTEND   (HAS_TEMP_0 || ENABLED(HEATER_0_USES_MAX6675))
  #define HAS_TEMP_BED      (PIN_EXISTS(TEMP_BED) && TEMP_SENSOR_BED != 0 && TEMP_SENSOR_BED > -2)
  #define HAS_TEMP_CHAMBER  (PIN_EXISTS(TEMP_CHAMBER) && TEMP_SENSOR_CHAMBER != 0 && TEMP_SENSOR_CHAMBER > -2)
  #define HAS_TEMP_COOLER   (PIN_EXISTS(TEMP_COOLER) && TEMP_SENSOR_COOLER != 0 && TEMP_SENSOR_COOLER > -2)

  // Heaters
  #define HAS_HEATER_0      (PIN_EXISTS(HEATER_0))
  #define HAS_HEATER_1      (PIN_EXISTS(HEATER_1))
  #define HAS_HEATER_2      (PIN_EXISTS(HEATER_2))
  #define HAS_HEATER_3      (PIN_EXISTS(HEATER_3))
  #define HAS_HEATER_BED    (PIN_EXISTS(HEATER_BED))
  #define HAS_HEATER_CHAMBER (PIN_EXISTS(HEATER_CHAMBER))
  #define HAS_COOLER        (PIN_EXISTS(COOLER))
  #define HAS_CNCROUTER     (PIN_EXISTS(CNCROUTER))

  // Thermal protection
  #define HAS_THERMALLY_PROTECTED_BED     (HAS_TEMP_BED && HAS_HEATER_BED && ENABLED(THERMAL_PROTECTION_BED))
  #define HAS_THERMALLY_PROTECTED_CHAMBER (HAS_TEMP_CHAMBER && HAS_HEATER_CHAMBER && ENABLED(THERMAL_PROTECTION_CHAMBER))
  #define HAS_THERMALLY_PROTECTED_COOLER  (HAS_TEMP_COOLER && HAS_COOLER && ENABLED(THERMAL_PROTECTION_COOLER))
  #define WATCH_HOTENDS     (ENABLED(THERMAL_PROTECTION_HOTENDS) && WATCH_TEMP_PERIOD > 0)
  #define WATCH_THE_BED     (HAS_THERMALLY_PROTECTED_BED && WATCH_BED_TEMP_PERIOD > 0)
  #define WATCH_THE_CHAMBER (HAS_THERMALLY_PROTECTED_CHAMBER && WATCH_CHAMBER_TEMP_PERIOD > 0)
  #define WATCH_THE_COOLER  (HAS_THERMALLY_PROTECTED_COOLER && WATCH_COOLER_TEMP_PERIOD > 0)

  // Auto fans
  #define HAS_AUTO_FAN_0    (ENABLED(HOTEND_AUTO_FAN) && PIN_EXISTS(H0_AUTO_FAN))
  #define HAS_AUTO_FAN_1    (ENABLED(HOTEND_AUTO_FAN) && PIN_EXISTS(H1_AUTO_FAN))
  #define HAS_AUTO_FAN_2    (ENABLED(HOTEND_AUTO_FAN) && PIN_EXISTS(H2_AUTO_FAN))
  #define HAS_AUTO_FAN_3    (ENABLED(HOTEND_AUTO_FAN) && PIN_EXISTS(H3_AUTO_FAN))
  #define HAS_AUTO_FAN      (HAS_AUTO_FAN_0 || HAS_AUTO_FAN_1 || HAS_AUTO_FAN_2 || HAS_AUTO_FAN_3)
  #define AUTO_1_IS_0       (H1_AUTO_FAN_PIN == H0_AUTO_FAN_PIN)
  #define AUTO_2_IS_0       (H2_AUTO_FAN_PIN == H0_AUTO_FAN_PIN)
  #define AUTO_2_IS_1       (H2_AUTO_FAN_PIN == H1_AUTO_FAN_PIN)
  #define AUTO_3_IS_0       (H3_AUTO_FAN_PIN == H0_AUTO_FAN_PIN)
  #define AUTO_3_IS_1       (H3_AUTO_FAN_PIN == H1_AUTO_FAN_PIN)
  #define AUTO_3_IS_2       (H3_AUTO_FAN_PIN == H2_AUTO_FAN_PIN)

  // Other fans
  #define HAS_FAN0          (PIN_EXISTS(FAN))
  #define HAS_FAN1          (PIN_EXISTS(FAN1) && CONTROLLERFAN_PIN != FAN1_PIN && H0_AUTO_FAN_PIN != FAN1_PIN && H1_AUTO_FAN_PIN != FAN1_PIN && H2_AUTO_FAN_PIN != FAN1_PIN && H3_AUTO_FAN_PIN != FAN1_PIN)
  #define HAS_FAN2          (PIN_EXISTS(FAN2) && CONTROLLERFAN_PIN != FAN2_PIN && H0_AUTO_FAN_PIN != FAN2_PIN && H1_AUTO_FAN_PIN != FAN2_PIN && H2_AUTO_FAN_PIN != FAN2_PIN && H3_AUTO_FAN_PIN != FAN2_PIN)
  #define HAS_FAN3          (PIN_EXISTS(FAN3) && CONTROLLERFAN_PIN != FAN3_PIN && H0_AUTO_FAN_PIN != FAN3_PIN && H1_AUTO_FAN_PIN != FAN3_PIN && H2_AUTO_FAN_PIN != FAN3_PIN && H3_AUTO_FAN_PIN != FAN3_PIN)
  #define HAS_CONTROLLERFAN (ENABLED(CONTROLLERFAN) && PIN_EXISTS(CONTROLLERFAN))

  // Servos
  #define HAS_SERVO_0       (PIN_EXISTS(SERVO0))
  #define HAS_SERVO_1       (PIN_EXISTS(SERVO1))
  #define HAS_SERVO_2       (PIN_EXISTS(SERVO2))
  #define HAS_SERVO_3       (PIN_EXISTS(SERVO3))
  #define HAS_SERVOS        ((ENABLED(ENABLE_SERVOS) && NUM_SERVOS > 0) && (HAS_SERVO_0 || HAS_SERVO_1 || HAS_SERVO_2 || HAS_SERVO_3))

  // Sensors
  #define HAS_FILAMENT_SENSOR           (ENABLED(FILAMENT_SENSOR) && PIN_EXISTS(FILWIDTH))
  #define HAS_FIL_RUNOUT                (ENABLED(FILAMENT_RUNOUT_SENSOR) && PIN_EXISTS(FIL_RUNOUT))
  #define HAS_POWER_CONSUMPTION_SENSOR  (ENABLED(POWER_CONSUMPTION) && PIN_EXISTS(POWER_CONSUMPTION))

  // User Interface
  #define HAS_HOME          (PIN_EXISTS(HOME))
  #define HAS_KILL          (PIN_EXISTS(KILL))
  #define HAS_SUICIDE       (PIN_EXISTS(SUICIDE))
  #define HAS_CHDK          (ENABLED(CHDK) && PIN_EXISTS(CHDK))
  #define HAS_PHOTOGRAPH    (ENABLED(PHOTOGRAPH) && PIN_EXISTS(PHOTOGRAPH))
  #define HAS_BUZZER        (PIN_EXISTS(BEEPER) || ENABLED(LCD_USE_I2C_BUZZER))
  #define HAS_CASE_LIGHT    (ENABLED(CASE_LIGHT) && PIN_EXISTS(CASE_LIGHT))

  // Digital control
  #define HAS_MICROSTEPS    (HAS_X_MICROSTEPS     \
                            || HAS_Y_MICROSTEPS   \
                            || HAS_Z_MICROSTEPS   \
                            || HAS_E0_MICROSTEPS  \
                            || HAS_E1_MICROSTEPS  \
                            || HAS_E2_MICROSTEPS  \
                            || HAS_E3_MICROSTEPS  \
                            || HAS_E4_MICROSTEPS  \
                            || HAS_E5_MICROSTEPS)
  #define HAS_STEPPER_RESET (PIN_EXISTS(STEPPER_RESET))
  #define HAS_DIGIPOTSS     (PIN_EXISTS(DIGIPOTSS))
  #define HAS_MOTOR_CURRENT_PWM_XY  (PIN_EXISTS(MOTOR_CURRENT_PWM_XY))
  #define HAS_MOTOR_CURRENT_PWM     (PIN_EXISTS(MOTOR_CURRENT_PWM_XY) || PIN_EXISTS(MOTOR_CURRENT_PWM_Z) || PIN_EXISTS(MOTOR_CURRENT_PWM_E))

  // MKR4 or NKR6
  #define HAS_E0E1          (PIN_EXISTS(E0E1_CHOICE))
  #define HAS_E0E2          (PIN_EXISTS(E0E2_CHOICE))
  #define HAS_E1E3          (PIN_EXISTS(E1E3_CHOICE))
  #define HAS_EX1           (PIN_EXISTS(EX1_CHOICE))
  #define HAS_EX2           (PIN_EXISTS(EX2_CHOICE))

  // Dondolo
  #define HAS_DONDOLO       (ENABLED(DONDOLO_SINGLE_MOTOR) || ENABLED(DONDOLO_DUAL_MOTOR))

  // LCD
  #define HAS_LCD           (ENABLED(ULTRA_LCD) || ENABLED(NEXTION))
  #define HAS_BTN_BACK      (PIN_EXISTS(BTN_BACK))

  // SD support
  #define HAS_SDSUPPORT     (ENABLED(SDSUPPORT))
  #define HAS_EEPROM_SD     (ENABLED(EEPROM_SD) && ENABLED(SDSUPPORT))

  // Other
  #define HAS_Z_PROBE_SLED  (ENABLED(Z_PROBE_SLED) && PIN_EXISTS(SLED))
  
  /**
   * Shorthand for filament sensor and power sensor for ultralcd.cpp, dogm_lcd_implementation.h, ultralcd_implementation_hitachi_HD44780.h
   */
  #define HAS_LCD_FILAMENT_SENSOR (HAS_FILAMENT_SENSOR && ENABLED(FILAMENT_LCD_DISPLAY))
  #define HAS_LCD_POWER_SENSOR (HAS_POWER_CONSUMPTION_SENSOR && ENABLED(POWER_CONSUMPTION_LCD_DISPLAY))

  /**
   * Flags for PID handling
   */
  #define HAS_PID_HEATING (ENABLED(PIDTEMP) || ENABLED(PIDTEMPBED) || ENABLED(PIDTEMPCHAMBER))
  #define HAS_PID_FOR_BOTH (ENABLED(PIDTEMP) && ENABLED(PIDTEMPBED))
  #define HAS_PID_COOLING (ENABLED(PIDTEMPCOOLER))

  /**
   * ENDSTOPPULLUPS
   */
  #if ENABLED(ENDSTOPPULLUPS)
    #define ENDSTOPPULLUP_XMIN
    #define ENDSTOPPULLUP_YMIN
    #define ENDSTOPPULLUP_ZMIN
    #define ENDSTOPPULLUP_Z2MIN
    #define ENDSTOPPULLUP_XMAX
    #define ENDSTOPPULLUP_YMAX
    #define ENDSTOPPULLUP_ZMAX
    #define ENDSTOPPULLUP_Z2MAX
    #define ENDSTOPPULLUP_ZPROBE
    #define ENDSTOPPULLUP_EMIN
  #endif

  /**
   * ENDSTOP LOGICAL
   */
  #if MB(ALLIGATOR) || MB(ALLIGATOR_V3)
    #define X_MIN_ENDSTOP_INVERTING   !X_MIN_ENDSTOP_LOGIC
    #define Y_MIN_ENDSTOP_INVERTING   !Y_MIN_ENDSTOP_LOGIC
    #define Z_MIN_ENDSTOP_INVERTING   !Z_MIN_ENDSTOP_LOGIC
    #define Z2_MIN_ENDSTOP_INVERTING  !Z2_MIN_ENDSTOP_LOGIC
    #define Z3_MIN_ENDSTOP_INVERTING  !Z3_MIN_ENDSTOP_LOGIC
    #define Z4_MIN_ENDSTOP_INVERTING  !Z4_MIN_ENDSTOP_LOGIC
    #define X_MAX_ENDSTOP_INVERTING   !X_MAX_ENDSTOP_LOGIC
    #define Y_MAX_ENDSTOP_INVERTING   !Y_MAX_ENDSTOP_LOGIC
    #define Z_MAX_ENDSTOP_INVERTING   !Z_MAX_ENDSTOP_LOGIC
    #define Z2_MAX_ENDSTOP_INVERTING  !Z2_MAX_ENDSTOP_LOGIC
    #define Z3_MAX_ENDSTOP_INVERTING  !Z3_MAX_ENDSTOP_LOGIC
    #define Z4_MAX_ENDSTOP_INVERTING  !Z4_MAX_ENDSTOP_LOGIC
    #define Z_PROBE_ENDSTOP_INVERTING !Z_PROBE_ENDSTOP_LOGIC
    #define E_MIN_ENDSTOP_INVERTING   !E_MIN_ENDSTOP_LOGIC
    #define DOOR_PIN_INVERTING        !DOOR_OPEN_LOGIC
    #define POWER_CHECK_PIN_INVERTING !POWER_CHECK_LOGIC
    #undef  ENDSTOPPULLUP_XMIN
    #undef  ENDSTOPPULLUP_YMIN
    #undef  ENDSTOPPULLUP_ZMIN
    #undef  ENDSTOPPULLUP_Z2MIN
    #undef  ENDSTOPPULLUP_Z3MIN
    #undef  ENDSTOPPULLUP_Z4MIN
    #undef  ENDSTOPPULLUP_XMAX
    #undef  ENDSTOPPULLUP_YMAX
    #undef  ENDSTOPPULLUP_ZMAX
    #undef  ENDSTOPPULLUP_Z2MAX
    #undef  ENDSTOPPULLUP_Z3MAX
    #undef  ENDSTOPPULLUP_Z4MAX
    #undef  ENDSTOPPULLUP_ZPROBE
    #undef  ENDSTOPPULLUP_EMIN
  #else
    #define X_MIN_ENDSTOP_INVERTING   X_MIN_ENDSTOP_LOGIC
    #define Y_MIN_ENDSTOP_INVERTING   Y_MIN_ENDSTOP_LOGIC
    #define Z_MIN_ENDSTOP_INVERTING   Z_MIN_ENDSTOP_LOGIC
    #define Z2_MIN_ENDSTOP_INVERTING  Z2_MIN_ENDSTOP_LOGIC
    #define Z3_MIN_ENDSTOP_INVERTING  Z3_MIN_ENDSTOP_LOGIC
    #define Z4_MIN_ENDSTOP_INVERTING  Z4_MIN_ENDSTOP_LOGIC
    #define X_MAX_ENDSTOP_INVERTING   X_MAX_ENDSTOP_LOGIC
    #define Y_MAX_ENDSTOP_INVERTING   Y_MAX_ENDSTOP_LOGIC
    #define Z_MAX_ENDSTOP_INVERTING   Z_MAX_ENDSTOP_LOGIC
    #define Z2_MAX_ENDSTOP_INVERTING  Z2_MAX_ENDSTOP_LOGIC
    #define Z3_MAX_ENDSTOP_INVERTING  Z3_MAX_ENDSTOP_LOGIC
    #define Z4_MAX_ENDSTOP_INVERTING  Z4_MAX_ENDSTOP_LOGIC
    #define Z_PROBE_ENDSTOP_INVERTING Z_PROBE_ENDSTOP_LOGIC
    #define E_MIN_ENDSTOP_INVERTING   E_MIN_ENDSTOP_LOGIC
    #define DOOR_PIN_INVERTING        DOOR_OPEN_LOGIC
    #define POWER_CHECK_PIN_INVERTING POWER_CHECK_LOGIC
  #endif

  /**
   * DELTA
   */
  #if MECH(DELTA)
    #undef SLOWDOWN       // DELTA not needs SLOWDOWN
    #undef Z_SAFE_HOMING  // DELTA non needs Z_SAFE_HOMING

    // DELTA must have same valour for 3 axis endstop hits
    #define X_HOME_BUMP_MM XYZ_HOME_BUMP_MM
    #define Y_HOME_BUMP_MM XYZ_HOME_BUMP_MM
    #define Z_HOME_BUMP_MM XYZ_HOME_BUMP_MM
    #define HOMING_BUMP_DIVISOR {XYZ_BUMP_DIVISOR, XYZ_BUMP_DIVISOR, XYZ_BUMP_DIVISOR}

    // Effective horizontal distance bridged by diagonal push rods.
    #define DEFAULT_DELTA_RADIUS (DELTA_SMOOTH_ROD_OFFSET - DELTA_EFFECTOR_OFFSET - DELTA_CARRIAGE_OFFSET)

    #if ENABLED(AUTO_BED_LEVELING_FEATURE)
      #define AUTO_BED_LEVELING_BILINEAR
      #define MIN_PROBE_EDGE 10
    #endif

    #define Z_PROBE_SPEED_FAST  Z_PROBE_SPEED
    #define Z_PROBE_SPEED_SLOW  Z_PROBE_SPEED

    // Set the rectangle in which to probe
    #define DELTA_PROBEABLE_RADIUS    (DELTA_PRINTABLE_RADIUS - 5)
    #define LEFT_PROBE_BED_POSITION   -(DELTA_PROBEABLE_RADIUS)
    #define RIGHT_PROBE_BED_POSITION  (DELTA_PROBEABLE_RADIUS)
    #define FRONT_PROBE_BED_POSITION  -(DELTA_PROBEABLE_RADIUS)
    #define BACK_PROBE_BED_POSITION   (DELTA_PROBEABLE_RADIUS)

    #if ENABLED(WORKSPACE_OFFSETS)
      #undef WORKSPACE_OFFSETS
    #endif

    #define HAS_DELTA_AUTO_CALIBRATION  (ENABLED(DELTA_AUTO_CALIBRATION_1) || ENABLED(DELTA_AUTO_CALIBRATION_2) || ENABLED(DELTA_AUTO_CALIBRATION_3))

  #endif // MECH(DELTA)

  /**
   * The BLTouch Probe emulates a servo probe
   */
  #if ENABLED(BLTOUCH)
    #if HAS(Z_PROBE_PIN)
      #define TEST_BLTOUCH() _TEST_BLTOUCH(Z_PROBE)
    #else
      #define TEST_BLTOUCH() _TEST_BLTOUCH(Z_MIN)
    #endif
  #endif

  /**
   * Set granular options based on the specific type of leveling
   */
  #define ABL_PLANAR (ENABLED(AUTO_BED_LEVELING_LINEAR) || ENABLED(AUTO_BED_LEVELING_3POINT))
  #define ABL_GRID   (ENABLED(AUTO_BED_LEVELING_LINEAR) || ENABLED(AUTO_BED_LEVELING_BILINEAR))
  #define HAS_ABL    (ABL_PLANAR || ABL_GRID)

  #define HAS_LEVELING          (HAS_ABL || ENABLED(MESH_BED_LEVELING))
  #define HAS_PROBING_PROCEDURE (HAS_ABL || ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST))

  #if HAS_PROBING_PROCEDURE
    #define PROBE_BED_WIDTH abs(RIGHT_PROBE_BED_POSITION - (LEFT_PROBE_BED_POSITION))
    #define PROBE_BED_HEIGHT abs(BACK_PROBE_BED_POSITION - (FRONT_PROBE_BED_POSITION))
  #endif

  /**
   * Set GRID MAX POINTS
   */
  #if ENABLED(GRID_MAX_POINTS_X) && ENABLED(GRID_MAX_POINTS_Y)
    #define GRID_MAX_POINTS ((GRID_MAX_POINTS_X) * (GRID_MAX_POINTS_Y))
  #endif

  /**
   * Auto Bed Leveling
   */
  #if IS_KINEMATIC
    // Check for this in the code instead
    #define MIN_PROBE_X X_MIN_POS
    #define MAX_PROBE_X X_MAX_POS
    #define MIN_PROBE_Y Y_MIN_POS
    #define MAX_PROBE_Y Y_MAX_POS
  #else
    // Boundaries for probing based on set limits
    #define MIN_PROBE_X (max(X_MIN_POS, X_MIN_POS + X_PROBE_OFFSET_FROM_NOZZLE))
    #define MAX_PROBE_X (min(X_MAX_POS, X_MAX_POS + X_PROBE_OFFSET_FROM_NOZZLE))
    #define MIN_PROBE_Y (max(Y_MIN_POS, Y_MIN_POS + Y_PROBE_OFFSET_FROM_NOZZLE))
    #define MAX_PROBE_Y (min(Y_MAX_POS, Y_MAX_POS + Y_PROBE_OFFSET_FROM_NOZZLE))
  #endif

  /**
   * Z probe repetitions
   */
  #if DISABLED(Z_PROBE_REPETITIONS)
    #define Z_PROBE_REPETITIONS 1
  #endif

  /**
   * Sled Options
   */
  #if ENABLED(Z_PROBE_SLED)
    #define Z_SAFE_HOMING
  #endif

  /**
   * Safe Homing Options
   */
  #if ENABLED(Z_SAFE_HOMING)
    #ifndef Z_SAFE_HOMING_X_POINT
      #define Z_SAFE_HOMING_X_POINT ((X_MIN_POS + X_MAX_POS) / 2)
    #endif
    #ifndef Z_SAFE_HOMING_Y_POINT
      #define Z_SAFE_HOMING_Y_POINT ((Y_MIN_POS + Y_MAX_POS) / 2)
    #endif
    #define X_TILT_FULCRUM Z_SAFE_HOMING_X_POINT
    #define Y_TILT_FULCRUM Z_SAFE_HOMING_Y_POINT
  #else
    #define X_TILT_FULCRUM X_HOME_POS
    #define Y_TILT_FULCRUM Y_HOME_POS
  #endif

  /**
   * Host keep alive
   */
  #if DISABLED(DEFAULT_KEEPALIVE_INTERVAL)
    #define DEFAULT_KEEPALIVE_INTERVAL 2
  #endif

  /**
   * DOUBLE_STEP_FREQUENCY for Arduino DUE or Mega
   */
  #if ENABLED(ARDUINO_ARCH_SAM)
    #if ENABLED(ADVANCE) || ENABLED(LIN_ADVANCE)
      #define DOUBLE_STEP_FREQUENCY 60000 // 60KHz
    #else
      #define DOUBLE_STEP_FREQUENCY 80000 // 80Khz
    #endif
  #else
    #define DOUBLE_STEP_FREQUENCY 10000
  #endif

  /**
   * MAX_STEP_FREQUENCY differs for TOSHIBA
   */
  #if ENABLED(CONFIG_STEPPERS_TOSHIBA)
    #define MAX_STEP_FREQUENCY DOUBLE_STEP_FREQUENCY // Max step frequency for Toshiba Stepper Controllers, 96kHz is close to maximum for an Arduino Due
  #else
    #define MAX_STEP_FREQUENCY (DOUBLE_STEP_FREQUENCY * 4L) // Max step frequency for the Due is approx. 330kHz
  #endif

  // MS1 MS2 Stepper Driver Microstepping mode table
  #define MICROSTEP1 LOW,LOW
  #define MICROSTEP2 HIGH,LOW
  #define MICROSTEP4 LOW,HIGH
  #define MICROSTEP8 HIGH,HIGH
  #if MB(ALLIGATOR) || MB(ALLIGATOR_V3)
    #define MICROSTEP16 LOW,LOW
    #define MICROSTEP32 HIGH,HIGH
  #else
    #define MICROSTEP16 HIGH,HIGH
  #endif

  /**
   * Advance calculated values
   */
  #if ENABLED(ADVANCE)
    #define EXTRUSION_AREA (0.25 * (D_FILAMENT) * (D_FILAMENT) * M_PI)
    #define STEPS_PER_CUBIC_MM_E (axis_steps_per_mm[E_AXIS + active_extruder] / (EXTRUSION_AREA))
  #endif

  /**
   * SD DETECT
   *
   */
  #if ENABLED(SD_DISABLED_DETECT)
    #undef SD_DETECT_PIN
    #define SD_DETECT_PIN   -1
  #endif
  #if ENABLED(ULTIPANEL) && DISABLED(ELB_FULL_GRAPHIC_CONTROLLER)
    #undef SD_DETECT_INVERTED
  #endif

  /**
   * Power Signal Control Definitions
   * By default use Normal definition
   */
  #if DISABLED(POWER_SUPPLY)
    #define POWER_SUPPLY 0
  #endif
  #if (POWER_SUPPLY == 1)     // 1 = ATX
    #define PS_ON_AWAKE  LOW
    #define PS_ON_ASLEEP HIGH
  #elif (POWER_SUPPLY == 2)   // 2 = X-Box 360 203W
    #define PS_ON_AWAKE  HIGH
    #define PS_ON_ASLEEP LOW
  #endif
  #if DISABLED(DELAY_AFTER_POWER_ON)
    #define DELAY_AFTER_POWER_ON 5
  #endif
  #define HAS_POWER_SWITCH (POWER_SUPPLY > 0 && PIN_EXISTS(PS_ON))

  /**
   * Temp Sensor defines
   */
  #if TEMP_SENSOR_0 == -3
    #define HEATER_0_USES_MAX6675
    #define MAX6675_IS_MAX31855
    #define MAX6675_TMIN -270
    #define MAX6675_TMAX 1800
  #elif TEMP_SENSOR_0 == -2
    #define HEATER_0_USES_MAX6675
    #define MAX6675_TMIN 0
    #define MAX6675_TMAX 1024
  #elif TEMP_SENSOR_0 == -1
    #define HEATER_0_USES_AD595
  #elif TEMP_SENSOR_0 == 0
    #undef HEATER_0_MINTEMP
    #undef HEATER_0_MAXTEMP
  #elif TEMP_SENSOR_0 > 0
    #define THERMISTORHEATER_0 TEMP_SENSOR_0
    #define HEATER_0_USES_THERMISTOR
  #endif

  #if TEMP_SENSOR_1 <= -2
    #error "MAX6675 / MAX31855 Thermocouples not supported for TEMP_SENSOR_1"
  #elif TEMP_SENSOR_1 == -1
    #define HEATER_1_USES_AD595
  #elif TEMP_SENSOR_1 == 0
    #undef HEATER_1_MINTEMP
    #undef HEATER_1_MAXTEMP
  #elif TEMP_SENSOR_1 > 0
    #define THERMISTORHEATER_1 TEMP_SENSOR_1
    #define HEATER_1_USES_THERMISTOR
  #endif

  #if TEMP_SENSOR_2 <= -2
    #error "MAX6675 / MAX31855 Thermocouples not supported for TEMP_SENSOR_2"
  #elif TEMP_SENSOR_2 == -1
    #define HEATER_2_USES_AD595
  #elif TEMP_SENSOR_2 == 0
    #undef HEATER_2_MINTEMP
    #undef HEATER_2_MAXTEMP
  #elif TEMP_SENSOR_2 > 0
    #define THERMISTORHEATER_2 TEMP_SENSOR_2
    #define HEATER_2_USES_THERMISTOR
  #endif

  #if TEMP_SENSOR_3 <= -2
    #error "MAX6675 / MAX31855 Thermocouples not supported for TEMP_SENSOR_3"
  #elif TEMP_SENSOR_3 == -1
    #define HEATER_3_USES_AD595
  #elif TEMP_SENSOR_3 == 0
    #undef HEATER_3_MINTEMP
    #undef HEATER_3_MAXTEMP
  #elif TEMP_SENSOR_3 > 0
    #define THERMISTORHEATER_3 TEMP_SENSOR_3
    #define HEATER_3_USES_THERMISTOR
  #endif

  #if TEMP_SENSOR_BED <= -2
    #error "MAX6675 / MAX31855 Thermocouples not supported for TEMP_SENSOR_BED"
  #elif TEMP_SENSOR_BED == -1
    #define BED_USES_AD595
  #elif TEMP_SENSOR_BED == 0
    #undef BED_MINTEMP
    #undef BED_MAXTEMP
  #elif TEMP_SENSOR_BED > 0
    #define THERMISTORBED TEMP_SENSOR_BED
    #define BED_USES_THERMISTOR
  #endif

  #if TEMP_SENSOR_CHAMBER == -1
    #define CHAMBER_USES_AD595
  #elif TEMP_SENSOR_CHAMBER == 0
    #undef CHAMBER_MINTEMP
    #undef CHAMBER_MAXTEMP
  #elif TEMP_SENSOR_CHAMBER > 0
    #define THERMISTORCHAMBER TEMP_SENSOR_CHAMBER
    #define CHAMBER_USES_THERMISTOR
  #endif

  #if TEMP_SENSOR_COOLER == -1
    #define COOLER_USES_AD595
  #elif TEMP_SENSOR_COOLER == 0
    #undef COOLER_MINTEMP
    #undef COOLER_MAXTEMP
  #elif TEMP_SENSOR_COOLER > 0
    #define THERMISTORCOOLER TEMP_SENSOR_COOLER
    #define COOLER_USES_THERMISTOR
  #endif

  #if HASNT(COOLER)
    #if ENABLED(PIDTEMPCOOLER)
      #undef PIDTEMPCOOLER
    #endif
  #endif

  #define HEATER_USES_AD595 (ENABLED(HEATER_0_USES_AD595) || ENABLED(HEATER_1_USES_AD595) || ENABLED(HEATER_2_USES_AD595) || ENABLED(HEATER_3_USES_AD595))

  /**
   * Helper Macros for heaters and extruder fan and rele
   */
  #if ENABLED(INVERTED_HEATER_PINS)
    #define WRITE_HEATER(pin, value) WRITE(pin, !value)
  #else
    #define WRITE_HEATER(pin, value) WRITE(pin, value)
  #endif
  #define WRITE_HEATER_0P(v) WRITE_HEATER(HEATER_0_PIN, v)
  #if HOTENDS > 1 || ENABLED(HEATERS_PARALLEL)
    #define WRITE_HEATER_1(v) WRITE_HEATER(HEATER_1_PIN, v)
    #if HOTENDS > 2
      #define WRITE_HEATER_2(v) WRITE_HEATER(HEATER_2_PIN, v)
      #if HOTENDS > 3
        #define WRITE_HEATER_3(v) WRITE_HEATER(HEATER_3_PIN, v)
      #endif
    #endif
  #endif
  #if ENABLED(HEATERS_PARALLEL)
    #define WRITE_HEATER_0(v) { WRITE_HEATER_0P(v); WRITE_HEATER_1(v); }
  #else
    #define WRITE_HEATER_0(v) WRITE_HEATER_0P(v)
  #endif
  #if HAS(HEATER_BED)
    #if ENABLED(INVERTED_BED_PIN)
      #define WRITE_HEATER_BED(v) WRITE(HEATER_BED_PIN,!v)
    #else
      #define WRITE_HEATER_BED(v) WRITE(HEATER_BED_PIN,v)
    #endif
  #endif
  #if HAS_HEATER_CHAMBER
    #if ENABLED(INVERTED_CHAMBER_PIN)
      #define WRITE_HEATER_CHAMBER(v) WRITE(HEATER_CHAMBER_PIN,!v)
    #else
      #define WRITE_HEATER_CHAMBER(v) WRITE(HEATER_CHAMBER_PIN,v)
    #endif
  #endif
  #if HAS_COOLER
    #if ENABLED(INVERTED_COOLER_PIN)
      #define WRITE_COOLER(v) WRITE(COOLER_PIN,!v)
    #else
      #define WRITE_COOLER(v) WRITE(COOLER_PIN,v)
    #endif
  #endif

  /**
   * Up to 4 PWM fans
   */
  #if HAS(FAN3)
    #define FAN_COUNT 4
  #elif HAS(FAN2)
    #define FAN_COUNT 3
  #elif HAS(FAN1)
    #define FAN_COUNT 2
  #elif HAS(FAN0)
    #define FAN_COUNT 1
  #else
    #define FAN_COUNT 0
  #endif
  #define FAN_LOOP() for (uint8_t f = 0; f < FAN_COUNT; f++)

  #if ENABLED(INVERTED_FAN_PINS)
    #define _WRITE_FAN(pin, v) WRITE(pin, !v)
  #else
    #define _WRITE_FAN(pin, v) WRITE(pin, v)
  #endif

  #if HAS(FAN0)
    #define WRITE_FAN(v) _WRITE_FAN(FAN_PIN, v)
    #define WRITE_FAN0(v) WRITE_FAN(v)
  #endif
  #if HAS(FAN1)
    #define WRITE_FAN1(v) _WRITE_FAN(FAN1_PIN, v)
  #endif
  #if HAS(FAN2)
    #define WRITE_FAN2(v) _WRITE_FAN(FAN2_PIN, v)
  #endif
  #if HAS(FAN3)
    #define WRITE_FAN3(v) _WRITE_FAN(FAN3_PIN, v)
  #endif
  #define WRITE_FAN_N(n, v) WRITE_FAN##n(v)

  #if HAS(CNCROUTER)
    #if ENABLED(INVERTED_CNCROUTER_PIN)
      #define WRITE_CNCROUTER(v) WRITE(CNCROUTER_PIN, !v)
    #else
      #define WRITE_CNCROUTER(v) WRITE(CNCROUTER_PIN, v)
    #endif
  #endif

  #if ENABLED(MKR4) || ENABLED(MKR6)
    #if ENABLED(INVERTED_RELE_PINS)
      #define WRITE_RELE(pin, value) WRITE(pin, !value)
      #define OUT_WRITE_RELE(pin, value) OUT_WRITE(pin, !value)
    #else
      #define WRITE_RELE(pin, value) WRITE(pin, value)
      #define OUT_WRITE_RELE(pin, value) OUT_WRITE(pin, value)
    #endif
  #endif

  /**
   * Buzzer/Speaker
   */
  #if ENABLED(LCD_USE_I2C_BUZZER)
    #ifndef LCD_FEEDBACK_FREQUENCY_HZ
      #define LCD_FEEDBACK_FREQUENCY_HZ 1000
    #endif
    #ifndef LCD_FEEDBACK_FREQUENCY_DURATION_MS
      #define LCD_FEEDBACK_FREQUENCY_DURATION_MS 100
    #endif
  #else
    #ifndef LCD_FEEDBACK_FREQUENCY_HZ
      #define LCD_FEEDBACK_FREQUENCY_HZ 5000
    #endif
    #ifndef LCD_FEEDBACK_FREQUENCY_DURATION_MS
      #define LCD_FEEDBACK_FREQUENCY_DURATION_MS 2
    #endif
  #endif

  /**
   * MIN_Z_HEIGHT_FOR_HOMING / Z_PROBE_BETWEEN_HEIGHT
   */
  #if DISABLED(MIN_Z_HEIGHT_FOR_HOMING)
    #if DISABLED(Z_PROBE_BETWEEN_HEIGHT)
      #define MIN_Z_HEIGHT_FOR_HOMING 0
    #else
      #define MIN_Z_HEIGHT_FOR_HOMING Z_PROBE_BETWEEN_HEIGHT
    #endif
  #endif
  #if DISABLED(Z_PROBE_BETWEEN_HEIGHT)
    #define Z_RAISE_BETWEEN_PROBING MIN_Z_HEIGHT_FOR_HOMING
  #endif
  #if Z_PROBE_BETWEEN_HEIGHT > MIN_Z_HEIGHT_FOR_HOMING
    #define MANUAL_PROBE_HEIGHT Z_PROBE_BETWEEN_HEIGHT
  #else
    #define MANUAL_PROBE_HEIGHT MIN_Z_HEIGHT_FOR_HOMING
  #endif

  /**
   * Servos
   */
  #if HAS(SERVOS)
    #ifndef Z_ENDSTOP_SERVO_NR
      #define Z_ENDSTOP_SERVO_NR -1
    #endif
  #endif

  /**
   * Set a flag for a servo probe
   */
  #define HAS_Z_SERVO_ENDSTOP (HAS_SERVOS && ENABLED(Z_ENDSTOP_SERVO_NR) && Z_ENDSTOP_SERVO_NR >= 0)

  /**
   * Set a flag for any enabled probe
   */
  #define PROBE_SELECTED (ENABLED(PROBE_MANUALLY) || ENABLED(Z_PROBE_FIX_MOUNTED) || ENABLED(Z_PROBE_SLED) || ENABLED(Z_PROBE_ALLEN_KEY) || HAS_Z_SERVO_ENDSTOP)
  #define PROBE_PIN_CONFIGURED (HAS_Z_PROBE_PIN || HAS_Z_MIN)
  #define HAS_BED_PROBE (PROBE_SELECTED && PROBE_PIN_CONFIGURED && DISABLED(PROBE_MANUALLY))

  #if ENABLED(Z_PROBE_ALLEN_KEY)
    #define PROBE_IS_TRIGGERED_WHEN_STOWED_TEST
  #endif

  /**
   * Nextion Manual BED leveling
   */
  #define HAS_NEXTION_MANUAL_BED (ENABLED(LCD_BED_LEVELING) && ENABLED(PROBE_MANUALLY) && ENABLED(NEXTION))

  /**
   * Bed Probe dependencies
   */
  #if HAS_BED_PROBE
    #ifndef X_PROBE_OFFSET_FROM_NOZZLE
      #define X_PROBE_OFFSET_FROM_NOZZLE 0
    #endif
    #ifndef Y_PROBE_OFFSET_FROM_NOZZLE
      #define Y_PROBE_OFFSET_FROM_NOZZLE 0
    #endif
    #ifndef Z_PROBE_OFFSET_FROM_NOZZLE
      #define Z_PROBE_OFFSET_FROM_NOZZLE 0
    #endif
    #ifndef Z_PROBE_OFFSET_RANGE_MIN
      #define Z_PROBE_OFFSET_RANGE_MIN -50
    #endif
    #ifndef Z_PROBE_OFFSET_RANGE_MAX
      #define Z_PROBE_OFFSET_RANGE_MAX 50
    #endif
    #ifndef XY_PROBE_SPEED
      #ifdef HOMING_FEEDRATE_X
        #define XY_PROBE_SPEED HOMING_FEEDRATE_X
      #elif define HOMING_FEEDRATE_XYZ
        #define XY_PROBE_SPEED HOMING_FEEDRATE_XYZ
      #else
        #define XY_PROBE_SPEED 4000
      #endif
    #endif
    #if Z_PROBE_BETWEEN_HEIGHT > Z_PROBE_DEPLOY_HEIGHT
      #define _Z_PROBE_DEPLOY_HEIGHT Z_PROBE_BETWEEN_HEIGHT
    #else
      #define _Z_PROBE_DEPLOY_HEIGHT Z_PROBE_DEPLOY_HEIGHT
    #endif
  #else
    #undef X_PROBE_OFFSET_FROM_NOZZLE
    #undef Y_PROBE_OFFSET_FROM_NOZZLE
    #undef Z_PROBE_OFFSET_FROM_NOZZLE
    #define X_PROBE_OFFSET_FROM_NOZZLE 0
    #define Y_PROBE_OFFSET_FROM_NOZZLE 0
    #define Z_PROBE_OFFSET_FROM_NOZZLE 0
    #define _Z_PROBE_DEPLOY_HEIGHT (Z_MAX_POS / 2)
  #endif

  #if ENABLED(CPU_32_BIT)
    // Add additional delay for between direction signal and pulse signal of stepper
    #ifndef STEPPER_DIRECTION_DELAY
      #define STEPPER_DIRECTION_DELAY 0 // time in microseconds
    #endif
  #endif

  // MUVE 3D
  #if MECH(MUVE3D) && ENABLED(PROJECTOR_PORT) && ENABLED(PROJECTOR_BAUDRATE)
    #if PROJECTOR_PORT == 1
      #define DLPSerial Serial1
    #elif PROJECTOR_PORT == 2
      #define DLPSerial Serial2
    #elif PROJECTOR_PORT == 3
      #define DLPSerial Serial3
    #else
      #define DLPSerial Serial2
    #endif
  #endif

  // PWM SPEED and MASK
  #if DISABLED(HEATER_PWM_SPEED)
    #define HEATER_PWM_SPEED 0
  #endif
  #if HEATER_PWM_SPEED < 0
    #define HEATER_PWM_SPEED 0
  #endif
  #if HEATER_PWM_SPEED > 4
    #define HEATER_PWM_SPEED 4
  #endif

  #if HEATER_PWM_SPEED == 0
    #define HEATER_PWM_STEP 1
    #define HEATER_PWM_MASK 255
  #elif HEATER_PWM_SPEED == 1
    #define HEATER_PWM_STEP 2
    #define HEATER_PWM_MASK 254
  #elif HEATER_PWM_SPEED == 2
    #define HEATER_PWM_STEP 4
    #define HEATER_PWM_MASK 252
  #elif HEATER_PWM_SPEED == 3
    #define HEATER_PWM_STEP 8
    #define HEATER_PWM_MASK 248
  #elif HEATER_PWM_SPEED == 4
    #define HEATER_PWM_STEP 16
    #define HEATER_PWM_MASK 240
  #endif

  #if DISABLED(FAN_PWM_SPEED)
    #define FAN_PWM_SPEED 0
  #endif
  #if FAN_PWM_SPEED < 0
    #define FAN_PWM_SPEED 0
  #endif
  #if FAN_PWM_SPEED > 4
    #define FAN_PWM_SPEED 4
  #endif

  #if FAN_PWM_SPEED == 0
    #define FAN_PWM_STEP 1
    #define FAN_PWM_MASK 255
  #elif FAN_PWM_SPEED == 1
    #define FAN_PWM_STEP 2
    #define FAN_PWM_MASK 254
  #elif FAN_PWM_SPEED == 2
    #define FAN_PWM_STEP 4
    #define FAN_PWM_MASK 252
  #elif FAN_PWM_SPEED == 3
    #define FAN_PWM_STEP 8
    #define FAN_PWM_MASK 248
  #elif FAN_PWM_SPEED == 4
    #define FAN_PWM_STEP 16
    #define FAN_PWM_MASK 240
  #endif

  // TEMPERATURE
  #if HOTENDS > 0 && HAS(TEMP_0)
    #define HOT0_ANALOG_INPUTS 1
    #define HOT0_SENSOR_INDEX 0
    #define HOT0_ANALOG_CHANNEL TEMP_0_PIN
    #define HOT_COMMA0 ,
  #else
    #define HOT0_ANALOG_INPUTS 0
    #define HOT0_SENSOR_INDEX TEMP_0_PIN
    #define HOT0_ANALOG_CHANNEL
    #define HOT_COMMA0
  #endif

  #if HOTENDS > 1 && HAS(TEMP_1)
    #define HOT1_ANALOG_INPUTS 1
    #define HOT1_SENSOR_INDEX HOT0_ANALOG_INPUTS
    #define HOT1_ANALOG_CHANNEL HOT_COMMA0 TEMP_1_PIN
    #define HOT_COMMA1 ,
  #else
    #define HOT1_ANALOG_INPUTS 0
    #define HOT1_SENSOR_INDEX TEMP_1_PIN
    #define HOT1_ANALOG_CHANNEL
    #define HOT_COMMA1 HOT_COMMA0
  #endif

  #if HOTENDS > 2 && HAS(TEMP_2)
    #define HOT2_ANALOG_INPUTS 1
    #define HOT2_SENSOR_INDEX HOT0_ANALOG_INPUTS+HOT1_ANALOG_INPUTS
    #define HOT2_ANALOG_CHANNEL HOT_COMMA1 TEMP_2_PIN
    #define HOT_COMMA2 ,
  #else
    #define HOT_COMMA2 HOT_COMMA1
    #define HOT2_ANALOG_INPUTS 0
    #define HOT2_SENSOR_INDEX TEMP_2_PIN
    #define HOT2_ANALOG_CHANNEL
  #endif

  #if HOTENDS > 3 && HAS(TEMP_3)
    #define HOT3_ANALOG_INPUTS 1
    #define HOT3_SENSOR_INDEX HOT0_ANALOG_INPUTS+HOT1_ANALOG_INPUTS+HOT2_ANALOG_INPUTS
    #define HOT3_ANALOG_CHANNEL HOT_COMMA2 TEMP_3_PIN
    #define HOT_COMMA3 ,
  #else
    #define HOT_COMMA3 HOT_COMMA2
    #define HOT3_ANALOG_INPUTS 0
    #define HOT3_SENSOR_INDEX TEMP_3_PIN
    #define HOT3_ANALOG_CHANNEL
  #endif

  #if HAS(TEMP_BED)
    #define BED_ANALOG_INPUTS 1
    #define BED_SENSOR_INDEX HOT0_ANALOG_INPUTS+HOT1_ANALOG_INPUTS+HOT2_ANALOG_INPUTS+HOT3_ANALOG_INPUTS
    #define BED_ANALOG_CHANNEL HOT_COMMA3 TEMP_BED_PIN
    #define BED_COMMA ,
  #else
    #define BED_ANALOG_INPUTS 0
    #define BED_SENSOR_INDEX TEMP_BED_PIN
    #define BED_ANALOG_CHANNEL
    #define BED_COMMA HOT_COMMA3
  #endif

  #if HAS(TEMP_CHAMBER)
    #define CHAMBER_ANALOG_INPUTS 1
    #define CHAMBER_SENSOR_INDEX HOT0_ANALOG_INPUTS+HOT1_ANALOG_INPUTS+HOT2_ANALOG_INPUTS+HOT3_ANALOG_INPUTS+BED_ANALOG_INPUTS
    #define CHAMBER_ANALOG_CHANNEL BED_COMMA TEMP_CHAMBER_PIN
    #define CHAMBER_COMMA ,
  #else
    #define CHAMBER_ANALOG_INPUTS 0
    #define CHAMBER_SENSOR_INDEX TEMP_CHAMBER_PIN
    #define CHAMBER_ANALOG_CHANNEL
    #define CHAMBER_COMMA BED_COMMA
  #endif

  #if HAS(TEMP_COOLER)
    #define COOLER_ANALOG_INPUTS 1
    #define COOLER_SENSOR_INDEX HOT0_ANALOG_INPUTS+HOT1_ANALOG_INPUTS+HOT2_ANALOG_INPUTS+HOT3_ANALOG_INPUTS+BED_ANALOG_INPUTS+CHAMBER_ANALOG_INPUTS
    #define COOLER_ANALOG_CHANNEL CHAMBER_COMMA TEMP_COOLER_PIN
    #define COOLER_COMMA ,
  #else
    #define COOLER_ANALOG_INPUTS 0
    #define COOLER_SENSOR_INDEX TEMP_COOLER_PIN
    #define COOLER_ANALOG_CHANNEL
    #define COOLER_COMMA CHAMBER_COMMA
  #endif

  #if HAS(FILAMENT_SENSOR)
    #define FILAMENT_ANALOG_INPUTS 1
    #define FILAMENT_SENSOR_INDEX HOT0_ANALOG_INPUTS+HOT1_ANALOG_INPUTS+HOT2_ANALOG_INPUTS+HOT3_ANALOG_INPUTS+BED_ANALOG_INPUTS+CHAMBER_ANALOG_INPUTS+COOLER_ANALOG_INPUTS
    #define FILAMENT_ANALOG_CHANNEL COOLER_COMMA FILWIDTH_PIN
    #define FILAMENT_COMMA ,
  #else
    #define FILAMENT_ANALOG_INPUTS 0
    #define FILAMENT_SENSOR_INDEX FILWIDTH_PIN
    #define FILAMENT_ANALOG_CHANNEL
    #define FILAMENT_COMMA COOLER_COMMA
  #endif

  #if HAS(POWER_CONSUMPTION_SENSOR)
    #define POWER_ANALOG_INPUTS 1
    #define POWER_SENSOR_INDEX HOT0_ANALOG_INPUTS+HOT1_ANALOG_INPUTS+HOT2_ANALOG_INPUTS+HOT3_ANALOG_INPUTS+BED_ANALOG_INPUTS+CHAMBER_ANALOG_INPUTS+COOLER_ANALOG_INPUTS+FILAMENT_ANALOG_INPUTS
    #define POWER_ANALOG_CHANNEL FILAMENT_COMMA POWER_CONSUMPTION_PIN
    #define POWER_COMMA ,
  #else
    #define POWER_ANALOG_INPUTS 0
    #define POWER_ANALOG_CHANNEL
    #define POWER_COMMA FILAMENT_COMMA
  #endif

  #if ENABLED(ARDUINO_ARCH_SAM) && !MB(RADDS) // RADDS not have MCU Temperature
    #define MCU_ANALOG_INPUTS 1
    #define MCU_SENSOR_INDEX HOT0_ANALOG_INPUTS+HOT1_ANALOG_INPUTS+HOT2_ANALOG_INPUTS+HOT3_ANALOG_INPUTS+BED_ANALOG_INPUTS+CHAMBER_ANALOG_INPUTS+COOLER_ANALOG_INPUTS+FILAMENT_ANALOG_INPUTS+POWER_ANALOG_INPUTS
    #define MCU_ANALOG_CHANNEL POWER_COMMA ADC_TEMPERATURE_SENSOR
  #else
    #define MCU_ANALOG_INPUTS 0
    #define MCU_SENSOR_INDEX -1
    #define MCU_ANALOG_CHANNEL
  #endif

  #define ANALOG_INPUTS (HOT0_ANALOG_INPUTS+HOT1_ANALOG_INPUTS+HOT2_ANALOG_INPUTS+HOT3_ANALOG_INPUTS+BED_ANALOG_INPUTS+CHAMBER_ANALOG_INPUTS+COOLER_ANALOG_INPUTS+FILAMENT_ANALOG_INPUTS+POWER_ANALOG_INPUTS+MCU_ANALOG_INPUTS)
  #if ANALOG_INPUTS > 0
    /** Channels are the MUX-part of ADMUX register */
    #define ANALOG_INPUT_CHANNELS {HOT0_ANALOG_CHANNEL HOT1_ANALOG_CHANNEL HOT2_ANALOG_CHANNEL HOT3_ANALOG_CHANNEL BED_ANALOG_CHANNEL CHAMBER_ANALOG_CHANNEL COOLER_ANALOG_CHANNEL FILAMENT_ANALOG_CHANNEL POWER_ANALOG_CHANNEL MCU_ANALOG_CHANNEL}
  #endif

#endif // _CONDITIONALS_POST_H
