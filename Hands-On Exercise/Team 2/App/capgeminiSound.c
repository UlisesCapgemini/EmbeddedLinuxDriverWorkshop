/**
 * @file
 * @brief Console inferface program from user space to use capgemini mic driver
 *
 * @details [Optional: More detailed description of the file]
 *
 * @author Victor M.
 * @date 14-08-2025
 *
 * @version 1.0
 * @note Changelog:
 * - 14-08-2025: User space program draft - Victor M. (vmartin2-cap)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alsa/asoundlib.h> 
/* Placeholder for PCM */
#define CS_DEFAULT_DURATION 5U
#define CS_DEFAULT_RATE 48000UL
#define CS_DEFAULT_CHANNELS 1U
#define CS_DEFAULT_FORMAT SND_PCM_FORMAT_S16_LE
#define CS_DEFAULT_FRAMES 1024U
#define CS_DEFAULT_DEVICE "default"

char last_recording_path[256] = "~/capgeminiSound_tmp/lastRecording.wav";
const char CS_Arg_Record[] = "--record";
const char CS_Arg_Play[] ="--play";
FILE *wav_file;
char usage[] = "Usage run on your shell: capgeminiSound --record <file.wav> | --play [file.wav]\n \
            default temporal folder: ~/capgeminiSound_tmp/lastRecording.wav";
/* Internal operations */
void CS_record_audio(const char *filepath);
void CS_play_audio(const char *filepath);
/* End of intenal */
int main(int argc, char *argv[]);

/**
 * @brief Entry point for the capgeminiSound CLI application.
 *
 * This function parses command-line arguments and invokes either the
 * recording or playback functionality based on the provided options.
 *
 * Supported options:
 * - --record <file.wav>: Records 5 seconds of audio and saves it to the specified file.
 * - --play [file.wav]: Plays the specified file or the last recorded file if none is provided.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Returns 0 on success, 1 on error.
 */
int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        fprintf(stderr, "%s\n",usage);
        return 1;
    }

    if (strcmp(argv[1], CS_Arg_Record) == 0) 
    {
        if (argc < 3) {
            fprintf(stderr, "Missing file path for recording.\n");
            return 1;
        }
        CS_record_audio(argv[2]);
    } 
    else if (strcmp(argv[1], CS_Arg_Play) == 0)
    {
        CS_play_audio(argc >= 3 ? argv[2] : NULL);
    }
    else
    {
        fprintf(stderr, "Unknown option: %s\n", argv[1]);
        return 1;
    }

    return 0;
}

/**
 * @brief Records audio from the default input device and saves it to a WAV file.
 *
 * This function Records audio using xxx API and writes it to the specified file path.
 *
 * @param filepath Path to the output WAV file.
 */
void CS_record_audio(const char *filepath) 
{
    

    snd_pcm_t *pcm_handle = NULL;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames = CS_DEFAULT_FRAMES; /* TODO assuming 1024 bu */
    /* defs for WAV file write */
    unsigned int rate = CS_DEFAULT_RATE;
    int dir;
    /* Open PCM device for recording */
    /* TODO Configure capture ie */
    if (snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_CAPTURE, 0) < 0) {
        fprintf(stderr, "CapgeminiSound ERR: Error opening PCM device. check if sound card is available\n");
        return;
    }

    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(pcm_handle, params);
    snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(pcm_handle, params, CS_DEFAULT_CHANNELS);
    snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, &dir);
    snd_pcm_hw_params(pcm_handle, params);
    snd_pcm_hw_params_get_period_size(params, &frames, &dir); 
    snd_pcm_hw_params_free(params);
    
    int bytes_per_sample = 2; /* 16-bit*/ 
    int total_frames = CS_DEFAULT_DURATION * rate; /* TODO to modify this duration */
    int buffer_size = frames * CS_DEFAULT_CHANNELS * bytes_per_sample;
    char *buffer;
    
   

    buffer = (char *)malloc(buffer_size);


    printf("Recording to: %s\n", filepath);
    /* TODO: Implement play with micdriver */
    strncpy(last_recording_path, filepath, sizeof(last_recording_path));
    /* Open PCM device for recording */
    /* Allocate hardware parameters */
    /* Open WAV file */
    wav_file = fopen(filepath, "wb");
    if (!wav_file) {
        fprintf(stderr, "Error opening WAV file.\n");
        /* handle pcm close: snd_pcm_close(pcm_handle);*/
        /*free(buffer);*/
        return;
    }
    /* Write WAV header */
    int data_chunk_pos = 44;
    int data_chunk_size = total_frames * CS_DEFAULT_CHANNELS * bytes_per_sample; /* TODO to modify this size */
    int file_size = data_chunk_pos + data_chunk_size - 8;

    fwrite("RIFF", 1, 4, wav_file);
    fwrite(&file_size, 4, 1, wav_file);
    fwrite("WAVEfmt ", 1, 8, wav_file);
    int fmt_chunk_size = 16;
    short audio_format = 1;
    short num_channels = CS_DEFAULT_CHANNELS;
    int byte_rate = rate * num_channels * bytes_per_sample;
    short block_align = num_channels * bytes_per_sample;
    short bits_per_sample = 16;

    fwrite(&fmt_chunk_size, 4, 1, wav_file);
    fwrite(&audio_format, 2, 1, wav_file);
    fwrite(&num_channels, 2, 1, wav_file);
    fwrite(&rate, 4, 1, wav_file);
    fwrite(&byte_rate, 4, 1, wav_file);
    fwrite(&block_align, 2, 1, wav_file);
    fwrite(&bits_per_sample, 2, 1, wav_file);
    fwrite("data", 1, 4, wav_file);
    fwrite(&data_chunk_size, 4, 1, wav_file);

    /* Record and write audio data */ 
    for (int i = 0; i < total_frames / frames; ++i) {
        /* snd_pcm_readi(pcm_handle, buffer, frames); */
        fwrite(buffer, buffer_size, 1, wav_file);
    }

    fclose(wav_file);
    snd_pcm_close(pcm_handle);
    free(buffer);

    printf("Recording saved to %s\n", filepath);




}

/**
 * @brief Plays audio from the default input device and saves it to a WAV file.
 *
 * This function captures audio using xxx API and writes it to the specified file path.
 *
 * @param filepath Path to the output WAV file.
 */
void CS_play_audio(const char *filepath)
{
    const char *target = filepath ? filepath : last_recording_path;
    printf("Playing: %s\n", target);
    /*TODO: Implement play with micdriver */
}