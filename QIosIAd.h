
#ifndef __QIOSIAD_H
#define __QIOSIAD_H

// tolua_begin

namespace iosIAd {

    bool isAvailable();
    const char* getError();
    int getProperty(const char* prop);
    bool setProperty(const char* prop, int value);
    bool start();
    bool stop();
    bool cancel();
    bool s3eExtIOSIAdAvailable();
    
} //namespace iosIAd

// tolua_end

#endif // QIOSIAD_H
