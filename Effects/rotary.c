#include "main.h"
#include <math.h>

#define SAMPLE_RATE DEFAULT_AUDIO_IN_FREQ
#define ROTARY_MAX_DELAY 100 // Maximum delay for the rotary effect in samples (for Doppler effect)
#define DELAY_BUFFER_SIZE 256 // Adjust as needed

void ApplyRotaryEffect(int16_t *buffer, int16_t *output, uint32_t size, float depth, float rate) {
	depth = depth * (1.6/4096.0);
	rate = rate * (8.0/4096.0);
    static float phase = 0.0f;
    static float last_amplitude_mod = 1.0f; // Last amplitude for smoothing
    static float delay_buffer[DELAY_BUFFER_SIZE] = {0}; // Delay buffer
    static uint32_t delay_index = 0; // Current write index in the delay buffer

    // Calculate modulation step based on the modulation frequency
    float modulation_step = 2.0f * M_PI * rate / SAMPLE_RATE;

    for (uint32_t i = 0; i < size; i++)
    {
        // Amplitude modulation using a sine wave
        float amplitude_mod = 1.0f + depth * sinf(phase);

        // Smooth the amplitude modulation using a low-pass filter
        last_amplitude_mod += (amplitude_mod - last_amplitude_mod) * 0.2f; // Adjust 0.2 for smoother response

        // Doppler effect simulation (delay line modulation)
        int32_t delay_samples = (int32_t)(ROTARY_MAX_DELAY * (0.5f + 0.5f * sinf(phase)));
        uint32_t read_index = (delay_index + DELAY_BUFFER_SIZE - delay_samples) % DELAY_BUFFER_SIZE;

        // Interpolation for smoother delay
        uint32_t next_index = (read_index + 1) % DELAY_BUFFER_SIZE;

        // Calculate the fractional part for interpolation
        float fractional = (ROTARY_MAX_DELAY * (0.5f + 0.5f * sinf(phase))) - delay_samples;

        // Apply amplitude modulation and delayed sample (Doppler effect)
        int16_t delayed_sample = (int16_t)(delay_buffer[read_index] * (1.0f - fractional) + delay_buffer[next_index] * fractional);

        // Combine the original and delayed sample with smoothing
        output[i] = (int16_t)((buffer[i] * last_amplitude_mod + delayed_sample) * 0.5f); // Average for smoothness

        // Clamp to avoid distortion
        if (output[i] > INT16_MAX) output[i] = INT16_MAX;
        else if (output[i] < INT16_MIN) output[i] = INT16_MIN;

        // Store the current sample in the delay buffer
        delay_buffer[delay_index] = output[i];
        delay_index = (delay_index + 1) % DELAY_BUFFER_SIZE;

        // Increment the phase for the modulation
        phase += modulation_step;
        if (phase > 2.0f * M_PI)
        {
            phase -= 2.0f * M_PI;
        }
    }
}
