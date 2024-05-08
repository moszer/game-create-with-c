#include <AudioToolbox/AudioToolbox.h>

// Function to handle output buffer
void HandleOutputBuffer(void *inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer) {
    // Nothing to do here for this basic example
}

int main() {
    // Specify the path to your .wav file
    CFURLRef soundFileURLRef = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, CFSTR("music.wav"), kCFURLPOSIXPathStyle, false);

    // Open the audio file
    AudioFileID audioFileID;
    OSStatus status = AudioFileOpenURL(soundFileURLRef, kAudioFileReadPermission, 0, &audioFileID);
    if (status != noErr) {
        printf("Error: Couldn't open the audio file\n");
        return 1;
    }

    // Get the audio file format
    AudioStreamBasicDescription audioFormat;
    UInt32 propertySize = sizeof(audioFormat);
    status = AudioFileGetProperty(audioFileID, kAudioFilePropertyDataFormat, &propertySize, &audioFormat);
    if (status != noErr) {
        printf("Error: Couldn't get the audio file format\n");
        return 1;
    }

    // Create the audio queue
    AudioQueueRef audioQueue;
    status = AudioQueueNewOutput(&audioFormat, HandleOutputBuffer, NULL, NULL, NULL, 0, &audioQueue);
    if (status != noErr) {
        printf("Error: Couldn't create the audio queue\n");
        return 1;
    }

    // Define a buffer to hold the audio data
    const int bufferByteSize = 32768; // Adjust this according to your file size
    AudioQueueBufferRef buffer;
    status = AudioQueueAllocateBuffer(audioQueue, bufferByteSize, &buffer);
    if (status != noErr) {
        printf("Error: Couldn't allocate audio queue buffer\n");
        return 1;
    }

    // Read the audio data into the buffer
    UInt32 numPacketsToRead = bufferByteSize / audioFormat.mBytesPerPacket;
    AudioStreamPacketDescription *packetDescs = NULL;
    status = ExtAudioFileRead(audioFileID, &numPacketsToRead, buffer, &packetDescs);
    if (status != noErr) {
        printf("Error: Couldn't read audio data from file\n");
        return 1;
    }

    // Enqueue the buffer
    status = AudioQueueEnqueueBuffer(audioQueue, buffer, 0, NULL);
    if (status != noErr) {
        printf("Error: Couldn't enqueue buffer\n");
        return 1;
    }

    // Start the audio queue
    status = AudioQueueStart(audioQueue, NULL);
    if (status != noErr) {
        printf("Error: Couldn't start audio queue\n");
        return 1;
    }

    // Wait for the audio to finish
    usleep(10000000); // Adjust this according to your file duration

    // Clean up
    AudioQueueStop(audioQueue, true);
    AudioQueueDispose(audioQueue, true);
    AudioFileClose(audioFileID);
    CFRelease(soundFileURLRef);

    return 0;
}
