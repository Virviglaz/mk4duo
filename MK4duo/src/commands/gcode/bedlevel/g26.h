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
 * gcode.h
 *
 * Copyright (c) 2019 Alberto Cotronei @MagoKimbra
 */

#if ENABLED(G26_MESH_VALIDATION)

#define CODE_G26

#define G26_OK false
#define G26_ERR true

#define EXTRUSION_MULTIPLIER 1.0
#define RETRACTION_MULTIPLIER 1.0
#define PRIME_LENGTH 10.0
#define OOZE_AMOUNT 0.3

#define INTERSECTION_CIRCLE_RADIUS 5
#define CROSSHAIRS_SIZE 3

#ifndef G26_XY_FEEDRATE
  #define G26_XY_FEEDRATE (PLANNER_XY_FEEDRATE() / 3.0)
#endif

#if CROSSHAIRS_SIZE >= INTERSECTION_CIRCLE_RADIUS
  #error "CROSSHAIRS_SIZE must be less than INTERSECTION_CIRCLE_RADIUS."
#endif

#if ENABLED(ARC_SUPPORT)
  void plan_arc(const xyze_pos_t &cart, const ab_float_t &offset, const uint8_t clockwise);
#endif

/**
 *   G26 Mesh Validation Tool
 *
 *   G26 is a Mesh Validation Tool intended to provide support for the MK4duo Unified Bed Leveling System.
 *   In order to fully utilize and benefit from the MK4duo Unified Bed Leveling System an accurate Mesh must
 *   be defined. G29 is designed to allow the user to quickly validate the correctness of her Mesh. It will
 *   first heat the bed and nozzle. It will then print lines and circles along the Mesh Cell boundaries and
 *   the intersections of those lines (respectively).
 *
 *   This action allows the user to immediately see where the Mesh is properly defined and where it needs to
 *   be edited. The command will generate the Mesh lines closest to the nozzle's starting position. Alternatively
 *   the user can specify the X and Y position of interest with command parameters. This allows the user to
 *   focus on a particular area of the Mesh where attention is needed.
 *
 *   B #  Bed         Set the Bed Temperature. If not specified, a default of 60 C. will be assumed.
 *
 *   C    Current     When searching for Mesh Intersection points to draw, use the current nozzle location
 *                    as the base for any distance comparison.
 *
 *   D    Disable     Disable the Unified Bed Leveling System. In the normal case the user is invoking this
 *                    command to see how well a Mesh as been adjusted to match a print surface. In order to do
 *                    this the Unified Bed Leveling System is turned on by the G26 command. The D parameter
 *                    alters the command's normal behaviour and disables the Unified Bed Leveling System even if
 *                    it is on.
 *
 *   H #  Hotend      Set the Nozzle Temperature. If not specified, a default of 205 C. will be assumed.
 *
 *   F #  Filament    Used to specify the diameter of the filament being used. If not specified
 *                    1.75mm filament is assumed. If you are not getting acceptable results by using the
 *                    'correct' numbers, you can scale this number up or down a little bit to change the amount
 *                    of filament that is being extruded during the printing of the various lines on the bed.
 *
 *   K    Keep-On     Keep the heaters turned on at the end of the command.
 *
 *   L #  Layer       Layer height. (Height of nozzle above bed)  If not specified .20mm will be used.
 *
 *   O #  Ooooze      How much your nozzle will Ooooze filament while getting in position to print. This
 *                    is over kill, but using this parameter will let you get the very first 'circle' perfect
 *                    so you have a trophy to peel off of the bed and hang up to show how perfectly you have your
 *                    Mesh calibrated. If not specified, a filament length of .3mm is assumed.
 *
 *   P #  Prime       Prime the nozzle with specified length of filament. If this parameter is not
 *                    given, no prime action will take place. If the parameter specifies an amount, that much
 *                    will be purged before continuing. If no amount is specified the command will start
 *                    purging filament until the user provides an LCD Click and then it will continue with
 *                    printing the Mesh. You can carefully remove the spent filament with a needle nose
 *                    pliers while holding the LCD Click wheel in a depressed state. If you do not have
 *                    an LCD, you must specify a value if you use P.
 *
 *   Q #  Multiplier  Retraction Multiplier. Normally not needed. Retraction defaults to 1.0mm and
 *                    un-retraction is at 1.2mm   These numbers will be scaled by the specified amount
 *
 *   R #  Repeat      Prints the number of patterns given as a parameter, starting at the current location.
 *                    If a parameter isn't given, every point will be printed unless G26 is interrupted.
 *                    This works the same way that the UBL G29 P4 R parameter works.
 *
 *                    NOTE:  If you do not have an LCD, you -must- specify R. This is to ensure that you are
 *                    aware that there's some risk associated with printing without the ability to abort in
 *                    cases where mesh point Z value may be inaccurate. As above, if you do not include a
 *                    parameter, every point will be printed.
 *
 *   S #  Nozzle      Used to control the size of nozzle diameter. If not specified, a .4mm nozzle is assumed.
 *
 *   U #  Random      Randomize the order that the circles are drawn on the bed. The search for the closest
 *                    un-drawn circle is still done. But the distance to the location for each circle has a
 *                    random number of the specified size added to it. Specifying S50 will give an interesting
 *                    deviation from the normal behaviour on a 10 x 10 Mesh.
 *
 *   X #  X Coord.    Specify the starting location of the drawing activity.
 *
 *   Y #  Y Coord.    Specify the starting location of the drawing activity.
 */

// Private functions

static MeshFlags circle_flags, horizontal_mesh_line_flags, vertical_mesh_line_flags;
float g26_e_axis_feedrate = 0.025,
      random_deviation = 0.0;

static bool g26_retracted = false; // Track the retracted state of the nozzle so mismatched
                                   // retracts/recovers won't result in a bad state.

float g26_extrusion_multiplier,
      g26_retraction_multiplier,
      g26_layer_height,
      g26_prime_length;

xy_pos_t g26_pos; // = { 0, 0 }

int16_t g26_bed_temp,
        g26_hotend_temp;

int8_t g26_prime_flag;

#if HAS_LCD_MENU

  /**
   * If the LCD is clicked, cancel, wait for release, return true
   */
  bool user_canceled() {
    if (!lcdui.button_pressed()) return false; // Return if the button isn't pressed
    lcdui.set_status_P(PSTR("Mesh Validation Stopped."), 99);
    #if HAS_LCD_MENU
      lcdui.quick_feedback(true);
    #endif
    lcdui.wait_for_release();
    return true;
  }

#endif

mesh_index_pair find_closest_circle_to_print(const xy_pos_t &pos) {
  float closest = 99999.99;
  mesh_index_pair return_val;

  return_val.x = return_val.y = -1;

  for (uint8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
    for (uint8_t j = 0; j < GRID_MAX_POINTS_Y; j++) {
      if (!circle_flags.marked(i, j)) {
        // We found a circle that needs to be printed
        const xyze_pos_t m = { _GET_MESH_X(i), _GET_MESH_Y(j) };

        // Get the distance to this intersection
        float f = HYPOT(pos.x - m.x, pos.y - m.y);

        // It is possible that we are being called with the values
        // to let us find the closest circle to the start position.
        // But if this is not the case, add a small weighting to the
        // distance calculation to help it choose a better place to continue.
        f += HYPOT(g26_pos.x - m.x, g26_pos.y - m.y) / 15.0;

        // Add in the specified amount of Random Noise to our search
        if (random_deviation > 1.0) f += random(0.0, random_deviation);

        if (f < closest) {
          closest = f;        // Found a closer un-printed location
          return_val.x = i;   // Save its data
          return_val.y = j;
          return_val.distance = closest;
        }
      }
    }
  }
  circle_flags.mark(return_val); // Mark this location as done.
  return return_val;
}

void G26_line_to_destination(const float &feed_rate) {
  REMEMBER(fr, mechanics.feedrate_mm_s, feed_rate);
  mechanics.prepare_move_to_destination();  // will ultimately call ubl.line_to_destination_cartesian or ubl.prepare_linear_move_to for UBL_DELTA
}

void move_to(const float &rx, const float &ry, const float &z, const float &e_delta) {
  float feed_value;
  static float last_z = -999.99;

  const xy_pos_t dest = { rx, ry };

  const bool has_xy_component = dest != mechanics.current_position; // Check if X or Y is involved in the movement.

  mechanics.destination = mechanics.current_position;

  if (z != last_z) {
    last_z = mechanics.destination.z = z;
    feed_value = mechanics.data.max_feedrate_mm_s.z * 0.5f; // Use half of the Z_AXIS max feed rate
    G26_line_to_destination(feed_value);
  }

  // If X or Y is involved do a 'normal' move. Otherwise retract/recover/hop.
  feed_value = has_xy_component ? G26_XY_FEEDRATE : mechanics.data.max_feedrate_mm_s.e[tools.data.extruder.active] * 0.666f;
  mechanics.destination = dest;
  mechanics.destination.e += e_delta;
  G26_line_to_destination(feed_value);
}

FORCE_INLINE void move_to(const xyze_pos_t &where, const float &de) {
  move_to(where.x, where.y, where.z, de);
}

void retract_filament(const xyze_pos_t &where) {
  if (!g26_retracted) { // Only retract if we are not already retracted!
    g26_retracted = true;
    move_to(where, -1.0 * g26_retraction_multiplier);
  }
}
void retract_lift_move(const xyz_pos_t &s) {
  retract_filament(mechanics.destination);
  move_to(mechanics.current_position.x, mechanics.current_position.y, mechanics.current_position.z + 0.500, 0.0);  // Z lift to minimize scraping
  move_to(s.x, s.y, s.z + 0.500, 0.0);  // Get to the starting point with no extrusion while lifted
}

void recover_filament(const xyze_pos_t &where) {
  if (g26_retracted) { // Only un-retract if we are retracted.
    move_to(where, 1.2 * g26_retraction_multiplier);
    g26_retracted = false;
  }
}

/**
 * print_line_from_here_to_there() takes two cartesian coordinates and draws a line from one
 * to the other. But there are really three sets of coordinates involved. The first coordinate
 * is the present location of the nozzle. We don't necessarily want to print from this location.
 * We first need to move the nozzle to the start of line segment where we want to print. Once
 * there, we can use the two coordinates supplied to draw the line.
 *
 * Note:  Although we assume the first set of coordinates is the start of the line and the second
 * set of coordinates is the end of the line, it does not always work out that way. This function
 * optimizes the movement to minimize the travel distance before it can start printing. This saves
 * a lot of time and eliminates a lot of nonsensical movement of the nozzle. However, it does
 * cause a lot of very little short retracement of th nozzle when it draws the very first line
 * segment of a 'circle'. The time this requires is very short and is easily saved by the other
 * cases where the optimization comes into play.
 */
void print_line_from_here_to_there(const xyz_pos_t &s, const xyz_pos_t &e) {

  // Distances to the start / end of the line
  xy_float_t svec = mechanics.current_position - s, evec = mechanics.current_position - e;

  const float dist_start = HYPOT2(svec.x, svec.y),
              dist_end = HYPOT2(evec.x, evec.y),
              line_length = HYPOT(e.x - s.x, e.y - s.y);

  // If the end point of the line is closer to the nozzle, flip the direction,
  // moving from the end to the start. On very small lines the optimization isn't worth it.
  if (dist_end < dist_start && (INTERSECTION_CIRCLE_RADIUS) < ABS(line_length))
    return print_line_from_here_to_there(e, s);

  // Decide whether to retract & lift
  if (dist_start > 2.0) retract_lift_move(s);

  move_to(s, 0.0); // Get to the starting point with no extrusion / un-Z lift

  const float e_pos_delta = line_length * g26_e_axis_feedrate * g26_extrusion_multiplier;

  recover_filament(mechanics.destination);
  move_to(e, e_pos_delta);  // Get to the ending point with an appropriate amount of extrusion
}

inline bool look_for_lines_to_connect() {
  xyz_pos_t s, e;
  s.z = e.z = g26_layer_height;

  for (uint8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
    for (uint8_t j = 0; j < GRID_MAX_POINTS_Y; j++) {

      #if HAS_LCD_MENU
        if (user_canceled()) return true;     // Check if the user wants to stop the Mesh Validation
      #endif

      if (i < GRID_MAX_POINTS_X) {  // Can't connect to anything farther to the right than GRID_MAX_POINTS_X.
                                    // Already a half circle at the edge of the bed.

        if (circle_flags.marked(i, j) && circle_flags.marked(i + 1, j)) {   // Test whether a leftward line can be done
          if (!horizontal_mesh_line_flags.marked(i, j)) {
            // Two circles need a horizontal line to connect them
            s.x = _GET_MESH_X(  i  ) + (INTERSECTION_CIRCLE_RADIUS - (CROSSHAIRS_SIZE)); // right edge
            e.x = _GET_MESH_X(i + 1) - (INTERSECTION_CIRCLE_RADIUS - (CROSSHAIRS_SIZE)); // left edge

            LIMIT(s.x, X_MIN_BED + 1, X_MAX_BED - 1);
            s.y = e.y = constrain(_GET_MESH_Y(j), Y_MIN_BED + 1, Y_MAX_BED - 1);
            LIMIT(e.x, X_MIN_BED + 1, X_MAX_BED - 1);

            if (mechanics.position_is_reachable(s.x, s.y) && mechanics.position_is_reachable(e.x, e.y))
              print_line_from_here_to_there(s, e);

            horizontal_mesh_line_flags.mark(i, j); // Mark done, even if skipped
          }
        }

        if (j < GRID_MAX_POINTS_Y) {  // Can't connect to anything further back than GRID_MAX_POINTS_Y.
                                      // Already a half circle at the edge of the bed.

          if (circle_flags.marked(i, j) && circle_flags.marked(i, j + 1)) {   // Test whether a downward line can be done
            if (!vertical_mesh_line_flags.marked(i, j)) {
              // Two circles that need a vertical line to connect them
              s.y = _GET_MESH_Y(  j  ) + (INTERSECTION_CIRCLE_RADIUS - (CROSSHAIRS_SIZE)); // top edge
              e.y = _GET_MESH_Y(j + 1) - (INTERSECTION_CIRCLE_RADIUS - (CROSSHAIRS_SIZE)); // bottom edge

              s.x = e.x = constrain(_GET_MESH_X(i), X_MIN_BED + 1, X_MAX_BED - 1);
              LIMIT(s.y, Y_MIN_BED + 1, Y_MAX_BED - 1);
              LIMIT(e.y, Y_MIN_BED + 1, Y_MAX_BED - 1);

              if (mechanics.position_is_reachable(s.x, s.y) && mechanics.position_is_reachable(e.x, e.y))
                print_line_from_here_to_there(s, e);

              vertical_mesh_line_flags.mark(i, j); // Mark done, even if skipped
            }
          }
        }
      }
    }
  }
  return false;
}

/**
 * Turn on the bed and nozzle heat and
 * wait for them to get up to temperature.
 */
inline bool turn_on_heaters() {

  if (printer.debugDryrun() || printer.debugSimulation()) return false;

  const bool oldReport = printer.isAutoreportTemp();
  printer.setAutoreportTemp(true);

  SERIAL_EM("Waiting for heatup.");

  #if MAX_BED > 0
    if (g26_bed_temp > 25) {
      #if HAS_LCD
        lcdui.set_status_P(PSTR("G26 Heating Bed."), 99);
        lcdui.quick_feedback(true);
        #if HAS_LCD_MENU
          lcdui.capture();
        #endif
      #endif
      beds[0]->set_target_temp(g26_bed_temp);

      // Wait for the temperature to stabilize
      beds[0]->wait_for_target(true);
      if (!printer.isWaitForHeatUp()) return G26_ERR;
    }
  #endif // HAS_TEMP_BED0

  // Start heating the active nozzle
  #if HAS_LCD
    lcdui.set_status_P(PSTR("G26 Heating Nozzle."), 99);
    lcdui.quick_feedback(true);
  #endif
  hotends[0]->set_target_temp(g26_hotend_temp);

  // Wait for the temperature to stabilize
  hotends[0]->wait_for_target(true);
  if (!printer.isWaitForHeatUp()) return G26_ERR;

  #if HAS_LCD
    lcdui.reset_status();
    lcdui.quick_feedback(true);
  #endif

  printer.setAutoreportTemp(oldReport);

  return G26_OK;
}

/**
 * Prime the nozzle if needed. Return true on error.
 */
inline bool prime_nozzle() {

  #if HAS_LCD_MENU
    #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
      float Total_Prime = 0.0;
    #endif

    if (g26_prime_flag == -1) {  // The user wants to control how much filament gets purged

      lcdui.capture();
      lcdui.set_status_P(PSTR("User-Controlled Prime"), 99);
      lcdui.chirp();

      mechanics.destination = mechanics.current_position;

      recover_filament(mechanics.destination); // Make sure G26 doesn't think the filament is retracted().

      while (!lcdui.button_pressed()) {
        lcdui.chirp();
        mechanics.destination.e += 0.25;
        #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
          Total_Prime += 0.25;
          if (Total_Prime >= EXTRUDE_MAXLENGTH) return G26_ERR;
        #endif
        G26_line_to_destination(mechanics.data.max_feedrate_mm_s.e[tools.data.extruder.active] / 15.0);
        mechanics.destination = mechanics.current_position;
        planner.synchronize();    // Without this synchronize, the purge is more consistent,
                                  // but because the planner has a buffer, we won't be able
                                  // to stop as quickly. So we put up with the less smooth
                                  // action to give the user a more responsive 'Stop'.
      }

      lcdui.wait_for_release();

      lcdui.set_status_P(PSTR("Done Priming"), 99);
      lcdui.quick_feedback(true);
      lcdui.release();
    }
    else
  #endif
  {
    #if HAS_LCD
      lcdui.set_status_P(PSTR("Fixed Length Prime."), 99);
      lcdui.quick_feedback(true);
    #endif
    mechanics.destination = mechanics.current_position;
    mechanics.destination.e += g26_prime_length;
    G26_line_to_destination(mechanics.data.max_feedrate_mm_s.e[tools.data.extruder.active] / 15.0);
    mechanics.destination = mechanics.current_position;
    retract_filament(mechanics.destination);
  }

  return G26_OK;
}

/**
 * G26: Mesh Validation Pattern generation.
 *
 * Used to interactively edit the mesh by placing the
 * nozzle in a problem area and doing a G29 P4 R command.
 *
 * Parameters:
 *
 *  B  Bed Temperature
 *  C  Continue from the Closest mesh point
 *  D  Disable leveling before starting
 *  F  Filament diameter
 *  H  Hotend Temperature
 *  K  Keep heaters on when completed
 *  L  Layer Height
 *  O  Ooze extrusion length
 *  P  Prime length
 *  Q  Retraction multiplier
 *  R  Repetitions (number of grid points)
 *  S  Nozzle Size (diameter) in mm
 *  T  Tool index to change to, if included
 *  U  Random deviation (50 if no value given)
 *  X  X position
 *  Y  Y position
 */
inline void gcode_G26() {
  SERIAL_EM("G26 command started. Waiting for heater(s).");

  // Don't allow Mesh Validation without homing first,
  // or if the parameter parsing did not go OK, abort
  if (mechanics.axis_unhomed_error()) return;

  // Change the tool first, if specified
  if (parser.seenval('T')) tools.change(parser.value_int());

  g26_extrusion_multiplier    = EXTRUSION_MULTIPLIER;
  g26_retraction_multiplier   = RETRACTION_MULTIPLIER;
  g26_layer_height            = MESH_TEST_LAYER_HEIGHT;
  g26_prime_length            = PRIME_LENGTH;
  g26_bed_temp                = MESH_TEST_BED_TEMP;
  g26_hotend_temp             = MESH_TEST_HOTEND_TEMP;
  g26_prime_flag              = 0;

  float g26_nozzle            = MESH_TEST_NOZZLE_SIZE,
        g26_filament_diameter = DEFAULT_NOMINAL_FILAMENT_DIA,
        g26_ooze_amount       = parser.linearval('O', OOZE_AMOUNT);

  bool  g26_continue_with_closest = parser.boolval('C'),
        g26_keep_heaters_on       = parser.boolval('K');

  #if MAX_BED > 0
    if (parser.seenval('B')) {
      g26_bed_temp = parser.value_celsius();
      if (g26_bed_temp && !WITHIN(g26_bed_temp, 40, beds[0]->data.temp.max - 10)) {
        SERIAL_EMV("?Specified bed temperature not plausible (40-", int(beds[0]->data.temp.max - 15));
        return;
      }
    }
  #endif

  if (parser.seenval('L')) {
    g26_layer_height = parser.value_linear_units();
    if (!WITHIN(g26_layer_height, 0.0, 2.0)) {
      SERIAL_EM("?Specified layer height not plausible.");
      return;
    }
  }

  if (parser.seen('Q')) {
    if (parser.has_value()) {
      g26_retraction_multiplier = parser.value_float();
      if (!WITHIN(g26_retraction_multiplier, 0.05, 15.0)) {
        SERIAL_EM("?Specified Retraction Multiplier not plausible.");
        return;
      }
    }
    else {
      SERIAL_EM("?Retraction Multiplier must be specified.");
      return;
    }
  }

  if (parser.seenval('S')) {
    g26_nozzle = parser.value_float();
    if (!WITHIN(g26_nozzle, 0.1, 2.0)) {
      SERIAL_EM("?Specified nozzle size not plausible.");
      return;
    }
  }

  if (parser.seen('P')) {
    if (!parser.has_value()) {
      #if HAS_LCD_MENU
        g26_prime_flag = -1;
      #else
        SERIAL_EM("?Prime length must be specified when not using an LCD.");
        return;
      #endif
    }
    else {
      g26_prime_flag++;
      g26_prime_length = parser.value_linear_units();
      if (!WITHIN(g26_prime_length, 0.0, 25.0)) {
        SERIAL_EM("?Specified prime length not plausible.");
        return;
      }
    }
  }

  if (parser.seenval('F')) {
    g26_filament_diameter = parser.value_linear_units();
    if (!WITHIN(g26_filament_diameter, 1.0, 4.0)) {
      SERIAL_EM("?Specified filament size not plausible.");
      return;
    }
  }
  g26_extrusion_multiplier *= sq(1.75) / sq(g26_filament_diameter); // If we aren't using 1.75mm filament, we need to
                                                                    // scale up or down the length needed to get the
                                                                    // same volume of filament

  g26_extrusion_multiplier *= g26_filament_diameter * sq(g26_nozzle) / sq(0.3); // Scale up by nozzle size

  if (parser.seenval('H')) {
    g26_hotend_temp = parser.value_celsius();
    if (!WITHIN(g26_hotend_temp, 165, hotends[0]->data.temp.max - 10)) {
      SERIAL_EM("?Specified nozzle temperature not plausible.");
      return;
    }
  }

  if (parser.seen('U')) {
    randomSeed(millis());
    // This setting will persist for the next G26
    random_deviation = parser.has_value() ? parser.value_float() : 50.0;
  }

  int16_t g26_repeats;
  #if HAS_LCD_MENU
    g26_repeats = parser.intval('R', GRID_MAX_POINTS + 1);
  #else
    if (!parser.seen('R')) {
      SERIAL_EM("?(R)epeat must be specified when not using an LCD.");
      return;
    }
    else
      g26_repeats = parser.has_value() ? parser.value_int() : GRID_MAX_POINTS + 1;
  #endif
  if (g26_repeats < 1) {
    SERIAL_EM("?(R)epeat value not plausible; must be at least 1.");
    return;
  }

  g26_pos.set(parser.seenval('X') ? NATIVE_X_POSITION(parser.value_linear_units()) : mechanics.current_position.x,
              parser.seenval('Y') ? NATIVE_Y_POSITION(parser.value_linear_units()) : mechanics.current_position.y);
  if (!mechanics.position_is_reachable(g26_pos.x, g26_pos.y)) {
    SERIAL_EM("?Specified X,Y coordinate out of bounds.");
    return;
  }

  /**
   * Wait until all parameters are verified before altering the state!
   */
  bedlevel.set_bed_leveling_enabled(!parser.seen('D'));

  if (mechanics.current_position.z < Z_PROBE_BETWEEN_HEIGHT) {
    mechanics.do_blocking_move_to_z(Z_PROBE_BETWEEN_HEIGHT);
    mechanics.current_position = mechanics.destination;
  }

  if (turn_on_heaters() != G26_OK) goto LEAVE;

  mechanics.current_position.e = 0.0;
  mechanics.sync_plan_position_e();

  if (g26_prime_flag && prime_nozzle() != G26_OK) goto LEAVE;

  /**
   *  Bed is preheated
   *
   *  Nozzle is at temperature
   *
   *  Filament is primed!
   *
   *  It's  "Show Time" !!!
   */

  circle_flags.reset();
  horizontal_mesh_line_flags.reset();
  vertical_mesh_line_flags.reset();

  // Move nozzle to the specified height for the first layer
  mechanics.destination = mechanics.current_position;
  mechanics.destination.z = g26_layer_height;
  move_to(mechanics.destination, 0.0);
  move_to(mechanics.destination, g26_ooze_amount);

  #if HAS_LCD_MENU
    lcdui.capture();
  #endif

  #if DISABLED(ARC_SUPPORT)

    /**
     * Pre-generate radius offset values at 30 degree intervals to reduce CPU load.
     */
    #define A_INT 30
    #define _ANGS (360 / A_INT)
    #define A_CNT (_ANGS / 2)
    #define _IND(A) ((A + _ANGS * 8) % _ANGS)
    #define _COS(A) (trig_table[_IND(A) % A_CNT] * (_IND(A) >= A_CNT ? -1 : 1))
    #define _SIN(A) (-_COS((A + A_CNT / 2) % _ANGS))
    #if A_CNT & 1
      #error "A_CNT must be a positive value. Please change A_INT."
    #endif
    float trig_table[A_CNT];
    for (uint8_t i = 0; i < A_CNT; i++)
      trig_table[i] = INTERSECTION_CIRCLE_RADIUS * COS(RADIANS(i * A_INT));

  #endif // !ARC_SUPPORT

  mesh_index_pair location;
  do {
    // Find the nearest confluence
    location = find_closest_circle_to_print(g26_continue_with_closest ? xy_pos_t(mechanics.current_position) : g26_pos);

    if (location.valid()) {
      xyze_pos_t circle = { _GET_MESH_X(location.x), _GET_MESH_Y(location.y) };

      // If this mesh location is outside the printable_radius, skip it.
      if (!mechanics.position_is_reachable(circle.x, circle.y)) continue;

      // Determine where to start and end the circle,
      // which is always drawn counter-clockwise.
      const xy_uchar_t st = location;
      const bool f = st.y == 0,
                 r = st.x >= GRID_MAX_POINTS_X - 1,
                 b = st.y >= GRID_MAX_POINTS_Y - 1;

      #if ENABLED(ARC_SUPPORT)

        #define ARC_LENGTH(quarters)      (INTERSECTION_CIRCLE_RADIUS * M_PI * (quarters) / 2)
        #define INTERSECTION_CIRCLE_DIAM  ((INTERSECTION_CIRCLE_RADIUS) * 2)

        xy_float_t e = { circle.x + INTERSECTION_CIRCLE_RADIUS, circle.y };
        xyz_float_t s = e;

        // Figure out where to start and end the arc - we always print counterclockwise
        float arc_length = ARC_LENGTH(4);
        if (st.x == 0) {                             // left edge
          if (!f) { s.x = circle.x; s.y -= INTERSECTION_CIRCLE_RADIUS; }
          if (!b) { e.x = circle.x; e.y += INTERSECTION_CIRCLE_RADIUS; }
          arc_length = (f || b) ? ARC_LENGTH(1) : ARC_LENGTH(2);
        }
        else if (r) {                             // right edge
          if (b) s.set(circle.x - (INTERSECTION_CIRCLE_RADIUS), circle.y);
          else   s.set(circle.x, circle.y + INTERSECTION_CIRCLE_RADIUS);
          if (f) e.set(circle.x - (INTERSECTION_CIRCLE_RADIUS), circle.y);
          else   e.set(circle.x, circle.y - (INTERSECTION_CIRCLE_RADIUS));
          arc_length = (f || b) ? ARC_LENGTH(1) : ARC_LENGTH(2);
        }
        else if (f) {
          e.x -= INTERSECTION_CIRCLE_DIAM;
          arc_length = ARC_LENGTH(2);
        }
        else if (b) {
          s.x -= INTERSECTION_CIRCLE_DIAM;
          arc_length = ARC_LENGTH(2);
        }

        const ab_float_t arc_offset = circle - s;
        const xy_float_t dist = mechanics.current_position - s;   // Distance from the start of the actual circle
        const float dist_start = HYPOT2(dist.x, dist.y);
        const xyze_pos_t endpoint = {
          e.x, e.y, g26_layer_height,
          mechanics.current_position.e + (arc_length * g26_e_axis_feedrate * g26_extrusion_multiplier)
        };

        if (dist_start > 2.0) {
          s.z = g26_layer_height + 0.5f;
          retract_lift_move(s);
        }

        s.z = g26_layer_height;
        move_to(s, 0.0);  // Get to the starting point with no extrusion / un-Z lift

        recover_filament(mechanics.destination);
        const float save_feedrate = mechanics.feedrate_mm_s;
        mechanics.feedrate_mm_s = PLANNER_XY_FEEDRATE() / 10.0;

        plan_arc(endpoint, arc_offset, false);  // Draw a counter-clockwise arc
        mechanics.feedrate_mm_s = save_feedrate;
        mechanics.destination = mechanics.current_position;
        #if HAS_LCD_MENU
          if (user_canceled()) goto LEAVE; // Check if the user wants to stop the Mesh Validation
        #endif

      #else // !ARC_SUPPORT

        int8_t start_ind = -2, end_ind = 9; // Assume a full circle (from 5:00 to 5:00)
        if (st.x == 0) {                   // Left edge? Just right half.
          start_ind = f ? 0 : -3;           //  03:00 to 12:00 for front-left
          end_ind = b ? 0 : 2;              //  06:00 to 03:00 for back-left
        }
        else if (r) {                       // Right edge? Just left half.
          start_ind = b ? 6 : 3;            //  12:00 to 09:00 for front-right
          end_ind = f ? 5 : 8;              //  09:00 to 06:00 for back-right
        }
        else if (f) {                       // Front edge? Just back half.
          start_ind = 0;                    //  03:00
          end_ind = 5;                      //  09:00
        }
        else if (b) {                       // Back edge? Just front half.
          start_ind = 6;                    //  09:00
          end_ind = 11;                     //  03:00
        }

        for (int8_t ind = start_ind; ind <= end_ind; ind++) {

          #if HAS_LCD_MENU
            if (user_canceled()) goto LEAVE;          // Check if the user wants to stop the Mesh Validation
          #endif

          xyz_float_t p = { circle.x + _COS(ind    ), circle.y + _SIN(ind    ), g26_layer_height },
                      q = { circle.x + _COS(ind + 1), circle.y + _SIN(ind + 1), g26_layer_height };

          #if IS_KINEMATIC
            // Check to make sure this segment is entirely on the bed, skip if not.
            if (!mechanics.position_is_reachable(p.x, p.y) || !mechanics.position_is_reachable(q.x, q.y)) continue;
          #else
            LIMIT(p.x, X_MIN_BED + 1, X_MAX_BED - 1); // Prevent hitting the endstops
            LIMIT(p.y, Y_MIN_BED + 1, Y_MAX_BED - 1);
            LIMIT(q.x, X_MIN_BED + 1, X_MAX_BED - 1);
            LIMIT(q.y, Y_MIN_BED + 1, Y_MAX_BED - 1);
          #endif

          print_line_from_here_to_there(p, q);
          SERIAL_FLUSH();   // Prevent host M105 buffer overrun.
        }

      #endif // !ARC_SUPPORT

      if (look_for_lines_to_connect()) goto LEAVE;
    }

    SERIAL_FLUSH(); // Prevent host M105 buffer overrun.

  } while (--g26_repeats && location.valid());

LEAVE:
  SERIAL_EM("Leaving G26");
  lcdui.set_status_P(PSTR("Leaving G26"), -1);

  retract_filament(mechanics.destination);
  mechanics.destination.z = Z_PROBE_BETWEEN_HEIGHT;

  move_to(mechanics.destination, 0); // Raise the nozzle

  mechanics.destination.set(g26_pos.x, g26_pos.y);  // Move back to the starting position

  move_to(mechanics.destination, 0); // Move back to the starting position

  #if HAS_LCD_MENU
    lcdui.release();   // Give back control of the LCD Panel!
  #endif

  if (!g26_keep_heaters_on) {
    #if MAX_BED > 0
      beds[0]->set_target_temp(0);
    #endif
    hotends[ACTIVE_HOTEND]->set_target_temp(0);
  }
}

#endif // G26_MESH_VALIDATION
