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
#pragma once

#define MESH_X_DIST     ((MESH_MAX_X - (MESH_MIN_X)) / (GRID_MAX_POINTS_X - 1))
#define MESH_Y_DIST     ((MESH_MAX_Y - (MESH_MIN_Y)) / (GRID_MAX_POINTS_Y - 1))
#define _GET_MESH_X(I)  mbl.data.index_to_xpos[I]
#define _GET_MESH_Y(J)  mbl.data.index_to_ypos[J]
#define Z_VALUES_ARR    mbl.data.z_values

// Struct Mbl data
typedef struct {
  float z_offset,
        index_to_xpos[GRID_MAX_POINTS_X],
        index_to_ypos[GRID_MAX_POINTS_Y],
        z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];
} mbl_data_t;

class mesh_bed_leveling {

  public: /** Constructor */

    mesh_bed_leveling() {}

  public: /** Public Parameters */

    static mbl_data_t data;

  public: /** Public Function */

    static void factory_parameters();

    static void set_z(const int8_t px, const int8_t py, const float &z) { data.z_values[px][py] = z; }

    static void line_to_destination(const feedrate_t fr_mm_s, uint16_t x_splits=0xFFFF, uint16_t y_splits=0xFFFF);

    static void report_mesh();

    FORCE_INLINE static bool has_mesh() {
      for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
        for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
          if (data.z_values[x][y]) return true;
      return false;
    }

    static inline void zigzag(const int8_t index, int8_t &px, int8_t &py) {
      px = index % (GRID_MAX_POINTS_X);
      py = index / (GRID_MAX_POINTS_X);
      if (py & 1) px = (GRID_MAX_POINTS_X - 1) - px; // Zig zag
    }

    static void set_zigzag_z(const int8_t index, const float &z) {
      int8_t px, py;
      zigzag(index, px, py);
      set_z(px, py, z);
    }

    static int8_t cell_index_x(const float &x) {
      int8_t cx = (x - (MESH_MIN_X)) * (1.0f / (MESH_X_DIST));
      return constrain(cx, 0, (GRID_MAX_POINTS_X) - 2);
    }

    static int8_t cell_index_y(const float &y) {
      int8_t cy = (y - (MESH_MIN_Y)) * (1.0f / (MESH_Y_DIST));
      return constrain(cy, 0, (GRID_MAX_POINTS_Y) - 2);
    }

    static int8_t probe_index_x(const float &x) {
      int8_t px = (x - (MESH_MIN_X) + (MESH_X_DIST) * 0.5f) * (1.0f / (MESH_X_DIST));
      return WITHIN(px, 0, GRID_MAX_POINTS_X - 1) ? px : -1;
    }

    static int8_t probe_index_y(const float &y) {
      int8_t py = (y - (MESH_MIN_Y) + (MESH_Y_DIST) * 0.5) * (1.0 / (MESH_Y_DIST));
      return WITHIN(py, 0, GRID_MAX_POINTS_Y - 1) ? py : -1;
    }

    static float calc_z0(const float &a0, const float &a1, const float &z1, const float &a2, const float &z2) {
      const float delta_z = (z2 - z1) / (a2 - a1),
                  delta_a = a0 - a1;
      return z1 + delta_a * delta_z;
    }

    static float get_z(const float &x0, const float &y0
      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
        , const float &factor
      #endif
    ) {
      const int8_t cx = cell_index_x(x0), cy = cell_index_y(y0);
      const float z1 = calc_z0(x0, data.index_to_xpos[cx], data.z_values[cx][cy],     data.index_to_xpos[cx + 1], data.z_values[cx + 1][cy]),
                  z2 = calc_z0(x0, data.index_to_xpos[cx], data.z_values[cx][cy + 1], data.index_to_xpos[cx + 1], data.z_values[cx + 1][cy + 1]),
                  z0 = calc_z0(y0, data.index_to_ypos[cy], z1,                        data.index_to_ypos[cy + 1], z2);

      return data.z_offset + z0
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          * factor
        #endif
      ;
    }

};

extern mesh_bed_leveling mbl;
