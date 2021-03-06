#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <signal.h>
#include <iio.h>
#include <stdio.h>
#include <time.h>


using namespace std;

static bool stop = false;
static const char *device_name = "bmp180";

static int buffer_length = 1;
static struct iio_device *dev;
static struct iio_context *ctx;
vector <struct iio_channel *> channels;
vector <const char *> attributes;
static int channel_count, attr_count;
const char *attr;
double var;

struct timespec tp;

static void handle_stop_sig(int sig) {
	cout << "Shutting down..." << endl;
	stop = true;
}


static void shutdown() {
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

	/* Declaration of file */
	FILE *fptr;
	fptr = fopen ("/root/iio-non/output_data/bmp180.txt", "w");
	if(fptr == NULL) {
		printf("Failed opening the file!");
		exit(1);
	}

	dev = iio_context_find_device(ctx, device_name);
	if (!dev) {
		perror("device not found");
		shutdown();
	}

	channel_count = iio_device_get_channels_count(dev);
	printf("there are %d channels\n", channel_count);
	for (int i=0; i<channel_count; ++i) {
		struct iio_channel *chn = iio_device_get_channel(dev, i);
		channels.push_back(chn);
		attr_count = iio_channel_get_attrs_count(chn);
		printf("%s \t", iio_channel_get_id(chn));
		fprintf(fptr,"%s \t", iio_channel_get_id(chn));
		for (int i=0; i<attr_count; ++i) {
			attr = iio_channel_get_attr(chn, i);
			attributes.push_back(attr);
			//printf("%s \t", attr);
		}
		//printf("\n");
	}
	printf("\n");
	fprintf(fptr, "\n");

	// channel sequence: 0-temp, 1-pressure
	// temp attribute sequence: 0-scale_available, 1-raw, 2-scale
	// pressure attribute sequence: 0-scale_available, 1-raw, 2-scale

	if (channels.size() == 0) {
		cerr << "No channels available in device" << endl;
		shutdown();
	}

	while (!stop) {
		clock_gettime(CLOCK_REALTIME, &tp);
		for (int i=0; i<channel_count; ++i) {
			clock_gettime(CLOCK_REALTIME, &tp);
			iio_channel_attr_read_double(channels[i], attributes[2], &var);
			printf("%lf \t", var);
			fprintf(fptr,"%lf \t", var);
		}
		printf("%lld\n", tp.tv_sec + tp.tv_nsec);
		fprintf(fptr,"%lld\n", tp.tv_sec + tp.tv_nsec);
	}

	fclose(fptr);
	shutdown();

	return EXIT_SUCCESS;
}
