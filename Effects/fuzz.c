#include "main.h"

#define SMOOTHING_FACTOR 0.8f  // Adjust this factor for smoothing (0 < factor < 1)

void ApplyFuzz(int16_t *buffer, int16_t *output, uint32_t size, float gain, float depth, int16_t threshold)
{
	gain = gain * (3.0 / 4096.0);
	depth = depth * (2.4 / 4096.0);
	threshold = threshold * (30000.0 / 4096.0);
    static float previous_sample = 0; // To store the previous output sample

    for (uint32_t i = 0; i < size; i++)
    {
        // Apply gain
        int32_t sample = (int32_t)(buffer[i] * gain);

        // Smooth the output by applying a simple low-pass filter
        sample = (int32_t)(SMOOTHING_FACTOR * previous_sample + (1 - SMOOTHING_FACTOR) * sample);

        // Soft clipping with additional shaping for fuzz effect
        if (sample > threshold)
        {
            sample = threshold + (sample - threshold) * depth * (1.0f - ((float)(sample - threshold) / (INT16_MAX - threshold))); // Smoother clipping
        }
        else if (sample < -threshold)
        {
            sample = -threshold + (sample + threshold) * depth * (1.0f - ((float)(-sample - threshold) / (INT16_MIN + threshold))); // Smoother clipping
        }

        // Apply non-linear shaping for richer fuzz sound
        sample = (sample * sample) >> 15; // Non-linear shaping

        // Smooth transition to avoid abrupt changes
        sample = (int16_t)(sample * 0.5f + previous_sample * 0.5f); // Smooth output

        // Store back the fuzzed sample
        output[i] = (int16_t)sample;

        // Update previous sample
        previous_sample = (float)output[i];
    }
}
