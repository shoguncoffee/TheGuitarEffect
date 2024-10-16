#include "main.h"

void ApplyDistortion(int16_t *buffer, int16_t *output, uint32_t size, float gain, int16_t threshold)
{
	gain = gain * (2.4 / 4096.0);
	threshold = threshold * (50000.0 / 4096.0);
    for (uint32_t i = 0; i < size; i++)
    {
        // Apply gain
        int32_t sample = (int32_t)(buffer[i] * gain);

        // Clip the signal using soft clipping to avoid harsh noise
        if (sample > threshold)
        {
            sample = threshold + (sample - threshold) / 5; // Soft clipping for smoother sound
        }
        else if (sample < -threshold)
        {
            sample = -threshold + (sample + threshold) / 5;
        }

        // Store back the distorted sample
        output[i] = (int16_t)sample;
    }
}
