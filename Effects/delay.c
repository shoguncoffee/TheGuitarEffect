//delay
#include "main.h"

#define SAMPLE_RATE 16000

void ApplyDelay(int16_t *buffer, int16_t *output, uint32_t size, float delay_time, float feedback, float mix)
{
	delay_time = delay_time * (900.0/4096.0);
	feedback = feedback * (0.8/4096.0);
	mix = mix * (1.2/4096);
    int16_t delay_buffer[48000];  // Delay buffer for 1 second at 48kHz
    static uint32_t delay_index = 0;     // Circular buffer index for delay

    // Convert delay time from milliseconds to number of samples
    uint32_t delay_samples = (uint32_t)(delay_time * SAMPLE_RATE / 1000.0f);

    for (uint32_t i = 0; i < size; i++)
    {
        // Circular buffer index for the delayed sample
        uint32_t delayed_index = (delay_index + 48000 - delay_samples) % 48000;

        // Retrieve the delayed sample from the delay buffer
        int16_t delayed_sample = delay_buffer[delayed_index];

        // Mix the original signal with the delayed sample, using a dry/wet mix
        int32_t delay_output = (int32_t)((1.0f - mix) * buffer[i] + mix * delayed_sample);

        // Apply feedback (add delayed signal back into the delay buffer)
        delay_buffer[delay_index] = (int16_t)(buffer[i] + feedback * delayed_sample);

        // Clamp the output to prevent clipping and distortion
        if (delay_output > 32767) delay_output = 32767;
        else if (delay_output < -32768) delay_output = -32768;

        // Write the processed sample back to the buffer
        output[i] = (int16_t)delay_output;

        // Update the circular buffer index
        delay_index = (delay_index + 1) % 48000;
    }
}
