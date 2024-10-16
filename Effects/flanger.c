#include "main.h"
#include "math.h"

#define SAMPLE_RATE 16000
#define TWO_PI 6.2831853f

void ApplyFlanger(int16_t *buffer, int16_t *output, uint32_t size, float delay_depth, float rate, float feedback, float mix)
{
	delay_depth = delay_depth * (6.0/4096.0);
	rate = rate * (1.0/4096.0);
	feedback = feedback * (0.6/4096.0);
	mix = mix * (1.2/4096);
    static float lfo_phase = 0.0f;  // LFO phase for modulation
    static uint32_t delay_index = 0;     // Circular buffer index for delay
    float smoothed_feedback = 0.0f;      // To smooth out feedback over time

    for (uint32_t i = 0; i < size; i++)
    {
        // Modulate the delay time smoothly using a sine wave
        float lfo = (sin(TWO_PI * lfo_phase) + 1.0f) / 2.0f;  // LFO oscillates between 0 and 1
        float delay_time = delay_depth * lfo;  // Calculate delay time in milliseconds
        uint32_t delay_samples = (uint32_t)(delay_time * SAMPLE_RATE / 1000.0f);  // Convert delay time to samples

        // Circular buffer index for the delayed sample
        uint32_t delayed_index = (delay_index + 48000 - delay_samples) % 48000;

        // Retrieve the delayed sample from the delay buffer
        int16_t delayed_sample = buffer[delayed_index];

        // Apply smoother feedback by mixing current and previous feedback
        smoothed_feedback = (feedback * delayed_sample) + ((1.0f - feedback) * smoothed_feedback);

        // Mix the original signal with the delayed sample, using a dry/wet mix
        int32_t flanger_sample = (int32_t)((1.0f - mix) * buffer[i] + mix * smoothed_feedback);

        // Clamp the output to prevent clipping and distortion
        if (flanger_sample > 32767) flanger_sample = 32767;
        else if (flanger_sample < -32768) flanger_sample = -32768;

        // Write the processed sample back to the buffer
        output[i] = (int16_t)flanger_sample;

        // Update the circular buffer index
        delay_index = (delay_index + 1) % 48000;

        // Advance the LFO phase based on the rate
        lfo_phase += rate / SAMPLE_RATE;
        if (lfo_phase >= 1.0f)
        {
            lfo_phase -= 1.0f;  // Keep the LFO phase within [0, 1]
        }
    }
}
