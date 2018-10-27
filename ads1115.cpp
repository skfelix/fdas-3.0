#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>
#include <stdlib.h>

#include <signal.h>

#include <iio.h>

using namespace std;

static bool stop = false;
static const char* device_name = "ads1015";
static const char* trigger_name = "instance2";

static int buffer_length = 1;
static struct iio_device *dev;
static struct iio_context *ctx;
static struct iio_buffer  *rxbuf;
vector <struct iio_channel *> channels;
static int channel_count;



static void handle_stop_sig(int sig) {
  cout << "Shutting down..." << endl;
  stop = true;
}


static void shutdown() {
  if (rxbuf)
    iio_buffer_destroy(rxbuf);

  if (dev)
    iio_device_set_trigger(dev, NULL);

  if (ctx)
    iio_context_destroy(ctx);

  exit(0);
}


int main (int argc, char *argv[]) {
  // Listen to ctrl+c to exit
  signal(SIGINT, handle_stop_sig);

  assert((ctx = iio_create_local_context()) && "No context");
  assert(iio_context_get_devices_count(ctx) > 0 && "No devices");
  

  //Setup trigger
  system("/bin/bash setup_trigger_ads1115.sh");

  dev = iio_context_find_device(ctx, device_name);
  if (!dev) {
    perror("device not found");
    shutdown();
  }

  channel_count = iio_device_get_channels_count(dev);
  printf("there are %d channels\n", channel_count);
  for (int i=4; i<channel_count; ++i) {
    struct iio_channel *chn = iio_device_get_channel(dev, i);
//    if (iio_channel_is_scan_element(chn)) {
      channels.push_back(chn);
      iio_channel_enable(chn);
//    }
  }
  
  if (channels.size() == 0) {
    cerr << "No channels available in device" << endl;
    shutdown();
  }

  struct iio_device *trigger = iio_context_find_device(ctx, trigger_name);
  if (!trigger) {
    cerr << "Could not find trigger '" << trigger_name << "'." << endl;
    shutdown();
  }
  if (!iio_device_is_trigger(trigger)) {
    cerr << "Device '" << trigger_name << "' is not a trigger." << endl;
    shutdown();
  }

  if (iio_device_set_trigger(dev, trigger)) {
    perror("Could not set trigger\n");
    shutdown();
  }

  
  rxbuf = iio_device_create_buffer(dev, buffer_length, false);
  if (!rxbuf) {
    perror("Could not create buffer");
    shutdown();
  }
  
  while (!stop) {
    ssize_t nrx = iio_buffer_refill(rxbuf);
    if (nrx < 0) {
      cerr << "Error refilling buffer: " << nrx << endl;
      shutdown();
    }

    ptrdiff_t p_inc = iio_buffer_step(rxbuf);
    void *p_end = iio_buffer_end(rxbuf);


    for (auto& chn: channels) {
      const struct iio_data_format *fmt = iio_channel_get_data_format(chn);
      unsigned int repeat = fmt->repeat;
      printf("%s ", iio_channel_get_id(chn));
      
      for (void *p_dat=iio_buffer_first(rxbuf, chn);
           p_dat<p_end; p_dat+=p_inc) {
        for (int j=0; j<repeat; ++j) {
          if (fmt->length/8 == sizeof(int16_t)) {
            int16_t converted;
            iio_channel_convert(chn, &converted, &((int16_t *)p_dat)[j]);
            printf("%i \t", (int)converted);
          } else if (fmt->length/8 == sizeof(int64_t)) {
            printf("%lld \t", (long long int)((int64_t *)p_dat)[j]);
          }
        }
      }
    }

    printf("\n");
  }

  shutdown();
  
  return EXIT_SUCCESS;
}

