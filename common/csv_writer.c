/**
 * @file csv_writer.c
 * @brief CSV file writer implementation.
 * @author suman
 * @date 2024/2/28
 */

#include "csv_writer.h"
#include <math.h>

DspStatus csv_write_single(const char *filename, const float *data,
                           int len, const char *col_name)
{
    if (!filename || !data || len <= 0 || !col_name)
        return DSP_ERR_PARAM;

    FILE *fp = fopen(filename, "w");
    if (!fp) return DSP_ERR_PARAM;

    fprintf(fp, "index,%s\n", col_name);
    for (int i = 0; i < len; i++)
        fprintf(fp, "%d,%g\n", i, (double)data[i]);

    fclose(fp);
    return DSP_SUCCESS;
}

DspStatus csv_write_double(const char *filename,
                           const float *col1, const float *col2, int len,
                           const char *col1_name, const char *col2_name)
{
    if (!filename || !col1 || !col2 || len <= 0 || !col1_name || !col2_name)
        return DSP_ERR_PARAM;

    FILE *fp = fopen(filename, "w");
    if (!fp) return DSP_ERR_PARAM;

    fprintf(fp, "index,%s,%s\n", col1_name, col2_name);
    for (int i = 0; i < len; i++)
        fprintf(fp, "%d,%g,%g\n", i, (double)col1[i], (double)col2[i]);

    fclose(fp);
    return DSP_SUCCESS;
}

DspStatus csv_write_spectrum(const char *filename, const Complex *spectrum,
                             int len)
{
    if (!filename || !spectrum || len <= 0)
        return DSP_ERR_PARAM;

    FILE *fp = fopen(filename, "w");
    if (!fp) return DSP_ERR_PARAM;

    fprintf(fp, "bin,magnitude\n");
    for (int i = 0; i < len; i++) {
        double mag = sqrt((double)spectrum[i].real * spectrum[i].real +
                          (double)spectrum[i].imag * spectrum[i].imag);
        fprintf(fp, "%d,%g\n", i, mag);
    }

    fclose(fp);
    return DSP_SUCCESS;
}
