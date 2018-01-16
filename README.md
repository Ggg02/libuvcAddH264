# libuvcAddH264
how to add h264 in libuvc 

## Add H264 to libUVC
just note my history , please feel free to share .

1. build libuvc with xcode (in this git)
2. wirte test code for capture  test. ( also in this git)
```
vid/pid change
res = uvc_find_device(
                          ctx, &dev,
                          VID, PID, NULL); 
```
3. set break point in stream.c/uvc_get_stream_ctrl_format_size
```
 if (!_uvc_frame_format_matches_guid(cf, format->guidFormat)) // set point here check device supported format 
      continue;
```
4. check H264 fromat .
5. add H264 format .


6. it's done for add H264

## License
This project is licensed under the MIT License
