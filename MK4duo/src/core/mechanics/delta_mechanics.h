/**
 * MK4duo Firmware for 3D Printer, Laser and CNC
 *
 * Based on Marlin, Sprinter and grbl
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2019 Alberto Cotronei @MagoKimbra
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
 * delta_mechanics.h
 *
 * Copyright (c) 2019 Alberto Cotronei @MagoKimbra
 */

#pragma once

// Struct Delta Settings
typedef struct : public generic_data_t {

  float       diagonal_rod,
              radius,
              print_radius,
              probe_radius,
              height;

  abc_float_t diagonal_rod_adj,
              endstop_adj,
              tower_angle_adj,
              tower_radius_adj;

  uint16_t    segments_per_second;

  uint8_t     segments_per_line;

} mechanics_data_t;

class Delta_Mechanics : public Mechanics {

  public: /** Constructor */

    Delta_Mechanics() {}

  public: /** Public Parameters */

    static mechanics_data_t data;

    static abc_float_t delta;

    static float  delta_clip_start_height;

  private: /** Private Parameters */

    static abc_float_t  D2,
                        towerX,
                        towerY;

    static float        Xbc, Xca, Xab,
                        Ybc, Yca, Yab,
                        coreKa, coreKb, coreKc,
                        Q, Q2;

  public: /** Public Function */

    /**
     * Initialize Factory parameters
     */
    static void factory_parameters();

    /**
     * Get the stepper positions in the cartesian_position[] array.
     * Forward kinematics are applied for DELTA.
     *
     * The result is in the current coordinate space with
     * leveling applied. The coordinates need to be run through
     * unapply_leveling to obtain the "ideal" coordinates
     * suitable for current_position.x, etc.
     */
    static void get_cartesian_from_steppers();

    #if DISABLED(AUTO_BED_LEVELING_UBL)
      /**
       * Prepare a linear move in a DELTA setup.
       *
       * This calls buffer_line several times, adding
       * small incremental moves for DELTA.
       */
      static bool prepare_move_to_destination_mech_specific();
    #endif

    /**
     *  Plan a move to (X, Y, Z) and set the current_position
     *  The final current_position may not be the one that was requested
     */
    static void do_blocking_move_to(const float rx, const float ry, const float rz, const feedrate_t &fr_mm_s=0.0f);

    FORCE_INLINE static void do_blocking_move_to(const xy_pos_t &raw, const feedrate_t &fr_mm_s=0.0f) {
      do_blocking_move_to(raw.x, raw.y, current_position.z, fr_mm_s);
    }
    FORCE_INLINE static void do_blocking_move_to(const xyz_pos_t &raw, const feedrate_t &fr_mm_s=0.0f) {
      do_blocking_move_to(raw.x, raw.y, raw.z, fr_mm_s);
    }
    FORCE_INLINE static void do_blocking_move_to(const xyze_pos_t &raw, const feedrate_t &fr_mm_s=0.0f) {
      do_blocking_move_to(raw.x, raw.y, raw.z, fr_mm_s);
    }

    FORCE_INLINE static void do_blocking_move_to_xy(const float &rx, const float &ry, const feedrate_t &fr_mm_s=0.0f) {
      do_blocking_move_to(rx, ry, current_position.z, fr_mm_s);
    }
    FORCE_INLINE static void do_blocking_move_to_xy(const xy_pos_t &raw, const feedrate_t &fr_mm_s=0.0f) {
      do_blocking_move_to(raw.x, raw.y, current_position.z, fr_mm_s);
    }
    FORCE_INLINE static void do_blocking_move_to_xy(const xyz_pos_t &raw, const feedrate_t &fr_mm_s=0.0f) {
      do_blocking_move_to(raw.x, raw.y, current_position.z, fr_mm_s);
    }
    FORCE_INLINE static void do_blocking_move_to_xy(const xyze_pos_t &raw, const feedrate_t &fr_mm_s=0.0f) {
      do_blocking_move_to(raw.x, raw.y, current_position.z, fr_mm_s);
    }

    FORCE_INLINE static void do_blocking_move_to_x(const float &rx, const feedrate_t &fr_mm_s=0.0f) {
      do_blocking_move_to(rx, current_position.y, current_position.z, fr_mm_s);
    }
    FORCE_INLINE static void do_blocking_move_to_y(const float &ry, const feedrate_t &fr_mm_s=0.0f) {
      do_blocking_move_to(current_position.x, ry, current_position.z, fr_mm_s);
    }
    FORCE_INLINE static void do_blocking_move_to_z(const float &rz, const feedrate_t &fr_mm_s=0.0f) {
      do_blocking_move_to(current_position.x, current_position.y, rz, fr_mm_s);
    }

    /**
     * Delta function
     */
    static void InverseTransform(const float Ha, const float Hb, const float Hc, xyz_pos_t &cartesian);
    FORCE_INLINE static void InverseTransform(const xyz_pos_t &pos, xyz_pos_t &cartesian) { InverseTransform(pos.x, pos.x, pos.z, cartesian); }
    static void Transform(const xyz_pos_t &raw);
    static void recalc_delta_settings();

    /**
     * Home Delta
     */
    static void home(const bool report_position=true);

    /**
     * Home an individual linear axis
     */
    static void do_homing_move(const AxisEnum axis, const float distance, const feedrate_t fr_mm_s=0.0);

    /**
     * Set an axis' current position to its home position (after homing).
     *
     * DELTA should wait until all homing is done before setting the XYZ
     * current_position.x to home, because homing is a single operation.
     * In the case where the axis positions are already known and previously
     * homed, DELTA could home to X or Y individually by moving either one
     * to the center. However, homing Z always homes XY and Z.
     *
     * Callers must sync the planner position after calling this!
     */
    static void set_axis_is_at_home(const AxisEnum axis);

    static bool position_is_reachable(const float &rx, const float &ry);
    inline static bool position_is_reachable(const xy_pos_t &pos) { return position_is_reachable(pos.x, pos.y); }
    static bool position_is_reachable_by_probe(const float &rx, const float &ry);

    /**
     * Report current position to host
     */
    static void report_current_position_detail();

    #if ENABLED(DELTA_AUTO_CALIBRATION_1)
      static float ComputeDerivative(unsigned int deriv, float ha, float hb, float hc);
    #endif

    /**
     * Print mechanics parameters in memory
     */
    #if DISABLED(DISABLE_M503)
      static void print_parameters();
      static void print_M92();
      static void print_M201();
      static void print_M203();
      static void print_M204();
      static void print_M205();
      static void print_M666();
    #endif

    #if HAS_NEXTION_LCD && ENABLED(NEXTION_GFX)
      static void nextion_gfx_clear();
    #endif

  private: /** Private Function */

    /**
     *  Home axis
     */
    static void homeaxis(const AxisEnum axis);

    /**
     * Calculate delta, start a line, and set current_position.x to destination
     */
    static void prepare_uninterpolated_move_to_destination(const feedrate_t &fr_mm_s=0.0);

    /**
     * Calculate the highest Z position where the
     * effector has the full range of XY motion.
     */
    static void Set_clip_start_height();

    #if ENABLED(DELTA_FAST_SQRT) && ENABLED(__AVR__)
      static float Q_rsqrt(float number);
    #endif

};

extern Delta_Mechanics mechanics;
