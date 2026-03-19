/**
 * @file circular_buffer.c
 * @brief Circular buffer implementation.
 * @author suman
 * @date 2024/2/28
 */

#include "circular_buffer.h"

DspStatus cb_init(CircularBuffer *cb, float *storage, int capacity)
{
    if (!cb || !storage || capacity <= 0) return DSP_ERR_PARAM;

    cb->buf      = storage;
    cb->capacity = capacity;
    cb->head     = 0;
    cb->tail     = 0;
    cb->count    = 0;
    return DSP_SUCCESS;
}

void cb_reset(CircularBuffer *cb)
{
    if (!cb) return;
    cb->head  = 0;
    cb->tail  = 0;
    cb->count = 0;
}

DspStatus cb_push(CircularBuffer *cb, float value)
{
    if (!cb) return DSP_ERR_PARAM;

    cb->buf[cb->head] = value;
    cb->head = (cb->head + 1) % cb->capacity;

    if (cb->count == cb->capacity) {
        /* overwrite oldest -- advance tail */
        cb->tail = (cb->tail + 1) % cb->capacity;
    } else {
        cb->count++;
    }
    return DSP_SUCCESS;
}

DspStatus cb_pop(CircularBuffer *cb, float *value)
{
    if (!cb || !value || cb->count == 0) return DSP_ERR_PARAM;

    *value = cb->buf[cb->tail];
    cb->tail = (cb->tail + 1) % cb->capacity;
    cb->count--;
    return DSP_SUCCESS;
}

DspStatus cb_peek(const CircularBuffer *cb, int offset, float *value)
{
    if (!cb || !value || offset < 0 || offset >= cb->count)
        return DSP_ERR_PARAM;

    int idx = (cb->tail + offset) % cb->capacity;
    *value = cb->buf[idx];
    return DSP_SUCCESS;
}
