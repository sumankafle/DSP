/**
 * @file circular_buffer.h
 * @brief Fixed-size circular (ring) buffer for streaming DSP.
 * @author suman
 * @date 2024/2/28
 *
 * A FIFO circular buffer useful for real-time sample buffering,
 * delay lines, and sliding-window operations.
 *
 * Example usage:
 * @code
 *   CircularBuffer cb;
 *   float storage[256];
 *   cb_init(&cb, storage, 256);
 *   cb_push(&cb, 1.0f);
 *   float val;
 *   cb_pop(&cb, &val);  // val == 1.0f
 * @endcode
 */

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include "dsp_types.h"

/**
 * @brief Circular buffer state.
 *
 * The buffer does not own the storage array -- the caller
 * provides it via cb_init().
 */
typedef struct {
    float *buf;     /**< Backing storage (caller-owned) */
    int    capacity; /**< Maximum number of elements */
    int    head;    /**< Write index */
    int    tail;    /**< Read index */
    int    count;   /**< Current number of stored elements */
} CircularBuffer;

/**
 * @brief Initialize a circular buffer with external storage.
 *
 * @param[out] cb        Buffer to initialize.
 * @param[in]  storage   Pre-allocated float array.
 * @param[in]  capacity  Size of the storage array.
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus cb_init(CircularBuffer *cb, float *storage, int capacity);

/**
 * @brief Reset the buffer to empty state.
 * @param[in,out] cb  Buffer to reset.
 */
void cb_reset(CircularBuffer *cb);

/**
 * @brief Push a sample into the buffer.
 *
 * If the buffer is full, the oldest sample is overwritten.
 *
 * @param[in,out] cb    Circular buffer.
 * @param[in]     value Sample to push.
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus cb_push(CircularBuffer *cb, float value);

/**
 * @brief Pop the oldest sample from the buffer.
 *
 * @param[in,out] cb    Circular buffer.
 * @param[out]    value Popped value.
 * @return DSP_SUCCESS, DSP_ERR_PARAM if empty.
 */
DspStatus cb_pop(CircularBuffer *cb, float *value);

/**
 * @brief Peek at a sample by offset from the oldest element.
 *
 * @param[in]  cb      Circular buffer.
 * @param[in]  offset  0 = oldest, count-1 = newest.
 * @param[out] value   Peeked value.
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus cb_peek(const CircularBuffer *cb, int offset, float *value);

/**
 * @brief Check if the buffer is full.
 * @param[in] cb  Circular buffer.
 * @return true if full.
 */
static inline bool cb_full(const CircularBuffer *cb)
{
    return cb && cb->count == cb->capacity;
}

/**
 * @brief Check if the buffer is empty.
 * @param[in] cb  Circular buffer.
 * @return true if empty.
 */
static inline bool cb_empty(const CircularBuffer *cb)
{
    return !cb || cb->count == 0;
}

#endif /* CIRCULAR_BUFFER_H */
