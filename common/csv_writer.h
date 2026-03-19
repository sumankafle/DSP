/**
 * @file csv_writer.h
 * @brief Simple CSV file writer for exporting DSP data.
 * @author suman
 * @date 2024/2/28
 *
 * Writes signal data to CSV files for plotting with Gnuplot
 * or importing into spreadsheets / Python / MATLAB.
 */

#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include "dsp_types.h"
#include <stdio.h>

/**
 * @brief Write a single float column to a CSV file.
 *
 * Format: "index,column_name\n" followed by "i,value\n" per sample.
 *
 * @param[in] filename    Output file path.
 * @param[in] data        Float array to write.
 * @param[in] len         Number of samples.
 * @param[in] col_name    Column header name (e.g. "amplitude").
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus csv_write_single(const char *filename, const float *data,
                           int len, const char *col_name);

/**
 * @brief Write two float columns to a CSV file.
 *
 * Format: "index,col1_name,col2_name\n".
 * Useful for comparing input vs. output signals.
 *
 * @param[in] filename    Output file path.
 * @param[in] col1        First data column.
 * @param[in] col2        Second data column.
 * @param[in] len         Number of samples (both columns same length).
 * @param[in] col1_name   Header for first column.
 * @param[in] col2_name   Header for second column.
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus csv_write_double(const char *filename,
                           const float *col1, const float *col2, int len,
                           const char *col1_name, const char *col2_name);

/**
 * @brief Write complex magnitude spectrum to CSV.
 *
 * Format: "bin,magnitude\n". Writes bins 0..len-1.
 *
 * @param[in] filename  Output file path.
 * @param[in] spectrum  Complex spectrum array.
 * @param[in] len       Number of bins to write.
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus csv_write_spectrum(const char *filename, const Complex *spectrum,
                             int len);

#endif /* CSV_WRITER_H */
