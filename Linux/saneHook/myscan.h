#ifdef _AIX
# include "../include/lalloca.h"                /* MUST come first for AIX! */
#endif

// #include "../include/sane/config.h"


#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <pwd.h>
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <getopt.h>
#include <time.h>
#include <stdint.h>

#include <sane/sane.h>
// #include "../include/sane/sanei.h"
#include <sane/saneopts.h>

// #include "sicc.h"
// #include "stiff.h"

// #include "../include/md5.h"

typedef struct
{
	uint8_t *data;
	int width; /*WARNING: this is in bytes, get pixel width from param*/
	int height;
	int x;
	int y;
} Image;

typedef struct Mysane
{
    SANE_Int sane_backend_versioncode;
    char *backend;
    char *backend_translation;
    char *device_set_filename;
    char *xsane_rc_set_filename;
    char *output_filename;
    char *dummy_filename;

    SANE_Int sensitivity;


    SANE_Handle *dev;
    const char *dev_name;
    // WellKnownOptions well_known;
    int num_elements;
    // DialogElement *element;
    // u_int rebuild : 1;
    int pixelcolor;
    int scanning;
    int reading_data;
    int cancel_scan;

    int batch_scan_load_default_list;	/* load default list at program startup flag */
    int batch_loop; /* is set when batch scanning and not last scan */
    int batch_scan_use_stored_scanmode;
    int batch_scan_use_stored_resolution;
    int batch_scan_use_stored_bit_depth;
    SANE_Status status_of_last_scan;



/* previous global stand alone varaibales */
    const char   *prog_name;      /* name of this program, normally "xsane" */
    const char   *device_text;      /* name of the selected device */
    const SANE_Device **devlist;      /* the list of available devices */
    int          selected_dev;        /* the selected device */
    int          num_of_devs;
    int          back_gtk_message_dialog_active;



    /* various scanning related state: */
    SANE_Int depth;
    // size_t num_bytes;
    // size_t bytes_read;
    int read_offset_16;
    char last_offset_16_byte;
    int  lineart_to_grayscale_x;
    int input_tag;
    SANE_Parameters param;
    int adf_page_counter;
    int scan_rotation;


    /* saving and transformation values: */
    FILE *out;
    int xsane_mode;
    int xsane_output_format;
    long header_size;
    int expand_lineart_to_grayscale;
    int reduce_16bit_to_8bit;


    // gfloat *free_gamma_data, *free_gamma_data_red, *free_gamma_data_green, *free_gamma_data_blue;
    // SANE_Int *gamma_data, *gamma_data_red, *gamma_data_green, *gamma_data_blue;
    // u_char *preview_gamma_data_red, *preview_gamma_data_green, *preview_gamma_data_blue;
    // u_char *histogram_gamma_data_red, *histogram_gamma_data_green, *histogram_gamma_data_blue;
    // u_char *histogram_medium_gamma_data_red, *histogram_medium_gamma_data_green, *histogram_medium_gamma_data_blue;



    int block_update_param;
    int block_enhancement_update;

    int broken_pipe; /* for printercommand pipe */

    int cancel_save;

/* -------------------------------------------------- */

/* device preferences: */

/* we have to use double and int here, gint or SANE_Word
   is not allowed because we need a defined size for
   rc_io-routintes that are based on double, int, ... */


/* -------------------------------------------------- */


} Mysane;