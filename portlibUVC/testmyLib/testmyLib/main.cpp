#include "libuvc/libuvc.h"
#include <stdio.h>
#include <unistd.h>
/* This callback function runs once per frame. Use it to perform any
 * quick processing you need, or have it put the frame into your application's
 * input queue. If this function takes too long, you'll start losing frames. */
void cb(uvc_frame_t *frame, void *ptr) {
    uvc_frame_t *bgr;
    uvc_error_t ret;
    
    /* We'll convert the image from YUV/JPEG to BGR, so allocate space */

    FILE *fp;
    fp = fopen("alex.h264", "a");
    fwrite(frame->data, 1, frame->data_bytes, fp);
    fclose(fp);
    /* Do the BGR conversion */
//    ret = uvc_mjpeg2rgb(frame, bgr);  //21 up Frame is mjpeg , 21 down bgr ,RGB
//
//    if (ret) {
//        uvc_perror(ret, "uvc_any2bgr");
//        uvc_free_frame(bgr);
//        return;
//    }
    
    
    
    
    //kl
    //
    /* Call a user function:
     *
     * my_type *my_obj = (*my_type) ptr;
     * my_user_function(ptr, bgr);
     * my_other_function(ptr, bgr->data, bgr->width, bgr->height);
     */
    
    /* Call a C++ method:
     *
     * my_type *my_obj = (*my_type) ptr;
     * my_obj->my_func(bgr);
     */
    
    /* Use opencv.highgui to display the image:
     *
     * cvImg = cvCreateImageHeader(
     *     cvSize(bgr->width, bgr->height),
     *     IPL_DEPTH_8U,
     *     3);
     *
     * cvSetData(cvImg, bgr->data, bgr->width * 3);
     *
     * cvNamedWindow("Test", CV_WINDOW_AUTOSIZE);
     * cvShowImage("Test", cvImg);
     * cvWaitKey(10);
     *
     * cvReleaseImageHeader(&cvImg);
     */
    
    uvc_free_frame(bgr);
}

int main2(int argc, char **argv) {
    uvc_context_t *ctx;
    uvc_error_t res;
    uvc_device_t *dev;
    uvc_device_handle_t *devh;
    uvc_device_descriptor_t *descp;
    uvc_device_t **devicelist;
    int hueValue;
    res = uvc_init(&ctx, NULL);
    res = uvc_find_device(ctx, &dev,0x07CA, 0x1510, NULL);
    if (res < 0) {
        uvc_perror(res, "uvc_find_device"); /* no devices found */
    }
    res = uvc_get_device_descriptor(dev, &descp);
    res = uvc_open(dev, &devh);
    uvc_print_diag(devh, stderr);
    
    
    printf("%d",hueValue);
    uvc_set_hue(devh, 10);
    
    printf("brefore free");
    uvc_exit(ctx);
    printf("free");
    return 0;
}

int main(int argc, char **argv) {
    uvc_context_t *ctx;
    uvc_device_t *dev;
    uvc_device_handle_t *devh;
    uvc_stream_ctrl_t ctrl;
    uvc_error_t res;
    
    /* Initialize a UVC service context. Libuvc will set up its own libusb
     * context. Replace NULL with a libusb_context pointer to run libuvc
     * from an existing libusb context. */
    res = uvc_init(&ctx, NULL);
    
    if (res < 0) {
        uvc_perror(res, "uvc_init");
        return res;
    }
    
    puts("UVC initialized");
    
    /* Locates the first attached UVC device, stores in dev */
    res = uvc_find_device(
                          ctx, &dev,
                          0, 0, NULL); /* filter devices: vendor_id, product_id, "serial_num" */
    //alex PID/VID
    if (res < 0) {
        uvc_perror(res, "uvc_find_device"); /* no devices found */
    } else {
        puts("Device found");
        
        /* Try to open the device: requires exclusive access */
        res = uvc_open(dev, &devh);
        
        if (res < 0) {
            uvc_perror(res, "uvc_open"); /* unable to open device */
        } else {
            puts("Device opened");
            
            /* Print out a message containing all the information that libuvc
             * knows about the device */
            uvc_print_diag(devh, stderr);
            
            /* Try to negotiate a 640x480 30 fps YUYV stream profile */
            res = uvc_get_stream_ctrl_format_size(
                                                  devh, &ctrl, /* result stored in ctrl */
                                                  UVC_FRAME_FORMAT_H264, /* YUV 422, aka YUV 4:2:2. try _COMPRESSED */
                                                  1920, 1080, 30 /* width, height, fps */
                                                  );
            
            /* Print out the result */
            uvc_print_stream_ctrl(&ctrl, stderr);
            
            if (res < 0) {
                uvc_perror(res, "get_mode"); /* device doesn't provide a matching stream */
            } else {
                /* Start the video stream. The library will call user function cb:
                 *   cb(frame, (void*) 12345)
                 */
                //uvc_start_streaming(<#uvc_device_handle_t *devh#>, uvc_stream_ctrl_t *ctrl, <#uvc_frame_callback_t *cb#>, void *user_ptr, <#uint8_t flags#>)

                void *user_ptr=0;
                uint8_t flags=0;
                res = uvc_start_streaming(devh, &ctrl, cb, user_ptr, 0);
                usleep(3000000);
                if (res < 0) {
                    uvc_perror(res, "start_streaming"); /* unable to start stream */
                } else {
                    puts("Streaming...");
                    
                    uvc_set_ae_mode(devh, 1); /* e.g., turn on auto exposure */
                    //uvc_frame_t *frame;
                    uvc_stream_handle_t *strmh;
                    
//                    res = uvc_stream_open_ctrl(devh,  &strmh, &ctrl);
//                    res = uvc_stream_start(strmh, nullptr, nullptr, 0);
//
//                    while(1)
//                    {
//                        uvc_frame_t *frame;
//
//                        res=uvc_stream_get_frame(strmh, &frame, 30);
//                        if(res==0){
//                        FILE *fp;
//                        fp = fopen("2.alex", "a");
//                        fwrite(frame->data, 1, frame->data_bytes, fp);
//                        fclose(fp);
//                        }else{
//                            printf("error get frame");}
//                    }
                    // cvWaitKey(0);
                    /* End the stream. Blocks until last callback is serviced */
                    //      uvc_stop_streaming(devh);
                    puts("Done streaming.");
                }
            }
            
            /* Release our handle on the device */
            //usleep(10000000000); /* stream for 10 seconds */
            uvc_close(devh);
            puts("Device closed");
        }
        
        /* Release the device descriptor */
        uvc_unref_device(dev);
    }
    
    /* Close the UVC context. This closes and cleans up any existing device handles,
     * and it closes the libusb context if one was not provided. */
    uvc_exit(ctx);
    puts("UVC exited");
    
    return 0;
}
