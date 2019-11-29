#include <ao/ao.h>
#include <mpg123.h>
#include <string>
#include <iostream>

#include <fcntl.h>
#include <cstring>
#include <unistd.h>

#define BITS 8

ao_device *dev;
int driver;

class audio_sample {
    
public:
    audio_sample(const char*  filename){
        mh = mpg123_new(NULL, &err);
        buffer_size = mpg123_outblock(mh);
        buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

        int fnum = open(filename, O_RDONLY);
        if(fnum < 0){
            printf("ERROR opening file: %s\n", strerror(fnum));
            exit(0);
        }
    
        mpg123_open(mh, filename);
        mpg123_getformat(mh, &rate, &channels, &encoding);
        
        /* set the output format and open the output device */
        format.bits = mpg123_encsize(encoding) * BITS;
        format.rate = rate;
        format.channels = channels;
        format.byte_format = AO_FMT_NATIVE;
        format.matrix = 0; 
    };
    
    ~audio_sample(){
        free(buffer);
        mpg123_close(mh);
        mpg123_delete(mh);
    }
    
    void play(){
        dev = ao_open_live(ao_default_driver_id(), &format, NULL);
        /* decode and play */
        while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK)
           ao_play(dev, reinterpret_cast<char*>(buffer), done);
    }
    
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;
    
    ao_sample_format format;
    int channels, encoding;
    long rate;
};

int main(int argc, char *argv[])
{
    
    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    
    int err = mpg123_init();
    if(err != MPG123_OK) {
       fprintf(stderr, "Basic setup goes wrong: %s", mpg123_plain_strerror(err));
    }
    
    //audio samples need to be a certain minimum length
    audio_sample dining_room("ophelia.mp3"); 

    /* clean up */
    ao_close(dev);
    
    mpg123_exit();
    ao_shutdown();

    return 0;
}
