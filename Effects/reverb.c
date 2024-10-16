#include "main.h"

#define SAMPLE_RATE      16000  // Audio sample rate

void ApplyReverb(int16_t *buffer, int16_t *output, uint32_t size, float reverb_time, float mix)
{
	reverb_time = reverb_time * (1000.0/4096.0);
	mix = mix * (1.4/4096.0);
    int16_t reverb_buffer[48000];  // Reverb buffer for 1 second at 48kHz
    static uint32_t reverb_index = 0;     // Circular buffer index for reverb

    // Convert reverb time from milliseconds to number of samples
    uint32_t reverb_samples = (uint32_t)(reverb_time * SAMPLE_RATE / 1000.0f);

    // Number of taps (reflections)
    int num_taps = 7;  // Increase number of taps for richer reverb
    float tap_times[] = {0.02f, 0.05f, 0.08f, 0.1f, 0.12f, 0.15f, 0.2f};  // In seconds (varying delays)
    float tap_gains[] = {0.7f, 0.5f, 0.3f, 0.2f, 0.1f, 0.05f, 0.02f};     // Gains for each reflection

    for (uint32_t i = 0; i < size; i++)
    {
        int32_t reverb_output = 0;

        // Apply multiple taps (reflections)
        for (int tap = 0; tap < num_taps; tap++)
        {
            // Convert tap time to number of samples
            uint32_t tap_delay_samples = (uint32_t)(tap_times[tap] * SAMPLE_RATE);

            // Calculate index for each tap
            uint32_t tap_index = (reverb_index + 48000 - tap_delay_samples) % 48000;

            // Retrieve delayed sample and apply gain for the tap
            int16_t tap_sample = reverb_buffer[tap_index];
            reverb_output += (int32_t)(tap_gains[tap] * tap_sample);
        }

        // Mix the original signal with the reverb output, using a dry/wet mix
        int32_t final_output = (int32_t)((1.0f - mix) * buffer[i] + mix * reverb_output);

        // Apply the reverb effect to the buffer
        reverb_buffer[reverb_index] = buffer[i];  // Store the current sample in the reverb buffer

        // Clamp the output to prevent clipping and distortion
        if (final_output > 32767) final_output = 32767;
        else if (final_output < -32768) final_output = -32768;

        // Write the processed sample back to the buffer
        output[i] = (int16_t)final_output;

        // Update the circular buffer index
        reverb_index = (reverb_index + 1) % 48000;
    }
}
