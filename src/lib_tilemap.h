// lib_tilemap.h

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "image_info.h"
#include "lib_gbm.h"

#ifndef LIB_TILEMAP_HEADER
#define LIB_TILEMAP_HEADER

    #define TILES_MAX_DEFAULT 255 // if TILES_MAX_DEFAULT > 255, tile_id_list must be larger than uint8_t -> uint32_t

    #define TILE_WIDTH_DEFAULT  8
    #define TILE_HEIGHT_DEFAULT 8

    #define TILE_ID_NOT_FOUND     -1
    #define TILE_ID_OUT_OF_SPACE  -2
    #define TILE_ID_FAILED_ENCODE -3
    #define TILE_ID_PALETTE_ERROR -4


    #define TILE_FLIP_BITS_NONE 0x00
    #define TILE_FLIP_BITS_X    0x01
    #define TILE_FLIP_BITS_Y    0x02
    #define TILE_FLIP_BITS_XY   (TILE_FLIP_BITS_X | TILE_FLIP_BITS_Y)
    #define TILE_FLIP_MASK      TILE_FLIP_BITS_XY
    #define TILE_FLIP_MIN       0
    #define TILE_FLIP_MIN_FLIP  1
    #define TILE_FLIP_MAX       3

    #define TILE_COLORS_PER_PALETTE   4 // 4 colors per Gameboy color palette
    #define TILE_DMG_PALETTE_COUNT    1 // 1 palettes in DMG mode
    #define TILE_CGB_PALETTE_COUNT    8 // 8 palettes in CGB mode

    #define TILE_DMG_COLORS_MAX (TILE_DMG_PALETTE_COUNT * TILE_COLORS_PER_PALETTE)
    #define TILE_CGB_COLORS_MAX (TILE_CGB_PALETTE_COUNT * TILE_COLORS_PER_PALETTE)


    enum tile_palettes {
        TILE_PAL_0 = 0,  // For DMG: only palette available, for CGB: default palette
        TILE_PAL_1,
        TILE_PAL_2,
        TILE_PAL_3,
        TILE_PAL_4,
        TILE_PAL_5,
        TILE_PAL_6,
        TILE_PAL_7,

        TILE_PAL_DEFAULT = TILE_PAL_0,
        TILE_PAL_MAX     = TILE_PAL_7,
        TILE_PAL_DMG_MAX = TILE_PAL_0,
        TILE_PAL_CGB_MAX = TILE_PAL_7,
        TILE_PAL_MAP_USE_DEFAULT_FROM_TILE = 0xFE,
        TILE_PAL_ERROR   = 0xFF
    };



    enum import_formats {
        IMPORT_FORMAT_GBDK_C_SOURCE = 1, // TODO  // For : GBDK/ZGB Game Boy Dev Kit
        IMPORT_FORMAT_GBR,  // For: Game Boy Tile Designer / GBTD
        IMPORT_FORMAT_GBM,  // For: Game Boy Map Builder / GBMB
        IMPORT_FORMAT_LAST
    };

    // App image bit depths/modes
    enum image_modes {
        IMG_BITDEPTH_INDEXED = 1,
        IMG_BITDEPTH_INDEXED_ALPHA = 2,
        IMG_BITDEPTH_RGB = 3,
        IMG_BITDEPTH_RGB_ALPHA = 4,
        IMG_BITDEPTH_LAST
    };

    // Export formats
    enum export_formats {
        EXPORT_FORMAT_GBDK_C_SOURCE = 1, // For : GBDK/ZGB Game Boy Dev Kit
        EXPORT_FORMAT_GBR,  // For: Game Boy Tile Designer / GBTD
        EXPORT_FORMAT_GBM,  // For: Game Boy Map Builder / GBMB
        EXPORT_FORMAT_LAST
    };

    enum tile_process_modes {
        MODE_DMG_4_COLOR,
        MODE_CGB_32_COLOR
    };


    typedef struct {
        uint16_t gb_mode;              // MODE_DMG_4_COLOR, MODE_CGB_32_COLOR
        uint16_t tile_dedupe_flips;    // only if (gb_mode == PROC_MODE_CGB_32_COLOR)
        uint16_t tile_dedupe_palettes;

    } tile_process_options;


    // Tile Map Entry records
    typedef struct {
        uint16_t id; // if TILES_MAX_DEFAULT > 255, this must be larger than uint8_t
        uint8_t flip_bits;
        uint8_t palette_num;
    } tile_map_entry;


    // Tile Map
    typedef struct {
        uint16_t width_in_tiles;
        uint16_t height_in_tiles;
        uint16_t tile_width;
        uint16_t tile_height;
        uint16_t map_width;
        uint16_t map_height;
        uint32_t size;
        uint16_t * tile_id_list; // if TILES_MAX_DEFAULT > 255, this must be larger than uint8_t
        uint8_t * flip_bits_list;
        uint8_t * palette_num_list;

        // Options
        tile_process_options options;
    } tile_map_data;


    // Individual Tile from Tile Set
    typedef struct {
        uint64_t  hash[4]; // 4 hash calcs: normal, flip-x, flip-y, flip-xy
        uint8_t   raw_bytes_per_pixel;
        uint16_t  raw_width;
        uint16_t  raw_height;
        uint32_t  raw_size_bytes;     // size in bytes // TODO
        uint32_t  encoded_size_bytes; // size in bytes
        uint8_t * p_img_raw;
        uint8_t * p_img_encoded;
        uint8_t   palette_num;
    } tile_data;

    // Tile Set (composed of individual tiles)
    typedef struct {
        uint8_t  tile_bytes_per_pixel; // TODO: convert me to tiles[n].raw_bytes_per_pixel, raw_width, raw_height
        uint16_t tile_width;
        uint16_t tile_height;
        uint32_t tile_size;  // size in bytes
        uint32_t tile_count;
        tile_data tiles[TILES_MAX_DEFAULT];
    } tile_set_data;


    void           tilemap_free_tile_set(void);
    void           tilemap_free_resources(void);
    static int32_t check_dimensions_valid(image_data * p_src_img);
    unsigned char  process_tiles(image_data * p_src_img);
    unsigned char  tilemap_export_process(image_data * p_src_img, tile_process_options export_options);
    int32_t        tilemap_initialize(image_data * p_src_img, tile_process_options export_options);
    int32_t        tilemap_save(const int8_t * filename, uint32_t export_format);

    tile_map_data * tilemap_get_map(void);
    tile_set_data * tilemap_get_tile_set(void);

    int32_t tilemap_get_image_of_deduped_tile_set(image_data * p_img);

#endif // LIB_TILEMAP_HEADER

