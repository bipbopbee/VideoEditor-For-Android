package com.example.cj.videoeditor.rtmp;
public class RtmpPusher {
    private long cPtr;
    private long timeOffset;

    public static RtmpPusher newInstance() {

        return new RtmpPusher();
    }
    private RtmpPusher(){}

    public int init(String url, int w, int h, int timeOut) {
        cPtr = PublishNative.Publish_init(url, w, h, timeOut);
        if (cPtr != 0) {
            return 0;
        }
        return -1;
    }

    public int sendSpsAndPps(byte[] sps, int spsLen, byte[] pps, int ppsLen, long timeOffset) {
        this.timeOffset = timeOffset;
        return PublishNative.Publish_sendSpsAndPps(cPtr, sps, spsLen, pps, ppsLen, 0);
    }

    public int sendVideoData(byte[] data, int len, long timestamp) {
        if(timestamp-timeOffset<=0){return -1;}
        return PublishNative.Publish_sendVideoData(cPtr, data, len, timestamp - timeOffset);
    }

    public int sendAacSpec(byte[] data, int len) {
        return PublishNative.Publish_sendAacSpec(cPtr, data, len);
    }

    public int sendAacData(byte[] data, int len, long timestamp) {
        if(timestamp-timestamp<0){return -1;}
        return PublishNative.Publish_sendAacData(cPtr, data, len, timestamp - timeOffset);
    }

    public int stop() {
        try {
            return PublishNative.Publish_stop(cPtr);
        }finally {
            cPtr=0;
        }
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        if(cPtr!=0){
            stop();
        }
    }
}

