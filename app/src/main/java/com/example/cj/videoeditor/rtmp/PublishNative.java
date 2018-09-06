package com.example.cj.videoeditor.rtmp;

public class PublishNative {
    static {
        System.loadLibrary("publish");
    }

    public static native long Publish_init(String url, int w, int h, int timeOut);

    public static native int Publish_sendSpsAndPps(long cptr, byte[] sps, int spsLen, byte[] pps,
                                    int ppsLen, long timestamp);

    public static native int Publish_sendVideoData(long cptr, byte[] data, int len, long timestamp);

    public static native int Publish_sendAacSpec(long cptr, byte[] data, int len);

    public static native int Publish_sendAacData(long cptr, byte[] data, int len, long timestamp);

    public static native int Publish_stop(long cptr);
}
